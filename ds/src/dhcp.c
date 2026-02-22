/*
Writer:  Robi
Checker: ?
Date:    22.02.2026
*/

#include <assert.h> /* assert         */
#include <limits.h> /* CHAR_BIT       */
#include <stdlib.h> /* malloc, free   */
#include <string.h> /* memcpy, memcmp */

#include "dhcp.h" /* our api        */
#include "trie.h" /* TrieCreate     */

#define BITS_IN_IP (LENGTH_OF_IP * CHAR_BIT)

struct dhcp
{
    trie_t* trie;
    size_t num_of_net_bits;
    unsigned char subnet_id[LENGTH_OF_IP];
};

static size_t GetCharNumToAdvance(dhcp_t* dhcp)
{
    return dhcp->num_of_net_bits / CHAR_BIT;
}

static void SetIPBit(unsigned char* ip, size_t bit_index, int value)
{
    unsigned char mask = 0x80 >> (bit_index % CHAR_BIT);

    assert(NULL != ip);

    if (value)
    {
        ip[bit_index / CHAR_BIT] = ip[bit_index / CHAR_BIT] | mask;

        return;
    }

    ip[bit_index / CHAR_BIT] = ip[bit_index / CHAR_BIT] & ~mask;
}

static void CopySubNet(dhcp_t* dhcp, unsigned char out_id[LENGTH_OF_IP])
{
    unsigned char mask = 0x80;
    size_t full_byts = 0;
    size_t reminder = 0;
    size_t i = 0;

    assert(NULL != dhcp);
    assert(NULL != out_id);

    full_byts = dhcp->num_of_net_bits / CHAR_BIT;
    reminder = dhcp->num_of_net_bits & (CHAR_BIT - 1);

    for (; i < full_byts; ++i)
    {
        out_id[i] = dhcp->subnet_id[i];
    }

    for (i = 0; i < reminder; ++i)
    {
        SetIPBit(out_id + full_byts, i, !!(dhcp->subnet_id[i] & mask));
        mask = mask >> 1;
    }
}

static void BuildHostPattern(unsigned char* adress, size_t host_bits,
                             int fill_value, int last_bit_value)
{
    size_t i = 0;

    assert(NULL != adress);

    /*memset(adress, 0, LENGTH_OF_IP);  ill make all zeros here?*/
    for (; i < host_bits; ++i)
    {
        SetIPBit(adress, i, (i < host_bits - 1) ? fill_value : last_bit_value);
    }
}

static dhcp_status_t InitAddresses(dhcp_t* dhcp)
{
    unsigned char network_address[LENGTH_OF_IP] = {0};
    unsigned char server_address[LENGTH_OF_IP] = {0};
    unsigned char broadcast_address[LENGTH_OF_IP] = {0};

    assert(NULL != dhcp);

    CopySubNet(dhcp, network_address);
    BuildHostPattern(broadcast_address, BITS_IN_IP, 1, 1);
    BuildHostPattern(server_address, BITS_IN_IP, 1, 0);

    if (SUCCESS != TrieInsert(dhcp->trie,
                              network_address + GetCharNumToAdvance(dhcp)) ||
        SUCCESS != TrieInsert(dhcp->trie,
                              server_address + GetCharNumToAdvance(dhcp)) ||
        SUCCESS != TrieInsert(dhcp->trie,
                              broadcast_address + GetCharNumToAdvance(dhcp)))
    {
        return DHCP_ALLOC_FAIL;
    }

    return DHCP_SUCCESS;
}

dhcp_t* DHCPCreate(const unsigned char subnet_id[LENGTH_OF_IP],
                   size_t num_of_net_bits)
{
    dhcp_t* dhcp = NULL;

    assert(NULL != subnet_id);

    dhcp = (dhcp_t*)malloc(sizeof(dhcp_t));
    if (NULL == dhcp)
    {
        return NULL;
    }

    dhcp->trie = TrieCreate(BITS_IN_IP - num_of_net_bits);
    if (NULL == dhcp->trie)
    {
        free(dhcp);
        dhcp = NULL;

        return NULL;
    }

    memcpy(dhcp->subnet_id, subnet_id, LENGTH_OF_IP);
    dhcp->num_of_net_bits = num_of_net_bits;
    if (DHCP_SUCCESS != InitAddresses(dhcp))
    {
        TrieDestroy(dhcp->trie);
        dhcp->trie = NULL;
        free(dhcp);
        dhcp = NULL;

        return NULL;
    }

    return dhcp;
}

void DHCPDestroy(dhcp_t* dhcp)
{
    assert(NULL != dhcp);

    TrieDestroy(dhcp->trie);
    dhcp->trie = NULL;
    free(dhcp);
    dhcp = NULL;
}

size_t DHCPCount(const dhcp_t* dhcp)
{
    assert(NULL != dhcp);

    return (1 << (BITS_IN_IP - dhcp->num_of_net_bits)) - TrieCount(dhcp->trie);
}

static void CopyAddress(unsigned char dest[LENGTH_OF_IP],
                        const unsigned char src[LENGTH_OF_IP])
{
    size_t i = 0;

    assert(NULL != dest);
    assert(NULL != src);

    for (; i < LENGTH_OF_IP; ++i)
    {
        dest[i] = src[i];
    }
}

dhcp_status_t DHCPAlloc(dhcp_t* dhcp, const unsigned char req_id[LENGTH_OF_IP],
                        unsigned char out_id[LENGTH_OF_IP])
{
    trie_status_t status = SUCCESS;

    assert(NULL != dhcp);
    assert(NULL != req_id);
    assert(NULL != out_id);

    CopyAddress(out_id, req_id);

    if (TrieFind(dhcp->trie, req_id + GetCharNumToAdvance(dhcp)) &&
        SUCCESS != TrieGetFreeNext(dhcp->trie,
                                   req_id + GetCharNumToAdvance(dhcp),
                                   out_id + GetCharNumToAdvance(dhcp)) &&
        SUCCESS != TrieGetFreeNext(dhcp->trie,
                                   dhcp->subnet_id + GetCharNumToAdvance(dhcp),
                                   out_id + GetCharNumToAdvance(dhcp)))
    {
        return DHCP_ALLOC_FAIL;
    }

    status = TrieInsert(dhcp->trie, out_id + GetCharNumToAdvance(dhcp));
    switch (status)
    {
    case SUCCESS:
        /*CopySubNet(dhcp, out_id);*/
        return DHCP_SUCCESS;
    default:
        return DHCP_ALLOC_FAIL;
    }
}

dhcp_status_t DHCPFree(dhcp_t* dhcp, const unsigned char free_id[LENGTH_OF_IP])
{
    assert(NULL != dhcp);

    if (!TrieFind(dhcp->trie, free_id + BITS_IN_IP - dhcp->num_of_net_bits))
    {
        return DHCP_INVALID_ADDRESSES;
    }

    TrieFree(dhcp->trie, free_id + BITS_IN_IP - dhcp->num_of_net_bits);

    return DHCP_SUCCESS;
}

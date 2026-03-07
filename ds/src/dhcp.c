/*
Writer:  Robi
Checker: ?
Date:    07.03.2026
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

static void SetIPBit(unsigned char* ip, size_t bit_index, int value)
{
    unsigned char mask = 0x80 >> (bit_index & (CHAR_BIT - 1));

    assert(NULL != ip);

    ip[bit_index / CHAR_BIT] = value ? ip[bit_index / CHAR_BIT] | mask
                                     : ip[bit_index / CHAR_BIT] & ~mask;
}

static void BuildHostPattern(unsigned char* adress, size_t host_bits,
                             int fill_value, int last_bit_value)
{
    size_t i = 0;

    assert(NULL != adress);

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
    size_t host_bits = 0;

    assert(NULL != dhcp);

    host_bits = BITS_IN_IP - dhcp->num_of_net_bits;
    BuildHostPattern(server_address, host_bits, 1, 0);
    BuildHostPattern(broadcast_address, host_bits, 1, 1);

    if (SUCCESS != TrieInsert(dhcp->trie, network_address) ||
        SUCCESS != TrieInsert(dhcp->trie, server_address) ||
        SUCCESS != TrieInsert(dhcp->trie, broadcast_address))
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

/*not truly suuport ipv6, becuase even if i
 * make the calc suuport it, the return
 * type in lab header is size_t so it wont
 * be able to represent this number..
 * */
size_t DHCPCount(const dhcp_t* dhcp)
{
    assert(NULL != dhcp);

    return ((size_t)1 << (BITS_IN_IP - dhcp->num_of_net_bits)) -
           TrieCount(dhcp->trie);
}

static int GetIPBit(const unsigned char* ip, size_t bit_index)
{
    unsigned char mask = 0x80 >> (bit_index & (CHAR_BIT - 1));

    assert(NULL != ip);

    return !!(ip[bit_index / CHAR_BIT] & mask);
}

static void ExtractHostPart(const dhcp_t* dhcp,
                            const unsigned char ip[LENGTH_OF_IP],
                            unsigned char host_part[LENGTH_OF_IP])
{
    size_t host_bits = 0;
    size_t i = 0;

    assert(NULL != dhcp);
    assert(NULL != ip);
    assert(NULL != host_part);

    host_bits = BITS_IN_IP - dhcp->num_of_net_bits;
    memset(host_part, 0, LENGTH_OF_IP);
    for (; i < host_bits; ++i)
    {
        SetIPBit(host_part, i, GetIPBit(ip, dhcp->num_of_net_bits + i));
    }
}

static void BuildIPFromHostPart(const dhcp_t* dhcp,
                                const unsigned char host_part[LENGTH_OF_IP],
                                unsigned char out_id[LENGTH_OF_IP])
{
    size_t i = 0;
    size_t host_bits = 0;

    assert(NULL != dhcp);
    assert(NULL != host_part);
    assert(NULL != out_id);

    host_bits = BITS_IN_IP - dhcp->num_of_net_bits;
    memset(out_id, 0, LENGTH_OF_IP);
    for (; i < dhcp->num_of_net_bits; ++i)
    {
        SetIPBit(out_id, i, GetIPBit(dhcp->subnet_id, i));
    }

    for (i = 0; i < host_bits; ++i)
    {
        SetIPBit(out_id, dhcp->num_of_net_bits + i, GetIPBit(host_part, i));
    }
}

static int IsInSubnet(const dhcp_t* dhcp, const unsigned char ip[LENGTH_OF_IP])
{
    size_t i = 0;

    assert(NULL != dhcp);
    assert(NULL != ip);

    for (; i < dhcp->num_of_net_bits; ++i)
    {
        if (GetIPBit(ip, i) != GetIPBit(dhcp->subnet_id, i))
        {
            return 0;
        }
    }

    return 1;
}

static int IsReservedHostPart(const dhcp_t* dhcp,
                              const unsigned char host_part[LENGTH_OF_IP])
{
    size_t host_bits = 0;
    unsigned char reserved[LENGTH_OF_IP] = {0};

    assert(NULL != dhcp);
    assert(NULL != host_part);

    host_bits = BITS_IN_IP - dhcp->num_of_net_bits;
    if (0 == memcmp(host_part, reserved, LENGTH_OF_IP))
    {
        return 1;
    }

    BuildHostPattern(reserved, host_bits, 1, 0);
    if (0 == memcmp(host_part, reserved, LENGTH_OF_IP))
    {
        return 1;
    }

    memset(reserved, 0, LENGTH_OF_IP);
    BuildHostPattern(reserved, host_bits, 1, 1);

    return (0 == memcmp(host_part, reserved, LENGTH_OF_IP));
}

dhcp_status_t DHCPAlloc(dhcp_t* dhcp, const unsigned char req_id[LENGTH_OF_IP],
                        unsigned char out_id[LENGTH_OF_IP])
{
    unsigned char requested_host[LENGTH_OF_IP] = {0};
    unsigned char allocated_host[LENGTH_OF_IP] = {0};
    unsigned char zero_host[LENGTH_OF_IP] = {0};
    trie_status_t status = SUCCESS;

    assert(NULL != dhcp);
    assert(NULL != out_id);
    assert(NULL != req_id);

    ExtractHostPart(dhcp, req_id, requested_host);
    if (TrieFind(dhcp->trie, requested_host))
    {
        if (SUCCESS !=
                TrieGetFreeNext(dhcp->trie, requested_host, allocated_host) &&
            SUCCESS != TrieGetFreeNext(dhcp->trie, zero_host, allocated_host))
        {
            return DHCP_ALLOC_FAIL;
        }
    }
    else
    {
        memcpy(allocated_host, requested_host, LENGTH_OF_IP);
    }

    status = TrieInsert(dhcp->trie, allocated_host);
    if (SUCCESS != status)
    {
        return DHCP_ALLOC_FAIL;
    }

    BuildIPFromHostPart(dhcp, allocated_host, out_id);

    return DHCP_SUCCESS;
}

dhcp_status_t DHCPFree(dhcp_t* dhcp, const unsigned char free_id[LENGTH_OF_IP])
{
    unsigned char host_part[LENGTH_OF_IP] = {0};

    assert(NULL != dhcp);
    assert(NULL != free_id);

    if (!IsInSubnet(dhcp, free_id))
    {
        return DHCP_INVALID_ADDRESSES;
    }

    ExtractHostPart(dhcp, free_id, host_part);
    if (IsReservedHostPart(dhcp, host_part) || !TrieFind(dhcp->trie, host_part))
    {
        return DHCP_INVALID_ADDRESSES;
    }

    TrieFree(dhcp->trie, host_part);

    return DHCP_SUCCESS;
}

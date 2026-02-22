/*
Writer:  Robi
Checker: ?
Date:    22.02.2026
*/

#include <stdlib.h> /* malloc, free   */
#include <string.h> /* memcpy, memcmp */
#include <assert.h> /* assert         */
#include <limits.h> /* CHAR_BIT       */

#include "dhcp.h"   /* our api        */
#include "trie.h"   /* TrieCreate     */

#define BITS_IN_IP (LENGTH_OF_IP * CHAR_BIT)

struct dhcp
{
    trie_t* trie;
    size_t num_of_host_bits;
    unsigned char subnet_id[LENGTH_OF_IP];
};

static void SetIPBit(unsigned char* ip, size_t bit_index, int value)
{
    unsigned char mask = 0x80 >> (bit_index % CHAR_BIT);

    assert (NULL != ip);

    if (value)
    {
        ip[bit_index / CHAR_BIT] = ip[bit_index / CHAR_BIT] | mask;

        return;
    }

    ip[bit_index / CHAR_BIT] =  ip[bit_index / CHAR_BIT] & ~mask;
}

static void BuildHostPattern(unsigned char* adress, size_t host_bits, int fill_value, int last_bit_value)
{
    size_t i = 0;

    assert (NULL != adress);

    /*memset(adress, 0, LENGTH_OF_IP);  ill make all zeros here?*/
    for (; i < host_bits; ++i)
    {
        SetIPBit(adress, i, (i < host_bits - 1) ? fill_value : last_bit_value);
    }
}


static trie_status_t InitAddresses(trie_t* trie, size_t num_of_net_bits)
{
    unsigned char network_address[LENGTH_OF_IP] = {0};
    unsigned char server_address[] = {0};
    unsigned char broadcast_address[] = {0};

    assert (NULL != trie);

    memset(network_address, 0, LENGTH_OF_IP);
    BuildHostPattern(broadcast_address, LENGTH_OF_IP - num_of_net_bits, 1, 1);
    BuildHostPattern(server_address, LENGTH_OF_IP - num_of_net_bits, 1, 0);

    if (SUCCESS != TrieInsert(trie, network_address) || SUCCESS != TrieInsert(trie, server_address)
     || SUCCESS != TrieInsert(trie, broadcast_address))
     {
        return ALLOC_FAIL;
     }

     return SUCCESS;
}

dhcp_t* DHCPCreate(const unsigned char subnet_id[LENGTH_OF_IP], size_t num_of_net_bits)
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
        free(dhcp); dhcp = NULL;
        
        return NULL;
    }

    if (SUCCESS != InitAddresses(dhcp->trie, num_of_net_bits))
    {
        TrieDestroy(dhcp->trie); 
        dhcp->trie = NULL;
        free(dhcp); dhcp = NULL;
        
        return NULL;
    }

    dhcp->num_of_host_bits = num_of_net_bits;
    memcpy(dhcp->subnet_id, subnet_id, LENGTH_OF_IP);

    return dhcp;
}

void DHCPDestroy(dhcp_t* dhcp)
{
    assert (NULL != dhcp);
    
    TrieDestroy(dhcp->trie); 
    dhcp->trie = NULL;
    free(dhcp); dhcp = NULL;
}

size_t DHCPCount(const dhcp_t* dhcp)
{
    assert (NULL != dhcp);
    
    return TrieCount(dhcp->trie);
}

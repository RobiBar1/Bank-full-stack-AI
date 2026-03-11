#ifndef __DHCP_H__
#define __DHCP_H__

#include <stddef.h>

typedef enum  
{
    DHCP_SUCCESS,
    DHCP_ALLOC_FAIL,
    DHCP_INVALID_ADDRESSES
} dhcp_status_t;

typedef struct dhcp dhcp_t; 

#define LENGTH_OF_IP (4)


/*
struct dhcp
{
  trie_t* trie;
  size_t num_of_host_bits;
  const unsigned char subnet_id[LENGTH_OF_IP];
};
*/
/**
 * @brief - creates a dhcp entity
 * first, second and last addresses are taken by default
 * @param[in] - subnet_id - the network ID 
 * @param[in] - num_of_net_bits - length of network ID
 * @pre - subnet_id != NULL
 * @pre - 0 < num_of_net_bits and LENGTH_OF_IP * 8 - num_of_net_bits > 1
 * @returns dhcp_t pointer to dhcp service entity
 */ 
dhcp_t* DHCPCreate(const unsigned char subnet_id[LENGTH_OF_IP], size_t num_of_net_bits);

/**
 * @brief - destroys the given dhcp
 * @param[in] - the dhcp to destroy
 * @pre - dhcp not NULL
 * @returns nothing
 */ 
void DHCPDestroy(dhcp_t* dhcp);
/**
 * @brief - finds a next free ip address 
 * @param[in] - dhcp
 * @param[in] - charreq_id, the ip to alloc
 * @pre - NULL != dhcp
 * @pre - NULL != charreq_id
 * @pre - NULL != out_id
 * @returns the status of the operation
 */ 
dhcp_status_t DHCPAlloc(dhcp_t* dhcp, const unsigned char req_id[LENGTH_OF_IP], unsigned char out_id[LENGTH_OF_IP]);

/**
 * @brief - frees the ip given by the user
 * @param[in] - dhcp
 * @param[in] - free_id, the ip to free
 * @pre - NULL != free_id
 * @returns the status of the operation
 */ 
dhcp_status_t DHCPFree(dhcp_t* dhcp, const unsigned char free_id[LENGTH_OF_IP]);

/**
 * @brief counts the number of free ip addresses
 * @return size_t - the number of free ip addresses
 * @param[in] dhcp  
 * @pre dhcp != NULL
 * @complexity: O(n)
 */
size_t DHCPCount(const dhcp_t* dhcp);

#endif /* __DHCP_H__ */

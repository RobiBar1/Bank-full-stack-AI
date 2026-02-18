/*
Writer:  Robi
Checker: ?
Date:    19.02.2026
*/

#include <assert.h> /* assert   */
#include <limits.h> /* CHAR_BIT */
#include <stdlib.h> /* calloc   */

#include "trie.h"

typedef enum {LEFT, RIGHT, NUM_OF_CHILDRENS} childrens_t;

typedef struct trie_node
{
    struct trie_node* childrens[2];
    struct trie_node* parent;
    int is_occupied;
}trie_node_t;

struct trie
{
    trie_node_t* root;
    size_t depth;
};

static trie_node_t* CreateTrieNode(trie_node_t* parent)
{
    trie_node_t* trie_node = NULL;
    trie_node = (trie_node_t*)calloc(1, sizeof(trie_node_t));
    

    trie_node->parent = parent;
    trie_node->is_occupied = 0;

    return trie_node;
}

trie_t* TrieCreate(size_t depth)
{
    trie_t* trie = (trie_t*)malloc(sizeof(trie_t));
    if (NULL == trie)
    {
        return NULL;
    }

    trie->root = CreateTrieNode(NULL);
    if (NULL == trie->root)
    {
        free(trie); trie = NULL;
        return NULL;
    }

    trie->depth = depth;

    return trie;
}

static void HelperDestory(trie_node_t* node)
{
    if (NULL == node)
    {
        return;
    }

    if (NULL != node->childrens[LEFT])
    {
        HelperDestory(node->childrens[LEFT]);
    }
    if (NULL != node->childrens[RIGHT])
    {
        HelperDestory(node->childrens[RIGHT]);
    }

    free(node); node = NULL;
}

void desroy(trie_t* trie)
{
    assert (NULL != trie);

    HelperDestory(trie->root);
    free(trie); trie = NULL;
}

int TrieIsEmpty(const trie_t* trie)
{
    assert (NULL != trie);

    return (NULL == trie->root->childrens[LEFT] && NULL == trie->root->childrens[RIGHT]);
}

trie_status_t TrieInsert(trie_t* trie, const unsigned char* word)
{
    trie_node_t* runner = NULL;
    size_t ip_num = 0;
    size_t i = 0;
    size_t j = 0;
    int mask = 0;

    assert (NULL != trie);
    assert (NULL != word);

    ip_num = trie->depth;
    runner = trie->root;
    mask = 0x01 << (ip_num & (CHAR_BIT - 1));
    for (; i < trie->depth; i += j - 1)
    {
        for (j = 0; j < (ip_num & (CHAR_BIT - 1)); ++j, --ip_num)
        {
            if (NULL == runner->childrens[!!(word[i] & mask)])
            {
                runner->childrens[!!(word[i] & mask)] = CreateTrieNode(runner);
                if (NULL == runner->childrens[!!(word[i] & mask)])
                {
                    return ALLOC_FAIL;
                }
            }

            runner = runner->childrens[!!(word[i] & mask)];
            mask = mask >> 1;
        }

        mask = 0x01 << (CHAR_BIT - 1);
    }

    return SUCCESS;
}

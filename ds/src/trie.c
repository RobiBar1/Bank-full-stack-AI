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

static trie_status_t GoNextChild(const unsigned char* word, size_t mask, trie_node_t* runner, size_t i)
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

    return SUCCESS;
}

static void UpdateParentOccupied(trie_node_t* runner)
{
    while (NULL != runner->parent)
    {
        if (NULL != runner->childrens[LEFT] && NULL != runner->childrens[RIGHT])    
        {
            runner->is_occupied = runner->childrens[LEFT]->is_occupied && runner->childrens[RIGHT]->is_occupied;
        }
    }
}

trie_status_t TrieInsert(trie_t* trie, const unsigned char* word)
{
    trie_node_t* runner = NULL;
    trie_status_t status = SUCCESS;
    size_t tmp_hight = 0;
    size_t i = 0;
    size_t j = 0;
    size_t mask = 0;

    assert (NULL != trie);
    assert (NULL != word);

    tmp_hight = trie->depth;
    runner = trie->root;
    mask = 0x01 << (tmp_hight & (CHAR_BIT - 1));
    for (; tmp_hight; ++i)
    {
        for (j = 0; j < (tmp_hight & (CHAR_BIT - 1)); ++j, --tmp_hight)
        {
            status = GoNextChild(word, mask, runner, i);///
            if (SUCCESS != status)
            {
                return status;
            }

            mask = mask >> 1;
        }

        mask = 0x01 << (CHAR_BIT - 1);
    }

    runner->is_occupied = 1;
    UpdateParentOccupied(runner->parent);
    
    return SUCCESS;
}

static size_t HelperCount(const trie_node_t* node)
{
    size_t sum = 1;

    if (NULL == node)
    {
        return 0;
    }

    sum += NULL != node->childrens[LEFT] ? HelperCount(node->childrens[LEFT]) : 0;
    sum += NULL != node->childrens[RIGHT] ? HelperCount(node->childrens[RIGHT]) : 0;

    return sum;
}

size_t TrieCount(const trie_t* trie)
{
    assert (NULL != trie);

    return HelperCount(trie->root) - 1;
}

/*@pre: */
int TrieFind(trie_t* trie, const unsigned char* word)
{
    trie_node_t* runner = NULL;
    trie_status_t status = SUCCESS;
    size_t tmp_hight = 0;
    size_t i = 0;
    size_t j = 0;
    size_t mask = 0;

    assert (NULL != trie);
    assert (NULL != word);

    tmp_hight = trie->depth;
    runner = trie->root;
    mask = 0x01 << (tmp_hight & (CHAR_BIT - 1));
    for (; i < trie->depth; i += j - 1)
    {
        
    }

    assert (NULL != trie);
    

}




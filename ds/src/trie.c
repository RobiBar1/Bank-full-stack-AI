/*
Writer:  Robi
Checker: ?
Date:    07.03.2026
*/

#include <assert.h> /* assert   */
#include <limits.h> /* CHAR_BIT */
#include <stdlib.h> /* calloc   */
#include <string.h> /* memcpy   */

#include "trie.h"

#define NOT_FOUND 0
#define IS_LEAF(depth) (0 == (depth))
typedef enum
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDRENS
} childrens_t;

typedef struct trie_node
{
    struct trie_node* childrens[2];
    struct trie_node* parent;
    int is_occupied;
} trie_node_t;

struct trie
{
    trie_node_t* root;
    size_t depth;
    size_t count;
};

static trie_node_t* CreateTrieNode(trie_node_t* parent)
{
    trie_node_t* trie_node = (trie_node_t*)calloc(1, sizeof(trie_node_t));
    if (NULL == trie_node)
    {
        return NULL;
    }

    trie_node->parent = parent;

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
        free(trie);
        trie = NULL;
        return NULL;
    }

    trie->depth = depth;
    trie->count = 0;

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
        node->childrens[LEFT] = NULL;
    }
    if (NULL != node->childrens[RIGHT])
    {
        HelperDestory(node->childrens[RIGHT]);
        node->childrens[RIGHT] = NULL;
    }

    free(node);
    node = NULL;
}

void TrieDestroy(trie_t* trie)
{
    assert(NULL != trie);

    HelperDestory(trie->root);
    trie->root = NULL;
    free(trie);
    trie = NULL;
}

int TrieIsEmpty(const trie_t* trie)
{
    assert(NULL != trie);

    return (NULL == trie->root->childrens[LEFT] &&
            NULL == trie->root->childrens[RIGHT]);
}

static trie_status_t GoNextChild(const unsigned char* word, size_t height,
                                 trie_node_t** runner)
{
    childrens_t side = LEFT;

    assert(NULL != word);

    side = !!(word[height / CHAR_BIT] & (0x80 >> (height & (CHAR_BIT - 1))));
    if (NULL == (*runner)->childrens[side])
    {
        (*runner)->childrens[side] = CreateTrieNode(*runner);
        if (NULL == (*runner)->childrens[side])
        {
            return ALLOC_FAIL;
        }
    }

    (*runner) = (*runner)->childrens[side];

    return SUCCESS;
}

static void UpdateParentOccupied(trie_node_t* runner)
{
    int left = 0;
    int right = 0;

    while (NULL != runner)
    {
        left = NULL != runner->childrens[LEFT]
                   ? runner->childrens[LEFT]->is_occupied
                   : 0;
        right = NULL != runner->childrens[RIGHT]
                    ? runner->childrens[RIGHT]->is_occupied
                    : 0;

        runner->is_occupied = (left && right);

        runner = runner->parent;
    }
}

trie_status_t TrieInsert(trie_t* trie, const unsigned char* word)
{
    trie_status_t status = SUCCESS;
    trie_node_t* runner = NULL;
    size_t height = 0;

    assert(NULL != trie);
    assert(NULL != word);

    runner = trie->root;
    for (; height < trie->depth; ++height)
    {
        status = GoNextChild(word, height, &runner);
        if (SUCCESS != status)
        {
            return status;
        }
    }

    if (runner->is_occupied)
    {
        return TAKEN;
    }

    runner->is_occupied = 1;
    ++(trie->count);
    UpdateParentOccupied(runner->parent);

    return SUCCESS;
}
/*
static size_t HelperCount(const trie_node_t* node, size_t depth)
{
    if (NULL == node)
    {
        return 0;
    }

    if (0 == depth--)
    {
        return node->is_occupied;
    }

    return (HelperCount(node->childrens[LEFT], depth) +
HelperCount(node->childrens[RIGHT], depth));
}
*/

size_t TrieCount(const trie_t* trie)
{
    assert(NULL != trie);

    /*return HelperCount(trie->root, trie->depth);*/

    return (trie->count);
}

int TrieFind(trie_t* trie, const unsigned char* word)
{
    trie_node_t* runner = NULL;
    childrens_t side = LEFT;
    size_t height = 0;

    assert(NULL != trie);
    assert(NULL != word);

    runner = trie->root;
    for (; height < trie->depth; ++height)
    {
        side =
            !!(word[height / CHAR_BIT] & (0x80 >> (height & (CHAR_BIT - 1))));
        if (NULL == runner->childrens[side])
        {
            return 0;
        }

        runner = runner->childrens[side];
    }

    return runner->is_occupied;
}

static int GetBit(const unsigned char* word, size_t height)
{
    return !!(word[height / CHAR_BIT] & (0x80 >> (height % CHAR_BIT)));
}

static void SetBit(unsigned char* word, size_t height, childrens_t value)
{
    unsigned char mask = 0x80 >> (height & (CHAR_BIT - 1));
    word[height / CHAR_BIT] =
        (word[height / CHAR_BIT] & ~mask) | (value ? mask : 0);
}

static int IsFree(trie_node_t* node)
{
    return (NULL == node || !node->is_occupied);
}

static void FillFirstFree(trie_node_t* node, unsigned char* out_word,
                          size_t height, size_t depth)
{
    childrens_t side = 0;

    for (; height < depth; ++height)
    {
        side = (NULL != node && !IsFree(node->childrens[LEFT]));
        SetBit(out_word, height, side);
        node = (NULL != node) ? node->childrens[side] : NULL;
    }
}

trie_status_t TrieGetFreeNext(trie_t* trie, const unsigned char* word,
                              unsigned char* out_word)
{
    trie_node_t* runner = NULL;
    size_t height = 0;
    int found = 0;

    assert(NULL != trie);
    assert(NULL != word);
    assert(NULL != out_word);

    runner = trie->root;
    memcpy(out_word, word, (trie->depth + CHAR_BIT - 1) / CHAR_BIT);
    while (height < trie->depth)
    {
        runner = runner->childrens[GetBit(word, height++)];
    }

    while (runner != trie->root && !found)
    {
        runner = runner->parent;
        found = !GetBit(out_word, --height) && IsFree(runner->childrens[RIGHT]);
    }

    if (!found)
    {
        return NOT_EXIST;
    }

    SetBit(out_word, height, 1);
    FillFirstFree(runner->childrens[RIGHT], out_word, height + 1, trie->depth);

    return SUCCESS;
}

void TrieFree(trie_t* trie, const unsigned char* word)
{
    trie_node_t* runner = NULL;
    childrens_t side = LEFT;
    size_t height = 0;

    assert(NULL != trie);
    assert(NULL != word);

    runner = trie->root;
    for (; height < trie->depth; ++height)
    {
        side = GetBit(word, height);
        if (NULL == runner->childrens[side])
        {
            return;
        }

        runner = runner->childrens[side];
    }

    runner->is_occupied = 0;
    --(trie->count);
    UpdateParentOccupied(runner->parent);
}

/*
Writer:  Robi
Checker: Michel
Date:	 12.02.2026
*/

#include <assert.h> /* assert */
#include <stdlib.h> /* calloc, free */

#include "avl.h" 	/* our api */

#define LEFT_HEAVY (2)
#define RIGHT_HEAVY (-2)
#define RIGHT_LEAN (-1)
#define LEFT_LEAN (1)

typedef enum {LEFT, RIGHT, NUM_CHILDREN} children_t;

typedef struct avl_node
{
    void* data;
    struct avl_node* children[NUM_CHILDREN];
    long height;
} avl_node_t;


struct avl
{
    avl_node_t* dummy;
    int (*cmp)(const void* data, const void* param);
};

static long GetMax(long a, long b);
static int IsLeaf(avl_node_t* node);
static int HasNodeOneChild(avl_node_t* node);
static long GetMaxFromTwoChildren(avl_node_t* parent);
static int CheckUpdateHeight(avl_node_t* parent);
static void RemoveNodeOneChild(avl_node_t* node, children_t side);
static void DeleteNode(avl_node_t* parent, avl_node_t* node, children_t side);
static void* FindRemoveNextLogic(avl_node_t* parent, avl_node_t* node, children_t side);

static long GetMax(long a, long b)
{
    return ((a > b) * a + (a <= b) * b);
}

static long GetMaxFromTwoChildren(avl_node_t* parent)
{
    if(NULL != parent->children[LEFT] && NULL != parent->children[RIGHT])
    {
        return GetMax(parent->children[LEFT]->height, 
                      parent->children[RIGHT]->height);
    }
    if(NULL == parent->children[RIGHT] && NULL == parent->children[LEFT])
    {
        return -1;
    }
    if(NULL == parent->children[RIGHT])
    {
        return parent->children[LEFT]->height;
    }
    return parent->children[RIGHT]->height;
}

static int CheckUpdateHeight(avl_node_t* parent)
{
    long max = 0;

    assert (NULL != parent);

    max = GetMaxFromTwoChildren(parent);
    if (parent->height != ++max)
    {
        parent->height = max;
        return 1;
    }
    
    return 0;
}

static int IsLeaf(avl_node_t* node)
{
    assert(NULL != node);
    
    return (NULL == node->children[LEFT] && NULL == node->children[RIGHT]);
}

static int HasNodeOneChild(avl_node_t* node)
{
    assert(NULL != node);
    
    return (NULL == node->children[LEFT] || NULL == node->children[RIGHT]);
}

static avl_node_t* CreateNode(const void* data)
{
    avl_node_t* new_node = (avl_node_t*)calloc(1, sizeof(avl_node_t));
    if(NULL == new_node)
    {
        return NULL;
    }
    
    new_node->data = (void*)data;
    
    return new_node;
}

static long GetNodeHeight(avl_node_t* node)
{
    if(NULL == node)
    {
        return -1;
    }
    
    return node->height;
}

/* side is RIGHT for Right rotation */
static avl_node_t* Balance(avl_node_t* root, children_t side)
{
    avl_node_t* pivot = NULL;
    
    assert(NULL != root);
    
    pivot = root->children[!side];
    
    root->children[!side] = pivot->children[side];
    pivot->children[side] = root;
    
    return pivot;
}

static long GetBalanceFactor(avl_node_t* node)
{
    assert(NULL != node);

    return GetNodeHeight(node->children[LEFT]) - GetNodeHeight(node->children[RIGHT]);
}

static avl_node_t* CheckFixBalance(avl_node_t* node)
{
    int balance_fact = 0;
    avl_node_t* new_root = node;
    
    assert(NULL != node);
    
    balance_fact = GetBalanceFactor(node);
    
    if(LEFT_HEAVY == balance_fact)
    {
        if(RIGHT_LEAN == GetBalanceFactor(node->children[LEFT]))
        {
            node->children[LEFT] = Balance(node->children[LEFT], LEFT);
        }
        
        new_root = Balance(node, RIGHT);
        CheckUpdateHeight(new_root->children[LEFT]);
        CheckUpdateHeight(new_root->children[RIGHT]);
        CheckUpdateHeight(new_root);
    }
    else if(RIGHT_HEAVY == balance_fact)
    {
        if(LEFT_LEAN == GetBalanceFactor(node->children[RIGHT]))
        {
            node->children[RIGHT] = Balance(node->children[RIGHT], RIGHT);
        }
        
        new_root = Balance(node, LEFT);
        CheckUpdateHeight(new_root->children[LEFT]);
        CheckUpdateHeight(new_root->children[RIGHT]);
        CheckUpdateHeight(new_root);
    }
    
    return new_root;
}

avl_t* AVLCreate(int (*cmp)(const void* first, const void* second))
{
    avl_t* tree = NULL;
    avl_node_t* dummy = NULL;
    
    assert(NULL != cmp);
    
    tree = (avl_t*)calloc(1, sizeof(avl_t) + sizeof(avl_node_t));
    if(NULL == tree)
    {
        return NULL;
    }
    
    dummy = (avl_node_t*)((char*)tree + sizeof(avl_t));
    dummy->height = -1;
    
    tree->dummy = dummy;
    tree->cmp = cmp;
    
    return tree;
}

static void DestroyNodes(avl_node_t* node)
{
    if(NULL == node)
    {
        return;
    }
    if(IsLeaf(node))
    {
        free(node);
        return;
    }
    
    DestroyNodes(node->children[LEFT]);
    DestroyNodes(node->children[RIGHT]);
    free(node);
}

void AVLDestroy(avl_t* avl)
{
    assert(NULL != avl);
    
    DestroyNodes(avl->dummy->children[LEFT]);   
    
    free(avl); avl = NULL;
}

static avl_node_t* AVLInsertHelp(avl_node_t* node, avl_node_t* new_node, 
                         int (*cmp)(const void* data, const void* param))
{
    int res = 0;
    children_t side = 0;
    
    assert(NULL != node);
    assert(NULL != cmp);
    
    res = cmp(new_node->data, node->data);
    side = (0 < res);
    
    if(NULL == node->children[side])
    {
        node->children[side] = new_node;
        
        if (CheckUpdateHeight(node))
        {
            return CheckFixBalance(node);
        }
        
        return node;
    }
    
    node->children[side] = AVLInsertHelp(node->children[side], new_node, cmp);
    if (CheckUpdateHeight(node))
    {
        return CheckFixBalance(node);
    }
    
    return node;
}

int AVLInsert(avl_t* avl, const void* data)
{
    avl_node_t* new_node = NULL;

    assert(NULL != avl);
    
    if(NULL == (new_node = CreateNode(data)))
    {
        return 1;
    }
    
    if(AVLIsEmpty(avl))
    {
        avl->dummy->children[LEFT] = new_node;
    }
    else
    {
        avl->dummy->children[LEFT] = AVLInsertHelp(avl->dummy->children[LEFT], new_node, avl->cmp);
    }
    
    return 0;
}

static void RemoveNodeOneChild(avl_node_t* node, children_t side)
{
	avl_node_t* tmp = NULL;
	
	assert (NULL != node);
	
	node->data = node->children[side]->data;
	tmp = node->children[side];
	node->children[LEFT] = tmp->children[LEFT];
	node->children[RIGHT] = tmp->children[RIGHT];
	--node->height;
	free(tmp); tmp = NULL;
}

static void DeleteNode(avl_node_t* parent, avl_node_t* node, children_t side)
{
    void* tmp = NULL;

    assert(NULL != node);
    
    if(IsLeaf(node))
    {
        parent->children[side] = NULL;
        free(node); node = NULL;
    }
    else if(HasNodeOneChild(node))
    {
        RemoveNodeOneChild(node, (NULL == node->children[RIGHT]) ? LEFT : RIGHT);
    }
    else
    {
        tmp = FindRemoveNextLogic(node, node->children[RIGHT], RIGHT);
        node->data = tmp;
        CheckUpdateHeight(node);
        parent->children[side] = CheckFixBalance(node);
    }
}

/* finds next for a node that has a next node */
static void* FindRemoveNextLogic(avl_node_t* parent, avl_node_t* node, children_t side)
{
    void* data = NULL;
    
    assert(NULL != node);

    if(NULL != node->children[LEFT])
    {
        data = FindRemoveNextLogic(node, node->children[LEFT], LEFT);
        CheckUpdateHeight(node);
        parent->children[side] = CheckFixBalance(node);
        return data;
    }
    
    data = node->data;
    DeleteNode(parent, node, side);
    
    return data;
}

static void AVLRemoveHelp(avl_node_t* parent, avl_node_t* node, const void* data,
                         int (*cmp)(const void* data, const void* param), 
                         children_t prev_side)
{
    int res = 0;
    int side = 0;
    
    assert(NULL != node);
    assert(NULL != cmp);
    
    res = cmp(data, node->data);
    
    if(0 == res)
    {
        DeleteNode(parent, node, prev_side);
        return ;
    }
    
    side = 0 < res;
    
    AVLRemoveHelp(node ,node->children[side], data, cmp, side);
    CheckUpdateHeight(node);
    parent->children[prev_side] = CheckFixBalance(node);
}

void AVLRemove(avl_t* avl, const void* data)
{
    assert(NULL != avl);
    assert(!AVLIsEmpty(avl));
    
    AVLRemoveHelp(avl->dummy, avl->dummy->children[LEFT], data, avl->cmp, LEFT);
}

int AVLIsEmpty(const avl_t* avl)
{
    assert(NULL != avl);
    
    return (NULL == avl->dummy->children[LEFT]);
}

static int CountHelper(const void* data, void* param)
{
    (void)data;
    ++(*(size_t*)param);

    return 0;
}

size_t AVLCount(const avl_t* avl)
{
    size_t count = 0;
    
    assert (NULL != avl);
    
    AVLForEach(avl, CountHelper, &count, POST);

    return count;
}

ssize_t AVLHeight(const avl_t* avl)
{
    assert(NULL != avl);
    assert(!AVLIsEmpty(avl));
    
    return (avl->dummy->children[LEFT]->height);
}

static void* AVLFindHelp(avl_node_t* node, const void* data,
                         int (*cmp)(const void* data, const void* param))
{
    int res = 0;
    int side = 0;
    
    assert(NULL != cmp);
    
    if(NULL == node)
    {
        return NULL;
    }
    
    res = cmp(data, node->data);
    
    if(0 == res)
    {
        return node->data;
    }
    
    side = 0 < res;
    
    return AVLFindHelp(node->children[side], data, cmp);
}

void* AVLFind(const avl_t* avl, const void* data)
{
    assert(NULL != avl);
    
    return AVLFindHelp(avl->dummy->children[LEFT], data, avl->cmp);
}

int ForEachPost(const avl_node_t* node, 
                int (*action)(const void* data, void* param), void* param)
{
    int res = 0;

    if(NULL == node)
    {
        return 0;
    }
    
    if(0 != (res = ForEachPost(node->children[LEFT], action, param)) ||
       0 != (res = ForEachPost(node->children[RIGHT], action, param)))
    {
        return res;
    }
    
    return action(node->data, param);
}

int ForEachPre(const avl_node_t* node, 
               int (*action)(const void* data, void* param), void* param)
{
    int res = 0;

    if(NULL == node)
    {
        return 0;
    }
    
    if(0 != (res = action(node->data, param)) ||
       0 != (res = ForEachPre(node->children[LEFT], action, param)))
    {
        return res;
    }
    
    return ForEachPre(node->children[RIGHT], action, param);
}

int ForEachIn(const avl_node_t* node, 
              int (*action)(const void* data, void* param), void* param)
{
    int res = 0;

    if(NULL == node)
    {
        return 0;
    }
    
    if(0 != (res = ForEachIn(node->children[LEFT], action, param)) ||
       0 != (res = action(node->data, param)))
    {
        return res;
    }
    
    return ForEachIn(node->children[RIGHT], action, param);
}


int AVLForEach(const avl_t* avl, int (*action)(const void* data, void* param), void* param, traversal_ord_t traversal_ord)
{
    assert(NULL != avl);
    assert(NULL != action);
    assert(0 <= traversal_ord);
    assert(NUM_OF_TRAVERSALS > traversal_ord);

    switch(traversal_ord)
    {
        case POST:
            return ForEachPost(avl->dummy->children[LEFT], action, param);
            break;
        case PRE:
            return ForEachPre(avl->dummy->children[LEFT], action, param);
            break;
        default:
            return ForEachIn(avl->dummy->children[LEFT], action, param);
    }
}

static void AVLMultiFindHelp(const avl_node_t* node, void* param, 
     vector_t* out_vector, int (*is_match)(const void* data, const void* param))
{
    assert(NULL != out_vector);
    assert(NULL != is_match);

    if(NULL == node)
    {
        return;
    }
    
    if(is_match(node->data, param))
    {
        if (0 != VectorPushBack(out_vector, node->data))
        {
            return;
        }
    }
    
    AVLMultiFindHelp(node->children[LEFT], param, out_vector, is_match);
    AVLMultiFindHelp(node->children[RIGHT], param, out_vector, is_match);
}

void AVLMultiFind(const avl_t* avl, 
                  int (*is_match)(const void* data, const void* param), 
                  void* param, vector_t* out_vector)
{
    assert(NULL != avl);
    assert(NULL != is_match);
    assert(NULL != out_vector);
    
    AVLMultiFindHelp(avl->dummy->children[LEFT], param, out_vector, is_match);
}

static void AVLMultiRemoveHelp(avl_t* avl, avl_node_t* node, void* param,
                  int (*is_match)(const void* data, const void* param))
{
    assert(NULL != avl);
    assert(NULL != is_match);

    if(NULL == node)
    {
        return;
    }
    
    AVLMultiRemoveHelp(avl, node->children[LEFT], param, is_match);
    AVLMultiRemoveHelp(avl, node->children[RIGHT], param, is_match);
    
    if(is_match(node->data, param))
    {
        AVLRemove(avl, node->data);
    }
}

void MultiRemove(avl_t* avl, 
                 int (*is_match)(const void* data, const void* param), 
                 void* param)
{
    assert(NULL != avl);
    assert(NULL != is_match);
    
    AVLMultiRemoveHelp(avl, avl->dummy->children[LEFT], param, is_match);
}



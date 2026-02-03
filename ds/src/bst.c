#include <assert.h> /* assert  */
#include <stdlib.h> /* malloc  */

#include "bst.h" 	/* our api */

#define FOUND_ELEMNT(num) ((num) == 0)
#define LEFT_BIGGER(num) ((num) > 0) 
#define RIGHT_BIGGER(num) ((num) < 0) 
#define SUCCESS (0) 
#define UNUSED(x) ((void)(x))

typedef enum {LEFT, RIGHT, NUM_CHILDREN} children_t;

typedef struct bst_node
{
    void* data;
    struct bst_node* parent;
    struct bst_node* children[NUM_CHILDREN];
} bst_node_t;

struct bst
{
    bst_node_t* end;
    int (*cmp)(const void* data, const void* param);
};

/*
typedef struct bst_node* bst_iter_t;
typedef struct bst bst_t;
*/

bst_node_t* CreateBSTNode(const void* data, const bst_node_t* parent,
		   const bst_node_t* left, const bst_node_t* right)
{
	bst_node_t* new_bst_node = (bst_node_t*)malloc(sizeof(bst_node_t));
	if (NULL == new_bst_node)
	{
		return NULL;
	}
	
	new_bst_node->data = (void*)data;
	new_bst_node->parent = (bst_node_t*)parent;
	new_bst_node->children[LEFT] = (bst_node_t*)left;
	new_bst_node->children[RIGHT] = (bst_node_t*)right;
	
	return new_bst_node;
}

bst_t* BSTCreate(int(*cmp)(const void* data, const void* param))
{
	bst_node_t* dummy_node = NULL;
	bst_t* new_bst = NULL;
	
	assert (NULL != cmp);
	
	new_bst = (bst_t*)malloc(sizeof(bst_t));
	if (NULL == new_bst)
	{
		return NULL;
	}
	
	dummy_node = CreateBSTNode(NULL, NULL, NULL, NULL);
	if (NULL == dummy_node)
	{
		free(new_bst); new_bst = NULL;
		return NULL;
	}
	
	new_bst->end = dummy_node;
	new_bst->cmp = cmp;
	
	return new_bst;
}

void BSTDestroy(bst_t* bst)
{
	bst_node_t* current = NULL;
	bst_node_t* tmp = NULL;
	
	assert (NULL != bst);
	
	if (BSTIsEmpty(bst))
	{
		current = bst->end;
	}
	else
	{
		current = bst->end->children[LEFT];
	}
	
	while (current != bst->end)
	{
		if (NULL != current->children[LEFT])
		{
			tmp = current->children[LEFT];
			current->children[LEFT] = NULL;
			current = tmp;
		}
		else if (NULL != current->children[RIGHT])
		{
			tmp = current->children[RIGHT];
			current->children[RIGHT] = NULL;
			current = tmp;
		}
		else
		{
			tmp = current->parent;
			free(current); current = NULL;
			current = tmp;
		}
	}
	
	free(bst->end);
	free(bst);
}

static bst_iter_t ConvertBSTNodeToIter(bst_node_t* node)
{
	return ((bst_iter_t)node);
}

static bst_node_t* ConvertIterToBSTNode(bst_iter_t iter)
{
	return ((bst_node_t*)iter);
}

bst_node_t* BSTFindPlace(bst_node_t** parent, 
						 int* side, bst_t* bst, void* data)
{
	int result = 0;
	bst_node_t* current = NULL;
	bst_node_t* prev = NULL;
	
	assert (NULL != bst);
	
	current = bst->end->children[LEFT];
	*parent = bst->end;
	*side = LEFT;
	while (NULL != current)
	{
		result = bst->cmp(current->data, data);
		prev = current;
		if (FOUND_ELEMNT(result))
		{	
			return current;
		}
		else if (RIGHT_BIGGER(result))
		{
			*parent = prev;
			*side = RIGHT;
			current = current->children[RIGHT];
		}
		else
		{	
			*parent = prev;
			*side = LEFT;
			current = current->children[LEFT];
		}
	}
	
	return bst->end;
}

bst_iter_t BSTFind(const bst_t* bst, const void* data)
{
	bst_node_t* parent = NULL;
	bst_node_t* current = NULL;
	int side = 0;
	
	UNUSED(parent);
	UNUSED(current);
	UNUSED(side);
	
	return ConvertBSTNodeToIter(BSTFindPlace(&parent, &side, (bst_t*)bst, (void*)data));
}

bst_iter_t BSTInsert(bst_t* bst, const void* data)
{
	bst_node_t* parent = NULL;
	bst_node_t* current = NULL;
	int side = 0;
	
	assert (NULL != bst);
	
	if (BSTIsEmpty(bst))
	{
		bst->end->children[LEFT] = CreateBSTNode(data, bst->end, NULL, NULL);
		if (NULL == bst->end->children[LEFT])
		{
			return BSTEnd(bst);
		}
		
		return ConvertBSTNodeToIter(bst->end->children[LEFT]);
	}
	else
	{
		BSTFindPlace(&parent, &side, bst, (void*)data);
		parent->children[side] = CreateBSTNode(data, parent, NULL, NULL);
		if (NULL == parent->children[side])
		{
			return BSTEnd(bst);
		}
		
		return ConvertBSTNodeToIter(current);
	}
}

bst_iter_t BSTEnd(const bst_t* bst)
{
	assert (NULL != bst);

	return ConvertBSTNodeToIter(bst->end);
}

void* BSTGetData(const bst_iter_t iter)
{
	bst_node_t* node = ConvertIterToBSTNode(iter);
	
	assert (NULL != node);
	
	return (node->data);
}

int BSTIsEmpty(const bst_t* bst)
{
	assert (NULL != bst);
	
	return (bst->end->children[LEFT] == NULL);
}

static bst_node_t* GetMostLeft(bst_node_t* current)
{
	assert (NULL != current);
	
	while (NULL != current->children[LEFT])
	{
		current = current->children[LEFT];
	}
	
	return current;
}

bst_iter_t BSTBegin(const bst_t* bst)
{	
	assert (NULL != bst);
	
	if (NULL != bst->end->children[LEFT])
	{
		return ConvertBSTNodeToIter(GetMostLeft(bst->end));	
	}
	else if (NULL != bst->end->children[RIGHT])
	{
		return ConvertBSTNodeToIter(GetMostLeft(bst->end->children[RIGHT]));	
	}
	
	return ConvertBSTNodeToIter(bst->end);
}

int BSTIsIterEqual(const bst_iter_t iter1, const bst_iter_t iter2)
{
	bst_node_t* node1 = ConvertIterToBSTNode(iter1);
	bst_node_t* node2 = ConvertIterToBSTNode(iter2);
	
	assert (NULL != node1);
	assert (NULL != node2);
	
	return (node1->data == node2->data);
}

static bst_node_t* GetMostRight(bst_node_t* current)
{
	assert (NULL != current);
	
	while (NULL != current->children[RIGHT])
	{
		current = current->children[RIGHT];
	}
	
	return current;
}

static int IsBSTNodeEquals(bst_node_t* left, bst_node_t* right)
{	
	return (left == right);
}

static bst_node_t* GetPrev(bst_node_t* current)
{
	assert (NULL != current);
	
	if (NULL != current->children[LEFT])
	{
		current = current->children[LEFT];
		return GetMostRight(current);
	}
	else
	{
		while (IsBSTNodeEquals(current, current->parent->children[LEFT]))
		{
			current = current->parent;
		}
	}
	
	return current->parent;
}

bst_iter_t BSTGetPrev(bst_iter_t iter)
{
	return ConvertBSTNodeToIter(GetPrev(ConvertIterToBSTNode(iter)));
}

static bst_node_t* GetNext(bst_node_t* current)
{
	assert (NULL != current);
	
	if (NULL != current->children[RIGHT])
	{
		current = current->children[RIGHT];
		return GetMostLeft(current);
	}
	else
	{
		while (IsBSTNodeEquals(current, current->parent->children[RIGHT]))
		{
			current = current->parent;
		}
	}
	
	return current->parent;
}

bst_iter_t BSTGetNext(bst_iter_t iter)
{
	return ConvertBSTNodeToIter(GetNext(ConvertIterToBSTNode(iter)));
}


static int IsLeaf(bst_node_t* node)
{
	assert (NULL != node);
	
	return (NULL == node->children[LEFT] && NULL == node->children[RIGHT]);
}

static int WhatSideChildAmI(bst_node_t* current)
{
	assert (NULL != current);	
	
	return (IsBSTNodeEquals(current->parent->children[RIGHT],current) * RIGHT
		  +IsBSTNodeEquals(current->parent->children[LEFT],current) * LEFT);
}

static void HelperRemoveForLeafOrOneChild(bst_node_t* current)
{
	int side = 0;
	int child_side = 0;
	
	assert (NULL != current);
	
	side = WhatSideChildAmI(current);
	if (IsLeaf(current))
	{
		current->parent->children[side] = NULL;
		free(current); current = NULL;
	}
	else
	{
		child_side = (NULL == current->children[LEFT] ? RIGHT : LEFT); 
		current->parent->children[side] = current->children[child_side];
		current->children[child_side]->parent = current->parent;
		free(current); current = NULL;
	}
}

bst_iter_t BSTRemove(bst_iter_t iter)
{
	bst_node_t* current = ConvertIterToBSTNode(iter);
	bst_iter_t next = NULL;
	
	assert (NULL != current);
	assert (NULL != current->parent);
	
	next = ConvertBSTNodeToIter(GetNext(current));
	if (NULL == current->children[LEFT] || NULL == current->children[RIGHT])
	{
		HelperRemoveForLeafOrOneChild(current);
		return next;
	}
	
	current->data = (ConvertIterToBSTNode(next))->data;
	HelperRemoveForLeafOrOneChild(ConvertIterToBSTNode(next));
	
	return ConvertBSTNodeToIter(current);
}

static int counting(void* data, void* param)
{
	UNUSED(data);
	++(*(size_t*)param);
	
	return 0;
}

size_t BSTCount(const bst_t* bst)
{
	size_t i = 0;	
	
	assert (NULL != bst);
	
	BSTForEach(BSTBegin(bst), BSTEnd(bst), counting, &i);
	
	return i;
}

int BSTForEach(bst_iter_t from, bst_iter_t to, int(*action)(void* data, void* param), void* param)
{
	bst_node_t* from_node = ConvertIterToBSTNode(from);
	bst_node_t* from_to = ConvertIterToBSTNode(to);
	int status = 0;
	
	assert (NULL != action);
	assert (NULL != from_node);
	assert (NULL != from_to);
	
	for (;!IsBSTNodeEquals(from_node, from_to); from_node = GetNext(from_node))
	{
		status = action(from_node->data, param);
		if (SUCCESS != status)
		{
			return status;
		}
	}
	
	return SUCCESS;
}

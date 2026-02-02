#include "bst.h"

#define FOUND_ELEMNT(num) ((num) == 0)
#define LEFT_BIGGER(num) ((num) < 0) 
#define RIGHT_BIGGER(num) ((num) > 0) 

typedef struct bst_node
{
    void* data;
    bst_node_t* parent;
    bst_node_t* children[NUM_CHILDREN];
} bst_node_t;

typedef struct bst
{
    bst_node_t* end;
    int (*cmp)(const void* data, const void* param);
}

static bst_node_t* CreateBSTNode(void* data, bst_node_t* parent,
	   bst_node_t* left, bst_node_t* right)
{
	bst_node_t* new_bst_node = (bst_node_t*)malloc(sizeof(bst_node_t));
	if (NULL == new_bst_node)
	{
		return NULL;
	}
	
	new_bst_node->data = data;
	new_bst_node->parent = parent;
	new_bst_node->children[LEFT] = left;
	new_bst_node->children[RIGHT] = right;
	
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
	if (NULL == new_bst_node)
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
	bst_node_t* temp = NULL;
	
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

static bst_iter_t ConvertBSTNodeToIter(bst_node* node)
{
	return ((bst_iter_t)node);
}

static bst_node* ConvertIterToBSTNode(bst_iter_t iter)
{
	return ((bst_node*)iter);
}


bst_node* BSTFindPlace(bst_node** parent, int* side, bst_t* bst, void* data)
{
	int i = 0;
	int result = 0;
	bst_node* current = NULL;
	bst_node* prev = NULL;
	
	assert (NULL != bst);
	
	current = bst->end->children[LEFT];
	while (NULL != current)
	{
		result = bst->cmp(data, current->data);
		prev = current;
		if (FOUND_ELEMNT(result))
		{
			*parent = current->parent;
			*side = LEFT_BIGGER(result);
			
			return current;
		}
		else if (RIGHT_BIGGER(result))
		{
			current = current->children[RIGHT];
		}
		else
		{
			current = current->children[LEFT];
		}
	}
	
	*parent = prev;
	*side = LEFT_BIGGER(result);
}

#define LEFT_BIGGER(num) ((num) > 0) 

bst_iter_t BSTInsert(bst_t* bst, const void* data)
{
	bst_node* parent = NULL;
	bst_node* current = NULL;
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
		BSTFindPlace(&parent, &side, bst, data);
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

	return bst->end;
}

















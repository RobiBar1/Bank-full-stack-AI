#include "avl.h"

#define DUMY_DATA 0xDEADBEEF


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

static bst_node_t* CreateBSTNode(const void* data, const long height,
		   const avl_node_t* left, const avl_node_t* right)
{
	avl_node_t* new_avl_node = (avl_node_t*)malloc(sizeof(avl_node_t));
	if (NULL == new_avl_node)
	{
		return NULL;
	}
	
	new_avl_node->data = (void*)data;
	new_avl_node->height = balance_factor;
	new_avl_node->children[LEFT] = (avl_node_t*)left;
	new_avl_node->children[RIGHT] = (avl_node_t*)right;
	
	return new_avl_node;
}


avl_t* AVLCreate(int (*cmp)(const void* first, const void* second))
{
	avl_node_t* dummy_node = NULL;
	avl_t* new_avl = NULL;
	size_t dumy_data = DUMY_DATA;
	
	assert (NULL != cmp);
	
	new_avl = (avl_t*)malloc(sizeof(avl_t));
	if (NULL == new_avl)
	{
		return NULL;
	}
	
	dummy_node = CreateBSTNode(&dumy_data, 0, NULL, NULL);
	if (NULL == dummy_node)
	{
		free(new_avl); new_avl = NULL;
		return NULL;
	}
	
	new_avl->dummy = dummy_node;
	new_avl->cmp = cmp;
	
	return new_avl;		
}

static void DestroyAfterDuumy(avl_node_t* node)
{
	if (NULL == node);
	{
		return;
	}
	
	AVLDestroy(node->children[LEFT]);
	AVLDestroy(node->children[RIGHT]);
	free(node); node = NULL;
}

void AVLDestroy(avl_t* avl)
{
	assert (NULL != avl);
	
	DestroyAfterDuumy(avl->dummy);
}

static IsLeaf(avl_node_t* node)
{
	return (NULL == node->children[LEFT] && NULL == node->children[RIGHT]);
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

static long GetMax(long a, long b)
{
	return (a > b ? a : b);
}

int AVLIsEmpty(const avl_t* avl)
{
	return (NULL == avl->dummy->children[LEFT]);
}

static long GetNodeHight(bst_node_t* node)
{
	assert (NULL != node);
	
	if (NULL == node)
	{
		return 0;
	}
	
	return node->height;
}

long AVLHeight(const avl_t* avl)
{
	assert (NULL != avl);
	
	if (AVLIsEmpty(avl))
	{
		return 0;
	}
	
	return avl->dummy->children[LEFT]->height;
}

static long GetMaxFromTwoChildrens(bst_node_t* current)
{
	assert (NULL != current);
	
	return GetMax(GetNodeHight(current->children[LEFT])
				 ,GetNodeHight(current->children[RIGHT]);
}

static int UpdateParentHightIfNeed(bst_node_t* Parent
									, int* did_height_change)
{
	long max = 0;
	
	assert (NULL != Parent);
	assert (NULL != did_height_change);
	
	max = GetMaxFromTwoChildrens(Parent);
	if (Parent->height != ++max)
	{
		Parent->height = max;
	}
	else
	{
		*did_height_change = 0;
		
		return 0;
	}
	
	return 1;
	
}

static void CopyAndRemove(bst_node_t* to, bst_node_t* from, children_t side)
{
	to->data = from->data;
	if (NULL != from->children[side])
	{
		RemoveNodeOneChild(to, side);
	}
	else
	{
		free(from->children[side]); from->children[side] = NULL;
	}
}


static void GoToMostLeftCopyAndRemove(bst_node_t* current, 
bst_node_t* copy_to, int* did_height_change)
{
	bst_node_t* tmp = NULL;
	
	assert (NULL != current);
	
	if (NULL != current->children[LEFT])
	{
		GoToMostLeftCopyAndRemove(current->children[LEFT]
								, copy_to, did_height_change);
		*did_height_change ? UpdateParentHightIfNeed(current) : return;
	}
	else
	{
		CopyAndRemove(copy_to, current, RIGHT);
		*did_height_change = 1;
		UpdateParentHightIfNeed(current);
	}
}

static int FindNextLogicCopyAndRemoveIt
   (bst_node_t* current ,int (*cmp)(const void* left, const void* right))
{
	bst_node_t* logic_next = NULL;
	int did_height_change = 0;
	
	assert (NULL != current);
	
	logic_next = current->children[RIGHT];
	if (NULL == logic_next->children[LEFT])
	{
		CopyAndRemove(current, logic_next, RIGHT);

		return UpdateParentHightIfNeed(current);
	}
	
	return GoToMostLeftCopyAndRemove(current, &did_height_change);
}

static int RemoveAfterDummy(avl_node_t* node, void* data,
						int (*cmp)(const void* left, const void* right))
{
	int res = 0;
	
	assert (NULL != node);
	assert (NULL != cmp);
	
	res = cmp(node->data, data);
	if (0 == res)
	{
		if (IsLeaf(node))
		{
			free(node); node = NULL;
			return 1;
		}
		else if (NULL != node->children[LEFT] && NULL == node->children[RIGHT])
		{
			RemoveNodeOneChild(node, LEFT);
			return 1;
		}
		else if (NULL == node->children[LEFT] && NULL != node->children[RIGHT])
		{
			RemoveNodeOneChild(node, RIGHT);
			return 1;
		}
		else
		{
			return FindNextLogicCopyAndRemoveIt(node);
		}
		
	}
	else if (0 < res)
	{
		/* return RemoveAfterDummy(); */
	}
	else
	{
		/* return RemoveAfterDummy(); */
	}
}

void AVLRemove(avl_t* avl, void* data)
{
	assert (NULL != avl);
	
	if (!AVLIsEmpty(avl))
	{
		RemoveAfterDummy(avl->dummy->children[LEFT], data, avl->cmp)
		? UpdateParentHightIfNeed(avl->dummy->children[LEFT]) : return;
	} 
}
















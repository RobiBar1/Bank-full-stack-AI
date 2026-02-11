#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

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

static avl_node_t* CreateAVLNode(const void* data, const long height,
		   const avl_node_t* left, const avl_node_t* right)
{
	avl_node_t* new_avl_node = (avl_node_t*)malloc(sizeof(avl_node_t));
	if (NULL == new_avl_node)
	{
		return NULL;
	}
	
	new_avl_node->data = (void*)data;
	new_avl_node->height = height;
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
	
	dummy_node = CreateAVLNode(&dumy_data, 0, NULL, NULL);
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
	if (NULL == node)
	{
		return;
	}
	
	DestroyAfterDuumy(node->children[LEFT]);
	DestroyAfterDuumy(node->children[RIGHT]);
	free(node); node = NULL;
}

void AVLDestroy(avl_t* avl)
{
	assert (NULL != avl);
	
	DestroyAfterDuumy(avl->dummy);
}

static int IsLeaf(avl_node_t* node)
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

static long GetNodeHight(avl_node_t* node)
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

static long GetMaxFromTwoChildrens(avl_node_t* current)
{
	assert (NULL != current);
	
	return GetMax(GetNodeHight(current->children[LEFT])
				 ,GetNodeHight(current->children[RIGHT]));
}

static void UpdateParentHightIfNeed(avl_node_t* parent
									, int* did_height_change)
{
	long max = 0;
	
	assert (NULL != parent);
	assert (NULL != did_height_change);
	
	max = GetMaxFromTwoChildrens(parent);
	if (parent->height != ++max)
	{
		parent->height = max;
	}
	else
	{
		*did_height_change = 0;
	}
	
}

static void CopyAndRemove(avl_node_t* to, avl_node_t* from, children_t side)
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


static void GoToMostLeftCopyAndRemove(avl_node_t* current, 
avl_node_t* copy_to, int* did_height_change)
{
	assert (NULL != current);
	
	if (NULL != current->children[LEFT])
	{
		GoToMostLeftCopyAndRemove(current->children[LEFT]
								, copy_to, did_height_change);
		if (!*did_height_change)
		{
			return;
		}
		
		UpdateParentHightIfNeed(current, did_height_change); 
	}
	else
	{
		CopyAndRemove(copy_to, current, RIGHT);
		*did_height_change = 1;
		UpdateParentHightIfNeed(current, did_height_change);
	}
}

static int FindNextLogicCopyAndRemoveIt(avl_node_t* current)
{
	avl_node_t* logic_next = NULL;
	int did_height_change = 0;
	
	assert (NULL != current);
	
	logic_next = current->children[RIGHT];
	if (NULL == logic_next->children[LEFT])
	{
		CopyAndRemove(current, logic_next, RIGHT);
		UpdateParentHightIfNeed(current, &did_height_change);
		
		return did_height_change;
	}
	
	GoToMostLeftCopyAndRemove(logic_next, current, &did_height_change);
	
	return did_height_change;
}

static void DeleteNode(avl_node_t* node)
{
		if (IsLeaf(node))
		{
			free(node); node = NULL;
		}
		else if (NULL != node->children[LEFT] &&
				 NULL == node->children[RIGHT])
		{
			RemoveNodeOneChild(node, LEFT);
		}
		else if (NULL == node->children[LEFT] && 
				 NULL != node->children[RIGHT])
		{
			RemoveNodeOneChild(node, RIGHT);
		}
		else
		{
			FindNextLogicCopyAndRemoveIt(node);
		}
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
		DeleteNode(node);
		
		return 1;
	}
	else if (0 < res)
	{
		/* return RemoveAfterDummy(); */
	}
	else
	{
		/* return RemoveAfterDummy(); */  
	}
	
	return 0;
}

void AVLRemove(avl_t* avl, const void* data);
{
	int did_height_change = 0;
	
	assert (NULL != avl);
	assert (!AVLIsEmpty(avl));
	
	if (RemoveAfterDummy(avl->dummy->children[LEFT], (void*)data, avl->cmp) &&
	UpdateParentHightIfNeed(
				avl->dummy->children[LEFT], &did_height_change),
				int (*cmp)(const void* left, const void* right))
	{
		/*CheckBalanceAndRotate()*/
	} 
}

static int Insert(avl_node_t* node, const void* data, avl_node_t* new_node,
int (*cmp)(const void* left, const void* right))
{
	int res = 0;
	int did_height_change = 0;
	children_t side = LEFT;
	
	assert (NULL != node);
	assert (NULL != cmp);
	
	res = cmp(node->data, data);
	side = (res < 0) * LEFT + (res > 0) * RIGHT;
	
	if (NULL == node->children[side])
	{
		node->children[side] = new_node;
		UpdateParentHightIfNeed(node->children[side], &did_height_change))
		if (did_height_change)
		{
			/*CheckBalanceAndRotate()*/
		}
		
		return 0;
	}
	
	res = Insert(node->children[side], data, new_node);
	UpdateParentHightIfNeed(node->children[side], &did_height_change))
	if (did_height_change)
	{
		/*CheckBalanceAndRotate()*/
	}
	
	return 0;
}

int AVLInsert(avl_t* avl, const void* data)
{
	avl_node_t* node = NULL;
	
	assert (NULL != avl);
	
	node = CreateAVLNode(data, 0, NULL, NULL);
	if (NULL == node->children[side])
	{
		return 1;
	}
	
	if (AVLIsEmpty(avl))
	{
		avl->dummy->children[LEFT] = node;
	}
	else
	{
		Insert(avl->dummy->children[LEFT], data, node, avl->cmp);
	}
	
	return 0;
}

Find(avl_node_t* node, void* data, 
int (*cmp)(const void* left, const void* right))
{
	int res = 0;
	int did_height_change = 0;
	children_t side = LEFT;
	
	assert (NULL != cmp);
	
	if (NULL == node)
	{
		return NULL;
	}
	
	res = cmp(node->data, data);
	
	if (0 == res)
	{
		return node->data;
	}
	
	side = (res < 0) * LEFT + (res > 0) * RIGHT;
	
	return Find(node->children[side], data, cmp);
}

void* AVLFind(const avl_t* avl, const void* data)
{
	if (AVLIsEmpty(avl))
	{
		return NULL;
	}
	else
	{
		return Find(avl->dummy->children[LEFT], data, avl->cmp);
	}
}


int main()
{
	
	return 0;
}

/*
Writer:  Robi
Checker: Chen
Date:    11.04.2026
Time Class have to solve: 6 hours(10:30 - 16:30).
Time took me to solve: 5:50H.

*/


/*
1.
psodo:
	prev, next = NULL.
	loop while cur != NULL:
		next = node.next
		node.next = prev
		prev = node
		node = next.
	return prev.
*/

Node* FlipIter(Node* node)
{
	Node* prev = NULL;
	Node* next = NULL;
	
	while(NULL != node)
	{
		next = node->next;
		node->next = prev;
		prev = node;
		node = next;
	}
	
	return prev;
}

Node* FlipRecursive(Node* node)
{
	Node* root = NULL;
	Node* prev = NULL;
	
	if (NULL == node)
	{
		return NULL;
	}
	
	root = FlipRecursive(node->next);
	if (NULL == root)
	{
		return node;
	}
	
	prev = node->next;
	if (NULL != prev)
	{
		prev->next = node;
		node->next = NULL;
	}
	
	return root;
}



/*
[-2, 1, -3, 4, -1, 2, 1, -5, 4, -1, -1, -1, 4, 1, 1]
2. 
psodo:
	loop from i = 0 to end of arr and do:
	
		tmp_max += arr[i]
		
		if tmp_max > max do:
			max = tmp_max
			end_index = i
			start_index = tmp_start_index
		
		if tmp_max < 0 do:
			tmp_start_index = i + 1
			tmp_max = 0
	
	return {max, end_index, start_index}
*/

typedef struct return_args
{
	size_t start;
	size_t end;
	int max_to_return;
}return_args_t;

static void* UpdateNewMax(return_args_t* args, int new_max, size_t start, size_t end)
{
	assert (NULL != args);
	
	args->start = start;
	args->end = end;
	args->max_to_return = new_max;	
	
	return NULL;
}

static void OpenNewSlideWindow(size_t* tmp_start, int* tmp_max, size_t new_start)
{
	assert (NULL != tmp_start);
	assert (NULL != tmp_max);
	
	*tmp_start = new_start;
	*tmp_max = 0;
}

void FindMaxContiguousSubArr(int* arr, size_t size, return_args_t* args)
{
	size_t tmp_start = 0;
	int tmp_max = 0;
	size_t i = 0;
	
	assert (NULL != arr);
	assert (NULL != args);
	assert (0 < size);
	
	args->max_to_return = 0;
	for(; i < size; ++i)
	{
		tmp_max += arr[i];
		args->max_to_return < tmp_max ? 
							  UpdateNewMax(args, tmp_max, tmp_start, i) : NULL ;
		if (0 > tmp_max)
		{
			OpenNewSlideWindow(&tmp_start, &tmp_max, i + 1);
		} 
	}
}


/*
3.
psodo:
	SortStack(Stack* s)
	helper = StackCreate()
	max = StackPop(s)
	stack_size = 0
	tmp = 0
	
	stack_size = CountStack(s)
	
	loop stack_size times and do:
		loop until !IsEmpty(s):
			tmp = StackPop(s)
			if (tmp < max)
				StackPush(helper, tmp)
			else
				StackPush(helper, max)
				max = tmp
*/

static void RemoveStackToOtherStack(Stack* helper,Stack* s)
{
	assert(NULL != helper);
	assert(NULL != s);
	
	while(!IsEmpty(helper))
	{
		StackPush(s, StackPop(helper));
	}
}

void SortStack(Stack* s)
{
	Stack* helper = StackCreate();
	size_t stack_size = 0;
	size_t cur_size = 0;
	int is_swap = 0;
	int max = 0;
	int tmp = 0;
	
	assert (NULL != s);
	
	if (NULL == helper)
	{
		perror("StackCreate() failed");
		
		return;
	}	
	
	stack_size = StackSize(s);
	while (stack_size--)
	{
		max = StackPop(s);
		cur_size = stack_size;
		is_swap = 0;
		while (cur_size--)
		{
			tmp = StackPop(s);
			if (tmp < max)
			{
				StackPush(helper, tmp);
				is_swap = 1;
			}
			else
			{
				StackPush(helper, max);
				max = tmp;
			}
		}
		
		StackPush(s, max);
		RemoveStackToOtherStack(helper, s);
		
		if (!is_swap)
		{
			StackDestroy(helper);
			return;
		}
		
	}
	
	StackDestroy(helper);
	helper = NULL;
}

/*
4.
psodo:
1.	read file line by line and do:
2.		for each char count it in count_arr
3.	overide the file content as follow:
4.		for i = 0 for each arr[i] do:
5.			write that char to file until 0 == arr[i]
*/

#define CHAR_SIZE 256

int SortCharsInFile(const char* file_name, char* result)
{
	size_t* count_arr = NULL;
	FILE* f = NULL;
	size_t i = 0;
	int ch = 0;
	
	assert (NULL != file_name);
	assert (NULL != result);
	
	f = fopen(file_name, "r");
	if (NULL == f)
	{
		perror("fopen failed");
		
		return -1;
	}	
	
	count_arr = (size_t*)calloc(1, sizeof(size_t) * CHAR_SIZE);
	if (NULL == count_arr)
	{
		perror("calloc failed");
		
		return -1;
	}
	
	ch = fgetc(f);
	while (EOF != ch)
	{
		++(count_arr[ch]);
		ch = fgetc(f);
	}
	
	fclose(f);
	f = NULL;
	for (ch = 0; ch < CHAR_SIZE; ++ch)
	{
		for (; 0 != count_arr[ch]; ++i, --(count_arr[ch]))
		{
			result[i] = (char)ch;
		}
	}
	
	result[i] = '\0';
	
	free(count_arr);
	count_arr = NULL;
	
	return 0;
}	

/*
5.
psodo:
	1. find right place for the node.
	2. attach him as a child.
*/

#define RIGHT 1
#define LEFT 0

Node* BSTInsertIter(Node* root, Node* node)
{
	Node* prev = NULL;
	Node* runner = root;
	
	assert(NULL != node);
	
	if(NULL == runner)
	{
		return node;
	}
	
	while (NULL != runner)
	{
		prev = runner;
		if (runner->data <= node->data)
		{
			runner = runner->child[RIGHT];
		}				
		else
		{
			runner = runner->child[LEFT];
		}
	}	
	
	if (prev->data >= node->data)
	{
		prev->child[LEFT] = node;
	}				
	else
	{
		prev->child[RIGHT] = node;
	}
	
	return root;
}

Node* BSTInsertRecursive(Node* root, Node* node)
{	
	assert(NULL != node);
	
	if(NULL == root)
	{
		return node;
	}
	
	if (root->data <= node->data)
	{
		root->child[RIGHT] = BSTInsertRecursive(root->child[RIGHT], node);
	}				
	else
	{
		root->child[LEFT] = BSTInsertRecursive(root->child[LEFT], node);
	}
		
	return root;
}


#define LEFT_CHILD(my_index) ((my_index * 2) + 1)
#define RIGHT_CHILD(my_index) ((my_index + 1) * 2)
#define EMPTY_SLOT INT_MAX

int BSTInsertToFixed(int* arr, size_t size, size_t root_index, int node_data)
{
	int prev_index = 0;
	
	assert(NULL != arr);
	
	while (root_index < size)
	{
		prev_index = root_index;
		if (EMPTY_SLOT == arr[root_index])
		{
			arr[root_index] = node_data;
			
			return 0;
		}
		root_index = arr[root_index] <= node_data ? RIGHT_CHILD(root_index) : 
													LEFT_CHILD(root_index);
	}	
	
	perror("no place for enter the node");
	
	return -1;
}

/*
6.
heuristic is method where for search solution to problem can take to long
so we choose to be ok with answer that is good enough but i can find her very
fast.

the classic examplse is to solve the problem with the travel agent:
agent need to go N city in the most short/fastest way possible, to calculate 
this its NP-Hard problem (Non polinom time to solve it).

so instead we will solve it with heuristic algorithem that for each city will 
go to the nearest city that he still isnt visit there yet.
*/


/*
7.
"hey world" -> "dlrow yeh" . ReverseStringRecursive("hey world", 9, 0).
psodo:
	if n <= 0 return.
	Swap(str[0] str[n-1])
	CallMeAgain(str[1], n - 2);	
*/

void ReverseStringRecursive(char* str, size_t size_not_inculde_null,
						    size_t swaps)
{
	char tmp = '\0';
	
	assert(NULL != str);
	
	if (swaps > size_not_inculde_null / 2)
	{
		return;
	}
	
	tmp = str[0];
	str[0] = str[size_not_inculde_null - swaps - 1];
	str[size_not_inculde_null - swaps - 1] = tmp;
	
	ReverseStringRecursive(++str, size_not_inculde_null, swaps + 1);
}

/*
8.
psodo:

*/

void generate_permutations_helper(const char *original, char *current_string, int *visited, int current_len, int original_len) 
{
    int i = 0;

	assert (NULL != current_string);
	assert (NULL != original);
	assert (NULL != visited);
		
    if (0 < current_len) 
    {
        current_string[current_len] = '\0';
        printf("%s\n", current_string);
    }

    if (current_len == original_len) 
    {
        return;
    }

    for (; i < original_len; i++) 
    {
        
        if (!visited[i]) 
        {
            visited[i] = 1;
            current_string[current_len] = original[i];

            generate_permutations_helper(original, current_string, visited, current_len + 1, original_len);

            visited[i] = 0;
        }
    }
}

void print_all_substring_permutations(const char *str) 
{
    char *current_string = NULL;
    int *visited = NULL;
    int len = 0;
	
	assert(NULL != str);
	
    len = strlen(str);
    visited = (int*)calloc(len, sizeof(int));
    current_string = (char*)malloc((len + 1) * sizeof(char));
    if (NULL == visited || NULL == current_string) 
    {
        perror("allocation failed\n");
        
        free(visited);
        visited = NULL;
        free(current_string);
        current_string = NULL;
        
        return;
    }

    generate_permutations_helper(str, current_string, visited, 0, len);

    free(visited);
    visited = NULL;
    free(current_string);
    current_string = NULL;
}

/*
8.b
i will store each string i print into hash table and then before each print
ill check if its already there, is yes i wont print and if no ill print and 
insert the new string.
*/

/*
9.
a. In a Trie where each node represent a bit.

b. in each node ill have bit that mark if this path is free of occupid, so for
the first check ill worst case is 2 checks, then im in the tree with 31 bit 
adresses(all that start with 1), now again worst case 2 checks and i will be in
30 bit addressess ... 29 ... 28 ... 2 ... 1 ... O(N) where N is the num of 
bits(32 on this case) but will can call it ~O(1) becuase it fixed very low
number of operetion we do.

c. 2 ways that work as following:
1. i go down the tree to the relevant adress, if see on the way Node that dont
exist yet, ite mean its the first time i go in that adress way -> so this 
adress is first time allocate.

2. i can save in bits a bit that tell me its occuped or not, so if i go down 
the way and see its occiped its mean all the addresses under that is occupied.

d. O(N) where N is the num of bits(32 on this case).
 but will can call it ~O(1) becuase it fixed very low number of operetion we do.
*/


/*
10.
psodo:
	tmp = 0
	
	if StackIsEmpty(s) || new_int < StackPeek(s):
		StackPush(s, new_int)
	
	tmp = StackPop(s)
	RecursiveCall(s)
	
	StackPush(s, tmp)
*/

void StackInsertRecursiveStaySorted(Stack* s, int new_int)
{
	int tmp = 0;
	
	assert(NULL != s);
	
	if(StackIsEmpty(s) || new_int < StackPeek(s))
	{
		StackPush(s, new_int);
		
		return;
	}
	
	tmp = StackPop(s);
	
	StackInsertRecursiveStaySorted(s, new_int);
	
	StackPush(s, tmp);
}

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
	max = 0;
	cur_sum = 0
	i = 0
	sta_window = 0;
	end_window = 0;
	first_positive_index = 0
	minus_sum = 0
	saw_first_minus = 0
	saw_first_positive = 0
	saw_second_minus = 0
	saw_second_positive = 0
	sum_from_positive = 0
	
	loop until arr[i] > 0.
	max = arr[i]
	cur_sum = arr[i]
	sta_window = i
	end_window = i
	
	loop unil the end of arr and do:
		if arr[i] >= 0:
			if saw_first_minus:
				if saw_first_positive
					if saw_second_minus
						if (sum_from_positive >= cur_sum):
							sta_window = saw_first_positive
							sum_from_positive = arr[i]
							first_positive_index = i
							saw_second_minus = 0	
						else
							sta_window = i
							sum_from_positive = 0
							first_positive_index = 0
							saw_first_minus = 0
							saw_first_positive = 0
							saw_second_minus = 0
							cur_sum = arr[i]
					else
						sum_from_positive += arr[i]
						cur_sum += arr[i]	
				else
					++saw_first_positive
					sum_from_positive += arr[i]
					cur_sum += arr[i]
					first_positive_index = 0 ? i : first_positive_index;	
			else
				cur_sum += arr[i]
				
			max + cur_sum > max ? max + cur_sum : max
		else
			++saw_first_minus;
			if saw_first_positive && !saw_second_minus:
				sum_from_positive += arr[i]
				++saw_second_minus
			cur_sum += arr[i]
			
		++end_window
*/


static void InitVariables(int* arr, size_t i, double* max, double* cur_sum,
						  size_t* sta_window, size_t* end_window)
{

	*max = arr[i];
	*cur_sum = arr[i];
	*sta_window = i;
	*end_window = i;
}

size_t FindMaxContiguousSubArr(int* arr, size_t size)
{
	size_t first_positive_index = 0;
	size_t saw_second_positive = 0;
	size_t saw_first_positive = 0;
	double sum_from_positive = 0;
	size_t saw_second_minus = 0;
	size_t saw_first_minus = 0;
	size_t sta_window = 0;
	size_t end_window = 0;
	double minus_sum = 0;
	double cur_sum = 0;
	double max = 0;
	size_t i = 0;
	
	assert (NULL != arr);
	assert (0 < size);
	
	while (arr[i] <= 0)
	{
		++i;
	}
	
	InitVariables(arr, i, &max, &cur_sum, &sta_window, &end_window);
	
	for (++i; i < size; ++i)
	{
		if (arr[i] >= 0)
		{
			if(saw_first_minus)
			{
				if(saw_first_positive)
				{
					if(saw_second_minus)
					{
						if (sum_from_positive >= cur_sum)
						{
							sta_window = saw_first_positive;
							cur_sum = sum_from_positive;
							sum_from_positive = arr[i];
							first_positive_index = i;
							saw_second_minus = 0;
						}
						else
						{
							sta_window = i;
							sum_from_positive = 0;
							first_positive_index = 0;
							saw_first_minus = 0;
							saw_first_positive = 0;
							saw_second_minus = 0;
							cur_sum = arr[i];
						}
					}
					else
					{
						sum_from_positive += arr[i];
						cur_sum += arr[i];
					}
				}
				else
				{
					++saw_first_positive;
					sum_from_positive += arr[i];
					cur_sum += arr[i];
					first_positive_index = 0 ? i : first_positive_index;	
				}
			}
			else
			{
				cur_sum += arr[i];
			}
			
			max + cur_sum > max ? max + cur_sum : max;
		}	
		else
		{
			++saw_first_minus;
			if (saw_first_positive && !saw_second_minus)
			{
				sum_from_positive += arr[i];
				++saw_second_minus;
			}
			
			cur_sum += arr[i];
		}
			
		++end_window;
	}
	
	return max;
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
PrintAllThisStrPermutations(str)
PrintAllThisStrPermutations()

"helo" helo, ehlo, elho, eloh, leoh, loeh, lohe, olhe, ohle, ohel, hoel, heol.
"hey" hey, ehy, eyh, yeh, yhe, hye 
PrintAllThisStrPermutations(str):
	print(str)
	
	loop size - 1 times
		MoveFirstLetter(size - 1);
	
	MoveFirstLetter(size - 2);

*/

static void Swap(char* ch, char* ch1)
{
	char tmp = '\0';
	
	assert(NULL != ch);
	assert(NULL != ch1);
	
	tmp = *ch;
	*ch = *ch1;
	*ch1 = tmp;
}

static void MoveFirstLetter(char* str, size_t str_size)
{
	size_t i = 0;
	
	assert(NULL != str);
	
	for (; i < str_size; ++i)
	{
		Swap(&(str[i]), &(str[i + 1]));
		printf("%s\n", str);	
	}
}
/*
"helwo"
"helo" helo, ehlo, elho, eloh, leoh, loeh, lohe, olhe, ohle, ohel, hoel, heol.
"hey" hey, ehy, eyh, yeh, yhe, hye 
*/
static void PrintAllThisStrPermutations(char* str, size_t str_size)
{
	size_t i = 0;
	
	assert(NULL != str);
	
	printf("%s\n", str);
	for (; i < str_size; ++i)
	{
		MoveFirstLetter(str, str_size);
	}
	
	MoveFirstLetter(str, str_size - 1);
}

void PrintArrAndSubArrAllPermutations(char* str)
{
	size_t str_size = 0;
	
	assert (NULL != str);
		
	str_size = strlen(str);
	PrintAllThisStrPermutations(str, str_size - 1);
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

#include <stddef.h> /* size_t */
/*
struct hash_table
{
    size_t(*hash_func)(const void*  data);
    int (*is_match_func)(const void* data,const void* inner_key);
    size_t num_buckets;
    dlist_t* buckets[1];
}
*/

typedef struct hash_table hash_table_t;

/**
 * @desc creates a new empty hash table
 * @param Hash_Func - a hash func which return the bucket in which the data will be stored
 * @param IsMatchFunc - to locate the data within the bucket, matches according to unique inner-key
 * @pre Hash_Func != NULL
 * @pre IsMatchFunc != NULL
 * @pre num_buckets != 0
 * @returns pointer to the new hash_func tree, or NULL on failure.
 * @complexity O(n)
 */
hash_table_t* HashTableCreate(size_t num_buckets, size_t(*hash_func)(const void* data), int(*is_match_func)(const void* data,const void* param));

/**
 * @desc - destroys the hash table
 * @param[in] table - the hash table to destroy
 * @pre - table != NULL
 * @complexity O(n)
 **/
void HashTableDestroy(hash_table_t* table);

/**
 * @desc - remove data from the table
 * @param[in] table - the hash table to remove to
 * @param[in] data - the data we remove from the table
 * @pre - table != NULL
 * @complexity O(1)
 **/
void HashTableRemove(hash_table_t* table, const void* data);

/**
 * @desc - inserts the new data into the table
 * @param[in] table - the hash table to insert to
 * @param[in] data - the data we insert into the table
 * @pre - table != NULL
 * @complexity O(1)
 **/
int HashTableInsert(hash_table_t* table, const void* data);

/**
 * @desc finds data in the hash table and caches last found value for faster access
 * @param[in] table - the hash table
 * @param[in] data - data to find in the hash table
 * @pre table != NULL
 * 
 * @return the data found of data is in the table, NULL otherwise
 * @complexity: O(1) when Load Factor and SD are optimal
 */
void* HashTableFind(hash_table_t* table, const void* data);

/**
 * @desc applies an action over the hash elements
 * @param[in] table - the hash table
 * @param[in] action - a function which executes a function on the hash elements.
 * The action() is not allowed to change the keys of the hash elements
 * action() should return 0 upon success or a non-zero value upon failure
 * @param[in] param - what action() will use to apply onto the hash elements
 * @pre table != NULL
 * @pre action != NULL
 * @return 0 - all actions on the hash elements were successful, non-zero otherwise
 * @complexity: O(n)
 */
int HashTableForEach(hash_table_t* table, int (*action_func)(void* data, void* param), void* param);

/**
 * @desc - counts the number of values in the table
 * @param[in] table - the hash table
 * @pre - table != NULL
 * @complexity O(n)
 **/
size_t HashTableCount(const hash_table_t* table);

/**
 * @desc - checks if the table is empty
 * @param[in] table - the hash table
 * @pre - table != NULL
 * @return - 1 if empty and 0 otherwise
 * @complexity O(num of buckets)
 **/
int HashTableIsEmpty(const hash_table_t* table);

/*
 * @desc Get the load factor of the hash table.
 * @param[in] table - the has hash table to get the load factor of.
 * @pre table != NULL
 * @return double - between 0 and 1 signifying the LF of the hash table, range around 0.6 to 0.75 considered good. 
 * @complexity: O(n)
*/
double HashTableLoadFactor(const hash_table_t* table);
/**
 * @desc Get the standard deviation of the hash table
 * @param[in] table - the has table to get the standard deviation of
 * @pre table != NULL
 * @return double - between 0 and 1 signifying the SD of the table. 
 * @complexity: O(n)
 **/
double HashTableSD(const hash_table_t* table);

/* NOTES */
/**
 * the load factor is a measure of how full the table is, defined as the ratio of the total number of stored entries to the total number of available buckets
 * It indicates the average number of entries per bucket and acts as a key indicator of performance.
 * the default load factor (0.75) offers a good tradeoff between time and space costs.
 **/
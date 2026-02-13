/*
Writer:  Robi
Checker: Hadar
Date:    13.02.2026
*/

#include <stdlib.h>      /* size_t */
#include <assert.h>      /* assert  */
#include <math.h>        /* pow     */

#include "hash_table.h"  /* our api */
#include "d_link_list.h" /* dlist_t */

#define UNUSED(x) (void)(x)

struct hash_table
{
    size_t(*hash_func)(const void*  data);
    int (*is_match_func)(const void* data,const void* inner_key);
    size_t num_buckets;
    dlist_t* buckets[1];
};

static void DestroyAllBuckets(hash_table_t* h_t, size_t num_to_create)
{
    size_t i = 0;

    assert (NULL != h_t);

    for (; i < num_to_create; ++i)
    {
        DListDestroy(h_t->buckets[i]);
        h_t->buckets[i] = NULL;
    }
}


hash_table_t* HashTableCreate(size_t num_buckets, size_t(*hash_func)(const void* data), 
                                int(*is_match_func)(const void* data,const void* param))
{
    hash_table_t* h_t = NULL;
    size_t i = 0;

    assert (NULL != hash_func);
    assert (NULL != is_match_func);

    h_t = (hash_table_t*)malloc(sizeof(hash_table_t) + sizeof(dlist_t*) * (num_buckets - 1));
    if (NULL == h_t)
    {
        return NULL;
    } 

    for (; i < num_buckets; ++i)
    {
        h_t->buckets[i] = DListCreate();
        if (NULL == h_t->buckets[i])
        {
            DestroyAllBuckets(h_t, i);
            free(h_t); h_t = NULL;
            return NULL;
        } 
    }
    
    h_t->num_buckets = num_buckets;
    h_t->hash_func = hash_func;
    h_t->is_match_func = is_match_func;

    return h_t;
}

void HashTableDestroy(hash_table_t* table)
{    
    assert (NULL != table);

    DestroyAllBuckets(table, table->num_buckets);
    free(table); table = NULL;
}

static dlist_t* GetBucket(hash_table_t* table, const void* data)
{
    assert (NULL != table);

    return table->buckets[table->hash_func(data)];
}

void HashTableRemove(hash_table_t* table, const void* data)
{
    dlist_t* link_list = NULL;  
    dlist_iter_t iter_to_remove = {0};
    
    assert (NULL != table);

    link_list = GetBucket(table, data);
    iter_to_remove = DListFind(DListBegin(link_list), DListEnd(link_list), data, table->is_match_func);
    if (!DListIsIterEqual(iter_to_remove, DListEnd(link_list)))
    {
        DListRemove(iter_to_remove);
    }
}

int HashTableInsert(hash_table_t* table, const void* data)
{
    assert (NULL != table);

    return DListPushFront(GetBucket(table, data), data);
}

void* HashTableFind(hash_table_t* table, const void* data)
{
    dlist_t* link_list = NULL;  
    dlist_iter_t iter_to_remove = {0};
    void* data_to_return = NULL;

    assert (NULL != table);

    link_list = GetBucket(table, data);
    iter_to_remove = DListFind(DListBegin(link_list), DListEnd(link_list), data, table->is_match_func);
    if (!DListIsIterEqual(iter_to_remove, DListEnd(link_list)))
    {
        data_to_return = DListGetData(iter_to_remove);
        if(!DListPushFront(link_list, data_to_return))
        {
            DListRemove(iter_to_remove);
        }

        return data_to_return;
    }

    return NULL;
}

int HashTableForEach(hash_table_t* table, int (*action_func)(void* data, void* param), void* param)
{   
    size_t i = 0;
    dlist_t* link_list = NULL;  
    int status = 0;

    assert (NULL != table);
    assert (NULL != action_func);

    for (; i < table->num_buckets; ++i)
    {
        link_list = table->buckets[i];
        status = DListForEach(DListBegin(link_list), DListEnd(link_list), action_func, param);
        if (status)
        {
            return status;             
        }
    }

    return 0;
}

static int Count(void* data, void* param)
{
    UNUSED(data);

    ++(*((size_t*)param));

    return 0;
}

size_t HashTableCount(const hash_table_t* table)
{
     size_t count = 0;

     assert (NULL != table);

     HashTableForEach((hash_table_t*)table, Count, &count);

     return count;
}

int HashTableIsEmpty(const hash_table_t* table)
{
    assert (NULL != table);

    return (0 == HashTableCount(table));
}

double HashTableLoadFactor(const hash_table_t* table)
{
    assert (NULL != table);

    return ((double)HashTableCount(table) / table->num_buckets);
}

double HashTableSD(const hash_table_t* table)
{
    double mean = 0;
    double sum = 0;
    size_t i = 0;

    assert (NULL != table);

    mean = HashTableLoadFactor(table);
    for (; i < table->num_buckets; ++i)
    {
        sum += pow((DListCount(table->buckets[i]) - mean), 2);
    }
    
    return pow((sum / table->num_buckets), 0.5);
}
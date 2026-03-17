#ifndef __TRIE_H__
#define __TRIE_H__

#include <stddef.h> /* size_t */

typedef enum { SUCCESS, TAKEN, ALLOC_FAIL, NOT_EXIST } trie_status_t;

typedef struct trie trie_t;

/**
 * @brief Creates a trie
 * @param[in] depth - the depth of the trie to create
 * @pre depth > 0
 * @returns trie_t or NULL upon failure
 *
 * complexity: O(1)
 */
trie_t *TrieCreate(size_t depth);
/**
 * @brief destroys trie
 * @param[in] trie - the trie to destroy
 * @pre trie != NULL
 *
 * complexity: O(n)
 */
void TrieDestroy(trie_t *trie);

/**
 * @brief inserts a new word
 * @param[in] trie - the trie to insert the word to
 * @param[in] word - the word to insert
 * @param[in] out_word - will be the next word if this is taken
 * @pre trie is not NULL
 * @return status of insert
 *
 * complexity: O(len(word))
 * Ubdefined Behavior: len(word) > word_size
 */
trie_status_t TrieInsert(trie_t *trie, const unsigned char *word);

/**
 * @brief checks if trie contain word
 * @param[in] trie - the trie to find the word.
 * @param[in] word - the word to check
 * @pre trie is not NULL
 * @return 1 if found otherwise 0
 *
 * complexity: O(len(word))
 */
int TrieFind(trie_t *trie, const unsigned char *word);

/**
 * @brief gets the next word in the order
 * @param[in] trie - the trie we are working on to get the next word
 * @param[in] word - the word we get the next from
 * @param[in] out_word
 * @pre trie is not NULL
 * @pre NULL != word
 *
 * @return status if success
 * @complexity: O(len(word))
 */
trie_status_t TrieGetFreeNext(trie_t *trie, const unsigned char *word,
                              unsigned char *out_word);

/**
 * @brief sets the last node's char entry (for last letter in word) from
 * 0xCAFEBABE to NULL (implementation detail)
 * @param[in] trie - the trie to set to node in
 * @param[in] word - the word
 * @pre trie is not NULL
 *
 * complexity: O(len(word))
 * Undefined behavior: set alloc twice or word not found
 */

void TrieFree(trie_t *trie, const unsigned char *word);

/**
 * @brief counts the number of occupied words that have this prefix
 * @param[in] trie - the trie to count from
 * @param[in] prefix - the prefix which is checked
 * @pre trie is not NULL
 * @return number of occupied words that contains prefix
 *
 * complexity: O(?)
 */
size_t TrieCount(const trie_t *trie);

int TrieIsEmpty(const trie_t *trie);

#endif /* __TRIE_H__ */
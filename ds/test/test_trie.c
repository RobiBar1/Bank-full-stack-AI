#include <stdio.h>
#include <string.h>

#include "trie.h"

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"
#define PASS (1)
#define FAIL (0)
#define UNUSED(x) ((void)(x))
#define RUN_TEST(suite, test_fn)                                               \
    do                                                                         \
    {                                                                          \
        test_result_t result = {0};                                            \
        (suite).total++;                                                       \
                                                                               \
        if (test_fn(&result) == PASS)                                          \
        {                                                                      \
            (suite).passed++;                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            printf(COLOR_RED "[FAIL] %s: %s\n" COLOR_RESET, #test_fn,          \
                   result.message);                                            \
        }                                                                      \
    } while (0)

typedef struct
{
    size_t total;
    size_t passed;
} test_suite_t;

typedef struct
{
    char message[256];
} test_result_t;

static void PrintTestSummary(test_suite_t* suite_ptr)
{
    if (suite_ptr->passed == suite_ptr->total)
    {
        printf(COLOR_GREEN "PASS: ");
    }
    else
    {
        printf(COLOR_RED "FAIL: ");
    }
    printf("%lu/%lu passed\n" COLOR_RESET, suite_ptr->passed, suite_ptr->total);
}

static int CheckTrieSize(trie_t* trie, size_t expected_size,
                         test_result_t* result)
{
    size_t size = TrieCount(trie);

    if (size != expected_size)
    {
        sprintf(result->message,
                COLOR_RED
                "trie is of Wrong Size! Got %lu, expected %lu" COLOR_RESET,
                size, expected_size);
        return FAIL;
    }

    return PASS;
}

static int CheckValues(int res, int expected, test_result_t* result)
{
    if (res != expected)
    {
        sprintf(result->message,
                COLOR_RED "res is different! Got %d, expected %d", res,
                expected);
        return FAIL;
    }

    return PASS;
}

static int CheckBuffers(char* buffer, char* expected_buffer, size_t size,
                        test_result_t* result)
{
    size_t i = 0;

    for (i = 0; i < size; ++i)
    {
        if (strncmp(buffer, expected_buffer, size))
        {
            sprintf(result->message,
                    COLOR_RED "res is different! Got %s, expected %s", buffer,
                    expected_buffer);
            return FAIL;
        }
    }
    return PASS;
}

static int TestTrieCreation(test_result_t* result)
{
    trie_t* trie = TrieCreate(10);
    int status = PASS;

    status = CheckTrieSize(trie, 0, result);

    TrieDestroy(trie);

    return status;
}

static void RunTestTrieCreation()
{
    test_suite_t suite = {0};

    printf(COLOR_BLUE "Run tests for trie creation\n" COLOR_RESET);

    RUN_TEST(suite, TestTrieCreation);

    PrintTestSummary(&suite);
    printf(COLOR_BLUE "Finished tests\n");
}

static int TestTrieInsertOneElement(test_result_t* result)
{
    trie_t* trie = TrieCreate(8);
    unsigned char* words[] = {(unsigned char*)"a"};
    size_t expected_size = 1;
    int status = PASS;
    size_t i = 0;

    for (i = 0; i < expected_size; ++i)
    {
        TrieInsert(trie, words[i]);
    }

    status = CheckTrieSize(trie, expected_size, result);

    TrieDestroy(trie);

    return status;
}

static int TestTrieInsertMultiElement(test_result_t* result)
{
    trie_t* trie = TrieCreate(8);
    unsigned char* words[] = {(unsigned char*)"a", (unsigned char*)"b",
                              (unsigned char*)"c", (unsigned char*)"d"};
    size_t expected_size = 4;
    int status = PASS;
    size_t i = 0;

    for (i = 0; i < expected_size; ++i)
    {
        TrieInsert(trie, words[i]);
    }

    status = CheckTrieSize(trie, expected_size, result);

    TrieDestroy(trie);

    return status;
}

static int TestTrieInsertElementExists(test_result_t* result)
{
    trie_t* trie = TrieCreate(9);
    unsigned char* words[] = {(unsigned char*)"aa", (unsigned char*)"aa",
                              (unsigned char*)"ca"};
    size_t expected_size = 2;
    int status = PASS;
    size_t i = 0;

    for (i = 0; i < expected_size + 1; ++i)
    {
        TrieInsert(trie, words[i]);
    }

    status = CheckTrieSize(trie, expected_size, result);

    TrieDestroy(trie);

    return status;
}

static void RunTestTrieInsert()
{
    test_suite_t suite = {0};

    printf(COLOR_BLUE "Run tests for trie insert\n" COLOR_RESET);

    RUN_TEST(suite, TestTrieInsertOneElement);
    RUN_TEST(suite, TestTrieInsertMultiElement);
    RUN_TEST(suite, TestTrieInsertElementExists);

    PrintTestSummary(&suite);
    printf(COLOR_BLUE "Finished tests\n");
}

static int TestTrieFindOneElement(test_result_t* result)
{
    trie_t* trie = TrieCreate(8);
    unsigned char* words[] = {(unsigned char*)"a"};
    size_t expected_size = 1;
    int res = 0;
    int expected_res = 1;
    int status = PASS;
    size_t i = 0;

    for (i = 0; i < expected_size; ++i)
    {
        TrieInsert(trie, words[i]);
    }

    res = TrieFind(trie, (unsigned char*)"a");

    status = CheckValues(res, expected_res, result);

    TrieDestroy(trie);

    return status;
}

static int TestTrieFindMultiElement(test_result_t* result)
{
    trie_t* trie = TrieCreate(8);
    unsigned char* words[] = {(unsigned char*)"a", (unsigned char*)"b",
                              (unsigned char*)"c", (unsigned char*)"d"};
    size_t expected_size = 4;
    int res = 0;
    int expected_res = 1;
    int status = PASS;
    size_t i = 0;

    for (i = 0; i < expected_size; ++i)
    {
        TrieInsert(trie, words[i]);
    }

    res = TrieFind(trie, (unsigned char*)"c");

    status = CheckValues(res, expected_res, result);

    TrieDestroy(trie);

    return status;
}

static int TestTrieFindNotFound(test_result_t* result)
{
    trie_t* trie = TrieCreate(8);
    unsigned char* words[] = {(unsigned char*)"a", (unsigned char*)"b",
                              (unsigned char*)"c", (unsigned char*)"d"};
    size_t expected_size = 4;
    int res = 0;
    int expected_res = 0;
    int status = PASS;
    size_t i = 0;

    for (i = 0; i < expected_size; ++i)
    {
        TrieInsert(trie, words[i]);
    }

    res = TrieFind(trie, (unsigned char*)"e");

    status = CheckValues(res, expected_res, result);

    TrieDestroy(trie);

    return status;
}

static void RunTestTrieFind()
{
    test_suite_t suite = {0};

    printf(COLOR_BLUE "Run tests for trie find\n" COLOR_RESET);

    RUN_TEST(suite, TestTrieFindOneElement);
    RUN_TEST(suite, TestTrieFindMultiElement);
    RUN_TEST(suite, TestTrieFindNotFound);

    PrintTestSummary(&suite);
    printf(COLOR_BLUE "Finished tests\n");
}

static int TestTrieFreeOneElement(test_result_t* result)
{
    trie_t* trie = TrieCreate(8);
    unsigned char* words[] = {(unsigned char*)"a", (unsigned char*)"b",
                              (unsigned char*)"c", (unsigned char*)"d"};
    size_t expected_size = 3;
    int res = 0;
    int expected_res = 0;
    int status = PASS;
    size_t i = 0;

    for (i = 0; i < expected_size + 1; ++i)
    {
        TrieInsert(trie, words[i]);
    }

    TrieFree(trie, (unsigned char*)"a");
    res = TrieFind(trie, (unsigned char*)"a");

    status = CheckValues(res, expected_res, result) &&
             CheckTrieSize(trie, expected_size, result);

    TrieDestroy(trie);

    return status;
}

static int TestTrieFreeAll(test_result_t* result)
{
    trie_t* trie = TrieCreate(8);
    unsigned char* words[] = {(unsigned char*)"a", (unsigned char*)"b",
                              (unsigned char*)"c", (unsigned char*)"d"};
    size_t expected_size = 0;
    int status = PASS;
    size_t i = 0;

    for (i = 0; i < expected_size + 4; ++i)
    {
        TrieInsert(trie, words[i]);
    }

    TrieFree(trie, (unsigned char*)"a");
    TrieFree(trie, (unsigned char*)"b");
    TrieFree(trie, (unsigned char*)"c");
    TrieFree(trie, (unsigned char*)"d");

    status = CheckTrieSize(trie, expected_size, result);

    TrieDestroy(trie);

    return status;
}

static void RunTestTrieFree()
{
    test_suite_t suite = {0};

    printf(COLOR_BLUE "Run tests for trie free\n" COLOR_RESET);

    RUN_TEST(suite, TestTrieFreeOneElement);
    RUN_TEST(suite, TestTrieFreeAll);

    PrintTestSummary(&suite);
    printf(COLOR_BLUE "Finished tests\n");
}

static int TestTrieGetNext(test_result_t* result)
{
    trie_t* trie = TrieCreate(8);
    unsigned char* words[] = {(unsigned char*)"a"};
    size_t expected_size = 1;
    int res = SUCCESS;
    int expected_res = SUCCESS;
    unsigned char buffer1[1] = {'a'};
    unsigned char buffer[1] = {'\0'};
    char expected_buffer[1] = {'b'};
    int status = PASS;
    size_t i = 0;

    for (i = 0; i < expected_size; ++i)
    {
        TrieInsert(trie, words[i]);
    }

    res = GetNextFree(trie, buffer1, buffer);

    status = CheckValues(res, expected_res, result) &&
             CheckBuffers((char*)buffer, (char*)expected_buffer, 1, result);

    TrieDestroy(trie);

    return status;
}

static void RunTestTrieGetNext()
{
    test_suite_t suite = {0};

    printf(COLOR_BLUE "Run tests for trie GetNext\n" COLOR_RESET);

    RUN_TEST(suite, TestTrieGetNext);

    PrintTestSummary(&suite);
    printf(COLOR_BLUE "Finished tests\n");
}

static void RunTests()
{
    RunTestTrieCreation();
    RunTestTrieInsert();
    RunTestTrieFind();
    RunTestTrieFree();
    RunTestTrieGetNext();
}

int main()
{
    RunTests();

    return 0;
}

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>

namespace test
{

/* Runners */
int tests_run = 0;
int tests_failed = 0;

#define PASSED_ALL_EMOJI "\xf0\x9f\x92\xaf"
#define PASSED_TEST_EMOJI "\xf0\x9f\x92\xaa"

#define TEST_START(name)                                                       \
    printf("Running %s... ", name);                                            \
    tests_run++;
#define TEST_PASS() printf("\033[0;32mPASSED\033[0m %s\n", PASSED_TEST_EMOJI)
#define TEST_RUN(test_func)                                                    \
    do                                                                         \
    {                                                                          \
        SetUp();                                                               \
        test_func();                                                           \
        TearDown();                                                            \
    } while (0)

#define TEST_RUN_SETUP_INPUT(test_func, setup_input)                           \
    do                                                                         \
    {                                                                          \
        SetUp(setup_input);                                                    \
        test_func();                                                           \
        TearDown();                                                            \
    } while (0)

#define TEST_FAIL(msg)                                                         \
    {                                                                          \
        printf("\033[0;31mFAILED\033[0m\n  Line %d: %s\n", __LINE__, msg);     \
        tests_failed++;                                                        \
        return;                                                                \
    }

/* The "Assertion" macros */
#define TEST_ASSERT_TRUE(cond)                                                 \
    if (!(cond))                                                               \
    TEST_FAIL("Condition was FALSE")
#define TEST_ASSERT_FALSE(cond)                                                \
    if ((cond))                                                                \
    TEST_FAIL("Condition was TRUE")

#define TEST_ASSERT_EQUAL_INT(exp, act)                                        \
    if ((exp) != (act))                                                        \
    {                                                                          \
        char msg[100];                                                         \
        sprintf(msg, "Expected %d, but got %d", (int)exp, (int)act);           \
        TEST_FAIL(msg);                                                        \
    }

#define DABS(a) (((a) >= (0)) ? (a) : (-a))

#define TEST_ASSERT_EQUAL_DOUBLE(exp, act)                                     \
    do                                                                         \
    {                                                                          \
        double diff = exp - act;                                               \
        if (DABS(diff) > 0.00001)                                              \
        {                                                                      \
            char msg[100];                                                     \
            sprintf(msg, "Expected %f, but got %f", (double)exp, (double)act); \
            TEST_FAIL(msg);                                                    \
        }                                                                      \
    } while (0);

#define TEST_ASSERT_NOT_NULL(ptr)                                              \
    if ((ptr) == NULL)                                                         \
    TEST_FAIL("Pointer was NULL")

#define TEST_ASSERT_EQUAL_PTR(exp, act)                                        \
    if ((exp) != (act))                                                        \
    {                                                                          \
        char msg[100];                                                         \
        sprintf(msg, "Expected pointer %p, but got %p", (void*)exp,            \
                (void*)act);                                                   \
        TEST_FAIL(msg);                                                        \
    }

#define TEST_ASSERT_EQUAL_STR(exp, act)                                        \
    if (0 != strcmp(exp, act))                                                 \
    {                                                                          \
        char msg[100];                                                         \
        sprintf(msg, "Expected string %s, but got %s", exp, act);              \
        TEST_FAIL(msg);                                                        \
    }

#define TEST_REPORT()                                                          \
    do                                                                         \
    {                                                                          \
        printf("\n------------------------------\n");                          \
        if (tests_failed == 0)                                                 \
        {                                                                      \
            printf("\033[0;32mALL %d TESTS PASSED\033[0m %s\n", tests_run,     \
                   PASSED_ALL_EMOJI);                                          \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            printf("\033[0;31mTESTS FAILED: %d/%d\033[0m\n", tests_failed,     \
                   tests_run);                                                 \
        }                                                                      \
    } while (0)

template <typename T> inline bool IsEqual(T exp, T act) { return (exp == act); }

inline bool IsEqual(double exp, double act)
{
    const double epsilon = 0.00001;

    return (DABS(exp - act) < epsilon);
}

template <typename T> inline void FailMsg(T exp, T act, char* msg)
{
    std::ostringstream out;
    out << "Expected " << exp << ", but got " << act;
    strncpy(msg, out.str().c_str(), 99);
    msg[99] = '\0';
}

#define TEST_ASSERT_EQUAL(exp, act)                                            \
    if (!IsEqual(exp, act))                                                    \
    {                                                                          \
        char msg[100];                                                         \
        FailMsg(exp, act, msg);                                                \
        TEST_FAIL(msg);                                                        \
    }
} // namespace test

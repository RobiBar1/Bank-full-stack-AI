#include <cstring> // strcmp
#include <iostream>
#include <string>

#include "string.hpp"

// gpp98d ./test_string.cpp ../src/String.cpp  -I ../include/

// using namespace ilrd;

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";

typedef enum
{
    TEST_STATUS_SUCCESS,
    TEST_STATUS_FAILURE
} test_status_t;

static test_status_t StringCreate_SmokeTest(void);
static test_status_t StringCreate_Basics(void);
static test_status_t StringAssignment_DiffElemsTest(void);
static test_status_t StringAssignment_SameElemsTest(void);
static test_status_t StringCopyCtor_Basic(void);
static test_status_t StringOperators_Basic(void);
static test_status_t StringOstream_Basic(void);
static test_status_t FooFunctionality(void);
static test_status_t BlaFunctionality(void);

static void CheckTest(test_status_t result, std::string name)
{
    if (result == TEST_STATUS_SUCCESS)
    {
        std::cout << "-> " << name << ": " << GREEN << "PASSED\n\n" << RESET;
    }
    else
    {
        std::cout << "-> " << name << ": " << RED << "FAILED\n\n" << RESET;
    }
}

int main()
{
    std::cout << "\n ---- TESTS ----\n\n";

    CheckTest(StringCreate_SmokeTest(), "StringCreate_SmokeTest");
    CheckTest(StringCreate_Basics(), "StringCreate_Basics");
    CheckTest(StringAssignment_DiffElemsTest(),
              "StringAssignment_DiffElemsTest");
    CheckTest(StringAssignment_SameElemsTest(),
              "StringAssignment_SameElemsTest");
    CheckTest(StringCopyCtor_Basic(), "StringCopyCtor_Basic");
    CheckTest(StringOperators_Basic(), "StringOperators_Basic");

    StringOstream_Basic();
    std::cout << "\n";

    CheckTest(FooFunctionality(), "FooFunctionality");
    CheckTest(BlaFunctionality(), "BlaFunctionality");

    return 0;
}

static test_status_t StringCreate_SmokeTest(void)
{
    ilrd::String my_str("hello");

    return TEST_STATUS_SUCCESS;
}

static test_status_t StringCreate_Basics(void)
{
    test_status_t res = TEST_STATUS_SUCCESS;
    ilrd::String my_str("hello");

    if (0 != strcmp("hello", my_str.Cstr()))
    {
        res = TEST_STATUS_FAILURE;
    }

    if (5 != my_str.Length())
    {
        res = TEST_STATUS_FAILURE;
    }

    return res;
}

static test_status_t StringAssignment_DiffElemsTest(void)
{
    test_status_t res = TEST_STATUS_SUCCESS;
    ilrd::String my_str1("hello");
    ilrd::String my_str2("mars");

    my_str1 = my_str2;

    if (0 != strcmp("mars", my_str1.Cstr()))
    {
        res = TEST_STATUS_FAILURE;
    }

    if (0 != strcmp("mars", my_str2.Cstr()))
    {
        res = TEST_STATUS_FAILURE;
    }

    return res;
}

static test_status_t StringAssignment_SameElemsTest(void)
{
    test_status_t res = TEST_STATUS_SUCCESS;
    ilrd::String my_str1("hello");

    my_str1 = my_str1;

    if (0 != strcmp("hello", my_str1.Cstr()))
    {
        res = TEST_STATUS_FAILURE;
    }

    return res;
}

static test_status_t StringCopyCtor_Basic(void)
{
    test_status_t res = TEST_STATUS_SUCCESS;
    ilrd::String my_str1("hello");
    ilrd::String my_str2(my_str1);

    if (0 != strcmp("hello", my_str2.Cstr()))
    {
        res = TEST_STATUS_FAILURE;
    }

    return res;
}

static test_status_t StringOperators_Basic(void)
{
    test_status_t res = TEST_STATUS_SUCCESS;
    ilrd::String my_str1("hello");
    ilrd::String my_str2("mars");

    if (my_str1 > my_str2)
    {
        res = TEST_STATUS_FAILURE;
    }

    if (my_str2 < my_str1)
    {
        res = TEST_STATUS_FAILURE;
    }

    if (my_str2 == my_str1)
    {
        res = TEST_STATUS_FAILURE;
    }

    my_str1 = "neptune";

    if (my_str1 < my_str2)
    {
        res = TEST_STATUS_FAILURE;
    }

    if (my_str2 > my_str1)
    {
        res = TEST_STATUS_FAILURE;
    }

    if (my_str2 == my_str1)
    {
        res = TEST_STATUS_FAILURE;
    }

    my_str1 = "mars";

    if (!(my_str1 == my_str2))
    {
        res = TEST_STATUS_FAILURE;
    }

    return res;
}

static test_status_t StringOstream_Basic(void)
{
    test_status_t res = TEST_STATUS_SUCCESS;
    ilrd::String my_str1("hello");

    std::cout << "Check that prints hello inside \"\": \"" << my_str1 << "\"\n";

    return res;
}

static int Foo(ilrd::String str) { return ("mars" == str); }

static test_status_t FooFunctionality(void)
{
    test_status_t res = TEST_STATUS_SUCCESS;
    ilrd::String my_str("mars");

    if (1 != Foo(my_str))
    {
        res = TEST_STATUS_FAILURE;
    }

    return res;
}

static ilrd::String Bla() { return "mars"; }

static test_status_t BlaFunctionality(void)
{
    test_status_t res = TEST_STATUS_SUCCESS;
    ilrd::String my_str = Bla();

    if (0 != strcmp("mars", my_str.Cstr()))
    {
        res = TEST_STATUS_FAILURE;
    }

    return res;
}
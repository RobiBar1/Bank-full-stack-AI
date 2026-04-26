#include <cstring> // strcmp
#include <iostream>
#include <limits> // inf
#include <string>

#include "complex.hpp"
#include <complex>

// gpp98d ./test_complex.cpp ../src/Complex.cpp  -I ../include/

// using namespace ilrd;

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";

typedef enum
{
    TEST_STATUS_SUCCESS,
    TEST_STATUS_FAILURE
} test_status_t;

static test_status_t ComplexCreate_SmokeTest(void);
static test_status_t Complex_AddTest(void);
static test_status_t Complex_SubTest(void);
static test_status_t Complex_MultTest(void);
static test_status_t Complex_DivTest_Simple(void);
static test_status_t Complex_Equality_Simple(void);
static test_status_t Complex_Arithmetic_Test(void);
static test_status_t Complex_DivTest_Zero(void);
static void Complex_Cin_Test(void);

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

    CheckTest(ComplexCreate_SmokeTest(), "ComplexCreate_SmokeTest");
    CheckTest(Complex_AddTest(), "Complex_AddTest");
    CheckTest(Complex_SubTest(), "Complex_SubTest");
    CheckTest(Complex_MultTest(), "Complex_MultTest");
    CheckTest(Complex_DivTest_Simple(), "Complex_DivTest_Simple");
    CheckTest(Complex_Equality_Simple(), "Complex_Equality_Simple");
    CheckTest(Complex_Arithmetic_Test(), "Complex_Arithmetic_Test");
    CheckTest(Complex_DivTest_Zero(), "Complex_DivTest_Zero");

    Complex_Cin_Test();

    return 0;
}

static int DoublesAreEqual(double d1, double d2)
{
    double epsilon = 0.1e-9;

    if (std::numeric_limits<double>::infinity() == d1 &&
        std::numeric_limits<double>::infinity() == d2)
    {
        return 1;
    }

    return abs(d1 - d2) < epsilon;
}

static int IsMatchComplex(const ilrd::Complex& c1,
                          const std::complex<double>& c2)
{
    return DoublesAreEqual(c1.GetReal(), real(c2)) &&
           DoublesAreEqual(c1.GetImg(), imag(c2));
}

static test_status_t ComplexCreate_SmokeTest(void)
{
    ilrd::Complex c(2, 5);
    ilrd::Complex c2(2);
    ilrd::Complex c3;

    return TEST_STATUS_SUCCESS;
}

static test_status_t Complex_AddTest(void)
{
    test_status_t status = TEST_STATUS_SUCCESS;
    ilrd::Complex c1(2, 5);
    ilrd::Complex c2(8, 10);
    std::complex<double> cc1(2, 5);
    std::complex<double> cc2(8, 10);

    c1 += c2;
    cc1 += cc2;

    if (!IsMatchComplex(c1, cc1))
    {
        status = TEST_STATUS_FAILURE;
    }

    return status;
}

static test_status_t Complex_SubTest(void)
{
    test_status_t status = TEST_STATUS_SUCCESS;
    ilrd::Complex c1(2, 5);
    ilrd::Complex c2(8, 10);
    std::complex<double> cc1(2, 5);
    std::complex<double> cc2(8, 10);

    c1 -= c2;
    cc1 -= cc2;

    if (!IsMatchComplex(c1, cc1))
    {
        status = TEST_STATUS_FAILURE;
    }

    return status;
}

static test_status_t Complex_MultTest(void)
{
    test_status_t status = TEST_STATUS_SUCCESS;
    ilrd::Complex c1(2, 5);
    ilrd::Complex c2(8, 10);
    std::complex<double> cc1(2, 5);
    std::complex<double> cc2(8, 10);

    c1 *= c2;
    cc1 *= cc2;

    if (!IsMatchComplex(c1, cc1))
    {
        status = TEST_STATUS_FAILURE;
    }

    return status;
}

static test_status_t Complex_DivTest_Simple(void)
{
    test_status_t status = TEST_STATUS_SUCCESS;
    ilrd::Complex c1(2, 5);
    ilrd::Complex c2(8, 10);
    std::complex<double> cc1(2, 5);
    std::complex<double> cc2(8, 10);

    c1 /= c2;
    cc1 /= cc2;

    if (!IsMatchComplex(c1, cc1))
    {
        status = TEST_STATUS_FAILURE;
    }

    return status;
}

static test_status_t Complex_Equality_Simple(void)
{
    test_status_t status = TEST_STATUS_SUCCESS;
    ilrd::Complex c1(2, 5);
    ilrd::Complex c2(8, 10);
    ilrd::Complex c3(10, 15);

    c1 += c2;

    if (c1 != c3)
    {
        status = TEST_STATUS_FAILURE;
    }

    if (c1 != c1)
    {
        status = TEST_STATUS_FAILURE;
    }

    if (c2 == c3)
    {
        status = TEST_STATUS_FAILURE;
    }

    return status;
}

static test_status_t Complex_Arithmetic_Test(void)
{
    test_status_t status = TEST_STATUS_SUCCESS;
    ilrd::Complex c1(2, 5);
    ilrd::Complex c2(8, 10);
    std::complex<double> cc1(2, 5);
    std::complex<double> cc2(8, 10);

    if (!IsMatchComplex(c1 + c2, cc1 + cc2))
    {
        status = TEST_STATUS_FAILURE;
    }

    if (!IsMatchComplex(c1 - c2, cc1 - cc2))
    {
        status = TEST_STATUS_FAILURE;
    }

    if (!IsMatchComplex(c1 * c2, cc1 * cc2))
    {
        status = TEST_STATUS_FAILURE;
    }

    if (!IsMatchComplex(c1 / c2, cc1 / cc2))
    {
        status = TEST_STATUS_FAILURE;
    }

    return status;
}

static test_status_t Complex_DivTest_Zero(void)
{
    test_status_t status = TEST_STATUS_SUCCESS;
    ilrd::Complex c1(2, 5);
    ilrd::Complex c2(0, 0);
    std::complex<double> cc1(2, 5);
    std::complex<double> cc2(0, 0);

    c1 /= c2;
    cc1 /= cc2;

    if (!IsMatchComplex(c1, cc1))
    {
        status = TEST_STATUS_FAILURE;
    }

    return status;
}

static void Complex_Cin_Test(void)
{
    ilrd::Complex c1(2, 5);
    // std::complex<double> c1(2, 5);

    std::cout << "Write: \"(10 , 30  )\"\n";

    std::cin >> c1;

    std::cout << c1 << "\n";
}
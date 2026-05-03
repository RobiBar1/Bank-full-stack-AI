#include <iostream>
#include <sstream>
#include <string>

#include "rcstring.hpp"

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"

class TestSuite
{
  public:
    TestSuite() : total(0), passed(0) {}

    void AddResult(bool pass)
    {
        ++total;
        if (pass)
            ++passed;
    }

    void PrintSummary() const
    {
        if (passed == total)
            std::cout << COLOR_GREEN << "PASS: ";
        else
            std::cout << COLOR_RED << "FAIL: ";

        std::cout << passed << "/" << total << " passed\n" << COLOR_RESET;
    }

  private:
    size_t total;
    size_t passed;
};

template <typename T>
bool CheckValues(const T& res, const T& expected, std::stringstream& result)
{
    if (res != expected)
    {
        result << COLOR_RED << "values are different! Got " << res
               << ", expected " << expected << std::endl;
        return false;
    }

    return true;
}

template <typename Func>
void RunTest(TestSuite& suite, const char* name, Func test_fn)
{
    std::stringstream result;
    bool pass = test_fn(result);

    suite.AddResult(pass);

    if (!pass)
    {
        std::cout << COLOR_RED << "[FAIL] " << name << ": " << result.str()
                  << COLOR_RESET;
    }
}

bool TestRCStringDefaultConstructor(std::stringstream& result)
{
    ilrd::RCString str;
    std::string expected = "";

    bool status = CheckValues<std::string>(str.Cstr(), expected, result);

    return status;
}

bool TestRCStringRegularConstructor(std::stringstream& result)
{
    ilrd::RCString str("Hi");
    std::string expected = "Hi";

    bool status = CheckValues<std::string>(str.Cstr(), expected, result);

    return status;
}

bool TestRCStringCopyConstructor(std::stringstream& result)
{
    ilrd::RCString str(ilrd::RCString("Hi"));
    std::string expected = "Hi";

    bool status = CheckValues<std::string>(str.Cstr(), expected, result);

    return status;
}

bool TestRCStringConstructorInvalidArgument(std::stringstream& result)
{
    bool status = false;

    try
    {
        ilrd::RCString str(NULL);

        result << "Expected exception, but none was thrown" << std::endl;
    }
    catch (const std::invalid_argument&)
    {
        status = true;
    }
    catch (...)
    {
        result << "Wrong exception type thrown" << std::endl;
    }

    return status;
}

bool TestRCStringOperatorInput(std::stringstream& result)
{
    std::string input = "Hello";
    std::istringstream iss(input);
    ilrd::RCString expected("Hello");
    ilrd::RCString res;

    iss >> res;

    return CheckValues(res, expected, result);
}

void RunTestsRCStringConstructors()
{
    TestSuite suite;

    std::cout << COLOR_BLUE "Run test for RCString constructors\n" COLOR_RESET;

    RunTest(suite, "TestRCStringDefaultConstructor",
            TestRCStringDefaultConstructor);
    RunTest(suite, "TestRCStringRegularConstructor",
            TestRCStringRegularConstructor);
    RunTest(suite, "TestRCStringCopyConstructor", TestRCStringCopyConstructor);
    RunTest(suite, "TestRCStringConstructorInvalidArgument",
            TestRCStringConstructorInvalidArgument);
    RunTest(suite, "TestRCStringOperatorInput", TestRCStringOperatorInput);

    suite.PrintSummary();
    std::cout << COLOR_BLUE "Finished tests\n" COLOR_RESET;
}

bool TestRCStringLengthEmpty(std::stringstream& result)
{
    ilrd::RCString str;
    size_t expected = 0;

    size_t res = str.Length();

    bool status = CheckValues<size_t>(res, expected, result);

    return status;
}

bool TestRCStringLengthNotEmpty(std::stringstream& result)
{
    ilrd::RCString str("Hello");
    size_t expected = 5;

    size_t res = str.Length();

    bool status = CheckValues<size_t>(res, expected, result);

    return status;
}

void RunTestsRCStringLength()
{
    TestSuite suite;

    std::cout << COLOR_BLUE "Run test for String Length\n" COLOR_RESET;

    RunTest(suite, "TestRCStringLengthEmpty", TestRCStringLengthEmpty);
    RunTest(suite, "TestRCStringLengthNotEmpty", TestRCStringLengthNotEmpty);

    suite.PrintSummary();
    std::cout << COLOR_BLUE "Finished tests\n" COLOR_RESET;
}

bool TestRCStringComparisonBiggerFirstBigger(std::stringstream& result)
{
    ilrd::RCString str1("aab");
    ilrd::RCString str2("aaa");
    bool expected = true;

    bool res = str1 > str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonBiggerFirstSmaller(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("aab");
    bool expected = false;

    bool res = str1 > str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonBiggerBothEquals(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("aaa");
    bool expected = false;

    bool res = str1 > str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonBiggerFirstEmpty(std::stringstream& result)
{
    ilrd::RCString str1("");
    ilrd::RCString str2("aaa");
    bool expected = false;

    bool res = str1 > str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonBiggerSecondEmpty(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("");
    bool expected = true;

    bool res = str1 > str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonBiggerBothEmpty(std::stringstream& result)
{
    ilrd::RCString str1("");
    ilrd::RCString str2("");
    bool expected = false;

    bool res = str1 > str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

void RunTestsRCStringComparisonBigger()
{
    TestSuite suite;

    std::cout << COLOR_BLUE "Run test for ComparisonBigger\n" COLOR_RESET;

    RunTest(suite, "TestRCStringComparisonBiggerFirstBigger",
            TestRCStringComparisonBiggerFirstBigger);
    RunTest(suite, "TestRCStringComparisonBiggerFirstSmaller",
            TestRCStringComparisonBiggerFirstSmaller);
    RunTest(suite, "TestRCStringComparisonBiggerBothEquals",
            TestRCStringComparisonBiggerBothEquals);
    RunTest(suite, "TestRCStringComparisonBiggerFirstEmpty",
            TestRCStringComparisonBiggerFirstEmpty);
    RunTest(suite, "TestRCStringComparisonBiggerSecondEmpty",
            TestRCStringComparisonBiggerSecondEmpty);
    RunTest(suite, "TestRCStringComparisonBiggerBothEmpty",
            TestRCStringComparisonBiggerBothEmpty);

    suite.PrintSummary();
    std::cout << COLOR_BLUE "Finished tests\n" COLOR_RESET;
}

bool TestRCStringComparisonSmallerFirstBigger(std::stringstream& result)
{
    ilrd::RCString str1("aab");
    ilrd::RCString str2("aaa");
    bool expected = false;

    bool res = str1 < str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonSmallerFirstSmaller(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("aab");
    bool expected = true;

    bool res = str1 < str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonSmallerBothEquals(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("aaa");
    bool expected = false;

    bool res = str1 > str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonSmallerFirstEmpty(std::stringstream& result)
{
    ilrd::RCString str1("");
    ilrd::RCString str2("aaa");
    bool expected = true;

    bool res = str1 < str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonSmallerSecondEmpty(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("");
    bool expected = false;

    bool res = str1 < str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonSmallerBothEmpty(std::stringstream& result)
{
    ilrd::RCString str1("");
    ilrd::RCString str2("");
    bool expected = false;

    bool res = str1 < str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

void RunTestsRCStringComparisonSmaller()
{
    TestSuite suite;

    std::cout << COLOR_BLUE
        "Run test for RCString ComparisonSmaller\n" COLOR_RESET;

    RunTest(suite, "TestRCStringComparisonSmallerFirstBigger",
            TestRCStringComparisonSmallerFirstBigger);
    RunTest(suite, "TestRCStringComparisonSmallerFirstSmaller",
            TestRCStringComparisonSmallerFirstSmaller);
    RunTest(suite, "TestRCStringComparisonSmallerBothEquals",
            TestRCStringComparisonSmallerBothEquals);
    RunTest(suite, "TestRCStringComparisonSmallerFirstEmpty",
            TestRCStringComparisonSmallerFirstEmpty);
    RunTest(suite, "TestRCStringComparisonSmallerSecondEmpty",
            TestRCStringComparisonSmallerSecondEmpty);
    RunTest(suite, "TestRCStringComparisonSmallerBothEmpty",
            TestRCStringComparisonSmallerBothEmpty);

    suite.PrintSummary();
    std::cout << COLOR_BLUE "Finished tests\n" COLOR_RESET;
}

bool TestRCStringComparisonEqualsFirstBigger(std::stringstream& result)
{
    ilrd::RCString str1("aab");
    ilrd::RCString str2("aaa");
    bool expected = false;

    bool res = str1 == str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonEqualsFirstSmaller(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("aab");
    bool expected = false;

    bool res = str1 == str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonEqualsBothEquals(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("aaa");
    bool expected = true;

    bool res = str1 == str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonEqualsFirstEmpty(std::stringstream& result)
{
    ilrd::RCString str1("");
    ilrd::RCString str2("aaa");
    bool expected = false;

    bool res = str1 == str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonEqualsSecondEmpty(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("");
    bool expected = false;

    bool res = str1 == str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonEqualsBothEmpty(std::stringstream& result)
{
    ilrd::RCString str1("");
    ilrd::RCString str2("");
    bool expected = true;

    bool res = str1 == str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonEqualsStringLiteral(std::stringstream& result)
{
    ilrd::RCString str("Hi");
    bool expected = true;

    bool res = ("Hi" == str);

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

void RunTestsRCStringComparisonEquals()
{
    TestSuite suite;

    std::cout << COLOR_BLUE
        "Run test for RCString ComparisonEquals\n" COLOR_RESET;

    RunTest(suite, "TestRCStringComparisonEqualsFirstBigger",
            TestRCStringComparisonEqualsFirstBigger);
    RunTest(suite, "TestRCStringComparisonEqualsFirstSmaller",
            TestRCStringComparisonEqualsFirstSmaller);
    RunTest(suite, "TestRCStringComparisonEqualsBothEquals",
            TestRCStringComparisonEqualsBothEquals);
    RunTest(suite, "TestRCStringComparisonEqualsFirstEmpty",
            TestRCStringComparisonEqualsFirstEmpty);
    RunTest(suite, "TestRCStringComparisonEqualsSecondEmpty",
            TestRCStringComparisonEqualsSecondEmpty);
    RunTest(suite, "TestRCStringComparisonEqualsBothEmpty",
            TestRCStringComparisonEqualsBothEmpty);
    RunTest(suite, "TestRCStringComparisonEqualsStringLiteral",
            TestRCStringComparisonEqualsStringLiteral);

    suite.PrintSummary();
    std::cout << COLOR_BLUE "Finished tests\n" COLOR_RESET;
}

bool TestRCStringComparisonNotEqualsFirstBigger(std::stringstream& result)
{
    ilrd::RCString str1("aab");
    ilrd::RCString str2("aaa");
    bool expected = true;

    bool res = str1 != str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonNotEqualsFirstSmaller(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("aab");
    bool expected = true;

    bool res = str1 != str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonNotEqualsBothEquals(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("aaa");
    bool expected = false;

    bool res = str1 != str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonNotEqualsFirstEmpty(std::stringstream& result)
{
    ilrd::RCString str1("");
    ilrd::RCString str2("aaa");
    bool expected = true;

    bool res = str1 != str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonNotEqualsSecondEmpty(std::stringstream& result)
{
    ilrd::RCString str1("aaa");
    ilrd::RCString str2("");
    bool expected = true;

    bool res = str1 != str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonNotEqualsBothEmpty(std::stringstream& result)
{
    ilrd::RCString str1("");
    ilrd::RCString str2("");
    bool expected = false;

    bool res = str1 != str2;

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

bool TestRCStringComparisonNotEqualsStringLiteral(std::stringstream& result)
{
    ilrd::RCString str("Hi");
    bool expected = false;

    bool res = ("Hi" != str);

    bool status = CheckValues<bool>(res, expected, result);

    return status;
}

void RunTestsRCStringNotComparisonEquals()
{
    TestSuite suite;

    std::cout << COLOR_BLUE
        "Run test for RCString ComparisonEquals\n" COLOR_RESET;

    RunTest(suite, "TestRCStringComparisonNotEqualsFirstBigger",
            TestRCStringComparisonNotEqualsFirstBigger);
    RunTest(suite, "TestRCStringComparisonNotEqualsFirstSmaller",
            TestRCStringComparisonNotEqualsFirstSmaller);
    RunTest(suite, "TestRCStringComparisonNotEqualsBothEquals",
            TestRCStringComparisonNotEqualsBothEquals);
    RunTest(suite, "TestRCStringComparisonNotEqualsFirstEmpty",
            TestRCStringComparisonNotEqualsFirstEmpty);
    RunTest(suite, "TestRCStringComparisonNotEqualsSecondEmpty",
            TestRCStringComparisonNotEqualsSecondEmpty);
    RunTest(suite, "TestRCStringComparisonNotEqualsBothEmpty",
            TestRCStringComparisonNotEqualsBothEmpty);
    RunTest(suite, "TestRCStringComparisonNotEqualsStringLiteral",
            TestRCStringComparisonNotEqualsStringLiteral);

    suite.PrintSummary();
    std::cout << COLOR_BLUE "Finished tests\n" COLOR_RESET;
}

bool TestRCStringAssignmentEmpty(std::stringstream& result)
{
    ilrd::RCString str;
    std::string expected = "";

    str = expected.c_str();

    bool status =
        CheckValues<std::string>(str.Cstr(), expected.c_str(), result);

    return status;
}

bool TestRCStringAssignmentNotEmpty(std::stringstream& result)
{
    ilrd::RCString str;
    std::string expected = "Hello";

    str = expected.c_str();

    bool status =
        CheckValues<std::string>(str.Cstr(), expected.c_str(), result);

    return status;
}

bool TestRCStringAssignmentSelf(std::stringstream& result)
{
    ilrd::RCString str("Hello");
    std::string expected = "Hello";

    str = str;

    bool status =
        CheckValues<std::string>(str.Cstr(), expected.c_str(), result);

    return status;
}

void RunTestsRCStringAssignment()
{
    TestSuite suite;

    std::cout << COLOR_BLUE "Run test for RCString assignment\n" COLOR_RESET;

    RunTest(suite, "TestRCStringAssignmentEmpty", TestRCStringAssignmentEmpty);
    RunTest(suite, "TestRCStringAssignmentNotEmpty",
            TestRCStringAssignmentNotEmpty);
    RunTest(suite, "TestRCStringAssignmentSelf", TestRCStringAssignmentSelf);

    suite.PrintSummary();
    std::cout << COLOR_BLUE "Finished tests\n" COLOR_RESET;
}

bool TestRCStringSquareBracketsAccess(std::stringstream& result)
{
    ilrd::RCString str("Hello");
    char expected = 'H';

    char res = str[0];

    bool status = CheckValues(res, expected, result);

    return status;
}

bool TestRCStringSquareBracketsAssignmentChar(std::stringstream& result)
{
    ilrd::RCString str("Hello");
    std::string expected = "Hollo";

    str[1] = 'o';

    bool status =
        CheckValues<std::string>(str.Cstr(), expected.c_str(), result);

    return status;
}

bool TestRCStringSquareBracketsAssignmentRCString(std::stringstream& result)
{
    ilrd::RCString str("Hello");
    ilrd::RCString expected("Hollo");

    str[1] = expected[1];

    bool status = CheckValues<std::string>(str.Cstr(), expected.Cstr(), result);

    return status;
}

bool TestRCStringSquareBracketsOutOfRange(std::stringstream& result)
{
    ilrd::RCString str("Hello");
    bool status = false;
    try
    {
        str[6];

        result << "Expected exception, but none was thrown" << std::endl;
    }
    catch (const std::out_of_range&)
    {
        status = true;
    }
    catch (...)
    {
        result << "Wrong exception type thrown" << std::endl;
    }

    return status;
}

void RunTestsRCStringSquareBrackets()
{
    TestSuite suite;

    std::cout << COLOR_BLUE << "Run test for RCString SquareBrackets"
              << std::endl;

    RunTest(suite, "TestRCStringSquareBracketsAccess",
            TestRCStringSquareBracketsAccess);
    RunTest(suite, "TestRCStringSquareBracketsAssignmentChar",
            TestRCStringSquareBracketsAssignmentChar);
    RunTest(suite, "TestRCStringSquareBracketsAssignmentRCString",
            TestRCStringSquareBracketsAssignmentRCString);
    RunTest(suite, "TestRCStringSquareBracketsOutOfRange",
            TestRCStringSquareBracketsOutOfRange);

    suite.PrintSummary();
    std::cout << COLOR_BLUE "Finished tests\n" COLOR_RESET;
}

bool RunTestsRCStringGetAddressOperatorSameString(std::stringstream& result)
{
    ilrd::RCString str1("Hello");
    ilrd::RCString str2(str1);
    bool expected = true;

    bool res = &str1[0] == &str2[0];

    bool status = CheckValues(res, expected, result);

    return status;
}

bool RunTestsRCStringGetAddressOperatorDifferentStrings(
    std::stringstream& result)
{
    ilrd::RCString str1("Hello");
    ilrd::RCString str2("Hello");
    bool expected = false;

    bool res = &str1[0] == &str2[0];

    bool status = CheckValues(res, expected, result);

    return status;
}

bool RunTestsRCStringGetAddressOperatorAfterAssignment(
    std::stringstream& result)
{
    ilrd::RCString str1("Hello");
    ilrd::RCString str2(str1);
    bool expected = false;

    str2[1] = 'o';

    bool res = &str1[0] == &str2[0];

    bool status = CheckValues(res, expected, result);

    return status;
}

void RunTestsRCStringGetAddressOperator()
{
    TestSuite suite;

    std::cout << COLOR_BLUE << "Run test for RCString GetAddress operator"
              << std::endl;

    RunTest(suite, "RunTestsRCStringGetAddressOperatorSameString",
            RunTestsRCStringGetAddressOperatorSameString);

    RunTest(suite, "RunTestsRCStringGetAddressOperatorDifferentStrings",
            RunTestsRCStringGetAddressOperatorDifferentStrings);

    RunTest(suite, "RunTestsRCStringGetAddressOperatorAfterAssignment",
            RunTestsRCStringGetAddressOperatorAfterAssignment);

    suite.PrintSummary();
    std::cout << COLOR_BLUE "Finished tests\n" COLOR_RESET;
}

static void RunTests()
{
    RunTestsRCStringConstructors();
    RunTestsRCStringLength();
    RunTestsRCStringComparisonBigger();
    RunTestsRCStringComparisonSmaller();
    RunTestsRCStringComparisonEquals();
    RunTestsRCStringAssignment();
    RunTestsRCStringSquareBrackets();
    RunTestsRCStringGetAddressOperator();
}

int main()
{
    RunTests();

    return 0;
}

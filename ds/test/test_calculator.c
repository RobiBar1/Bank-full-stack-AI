#include <stdio.h>
#include <math.h>
#include <float.h>

#include "calculator.h"

#define COLOR_RED		"\033[31m"
#define COLOR_GREEN		"\033[32m"
#define COLOR_YELLOW	"\033[33m"
#define COLOR_BLUE		"\033[34m"
#define COLOR_RESET		"\033[0m"
#define PASS (1)
#define FAIL (0)
#define UNUSED(x) ((void)(x))
#define RUN_TEST(suite, test_fn)							\
do															\
{															\
	test_result_t result = {0};								\
	(suite).total++;										\
															\
	if (test_fn(&result) == PASS)							\
	{														\
		(suite).passed++;									\
	}														\
	else													\
	{														\
		printf(COLOR_RED "[FAIL] %s: %s\n" COLOR_RESET,		\
				#test_fn, result.message);					\
	}														\
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

static int CheckStatus(status_t res, status_t expected, test_result_t* result)
{
	int status = PASS;

	if (res != expected)
	{
		sprintf(result->message,
				COLOR_RED "Status is wrong! Got %d, expected %d" COLOR_RESET,
				res, expected);
		status = FAIL;
    }

	return status;
}

static int AreNearlyEqual(double one, double other)
{
	const double EPSILON = 1e-9;
	
	return fabs(one - other) <= fmax(fabs(one), fabs(other)) * EPSILON;
}

static int CheckResult(double res, double expected, test_result_t* result)
{
    int status = PASS;

    if (!AreNearlyEqual(res, expected))
    {
        sprintf(result->message,
                COLOR_RED "Result is wrong! Got %f, expected %f" COLOR_RESET,
                res, expected);
        status = FAIL;
    }

	return status;
}

static int TestCalculatorSingleNumber(test_result_t* result)
{
	char* expression = "7";
	double res = 0;
	double expected_res = 7;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorSingleFloatNumber(test_result_t* result)
{
	char* expression = ".7";
	double res = 0;
	double expected_res = 0.7;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorSinglePlusNumber(test_result_t* result)
{
	char* expression = "+7";
	double res = 0;
	double expected_res = 7;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorSingleMinusNumber(test_result_t* result)
{
	char* expression = "-7";
	double res = 0;
	double expected_res = -7;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static void RunTestsCalculatorSingleNumber()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for single number\n" COLOR_RESET);

	RUN_TEST(suite, TestCalculatorSingleNumber);
	RUN_TEST(suite, TestCalculatorSingleFloatNumber);
	RUN_TEST(suite, TestCalculatorSinglePlusNumber);
	RUN_TEST(suite, TestCalculatorSingleMinusNumber);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCalculatorBasicAddition(test_result_t* result)
{
	char* expression = "1+1";
	double res = 0;
	double expected_res = 2;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicAdditionFirstPositive(test_result_t* result)
{
	char* expression = "+1+1";
	double res = 0;
	double expected_res = 2;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicAdditionSecondPositive(test_result_t* result)
{
	char* expression = "1++1";
	double res = 0;
	double expected_res = 2;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicAdditionFirstNegative(test_result_t* result)
{
	char* expression = "-1+1";
	double res = 0;
	double expected_res = 0;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicAdditionSecondNegative(test_result_t* result)
{
	char* expression = "1+-1";
	double res = 0;
	double expected_res = 0;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static void RunTestsCalculatorBasicAddition()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for basic addition\n" COLOR_RESET);

	RUN_TEST(suite, TestCalculatorBasicAddition);
	RUN_TEST(suite, TestCalculatorBasicAdditionFirstPositive);
	RUN_TEST(suite, TestCalculatorBasicAdditionSecondPositive);
	RUN_TEST(suite, TestCalculatorBasicAdditionFirstNegative);
	RUN_TEST(suite, TestCalculatorBasicAdditionSecondNegative);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCalculatorBasicSubtraction(test_result_t* result)
{
	char* expression = "1-1";
	double res = 0;
	double expected_res = 0;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicSubtractionFirstPositive(test_result_t* result)
{
	char* expression = "+1-1";
	double res = 0;
	double expected_res = 0;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicSubtractionSecondPositive(test_result_t* result)
{
	char* expression = "1-+1";
	double res = 0;
	double expected_res = 0;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicSubtractionFirstNegative(test_result_t* result)
{
	char* expression = "-1-1";
	double res = 0;
	double expected_res = -2;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicSubtractionSecondNegative(test_result_t* result)
{
	char* expression = "1--1";
	double res = 0;
	double expected_res = 2;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static void RunTestsCalculatorBasicSubtraction()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for basic subtraction\n" COLOR_RESET);

	RUN_TEST(suite, TestCalculatorBasicSubtraction);
	RUN_TEST(suite, TestCalculatorBasicSubtractionFirstPositive);
	RUN_TEST(suite, TestCalculatorBasicSubtractionSecondPositive);
	RUN_TEST(suite, TestCalculatorBasicSubtractionFirstNegative);
	RUN_TEST(suite, TestCalculatorBasicSubtractionSecondNegative);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCalculatorBasicMultiplication(test_result_t* result)
{
	char* expression = "2*3";
	double res = 0;
	double expected_res = 6;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicMultiplicationFirstPositive(test_result_t* result)
{
	char* expression = "+2*3";
	double res = 0;
	double expected_res = 6;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicMultiplicationSecondPositive(test_result_t* result)
{
	char* expression = "2*+3";
	double res = 0;
	double expected_res = 6;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicMultiplicationFirstNegative(test_result_t* result)
{
	char* expression = "-2*3";
	double res = 0;
	double expected_res = -6;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicMultiplicationSecondNegative(test_result_t* result)
{
	char* expression = "2*-3";
	double res = 0;
	double expected_res = -6;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static void RunTestsCalculatorBasicMultiplication()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for basic multiplication\n" COLOR_RESET);

	RUN_TEST(suite, TestCalculatorBasicMultiplication);
	RUN_TEST(suite, TestCalculatorBasicMultiplicationFirstPositive);
	RUN_TEST(suite, TestCalculatorBasicMultiplicationSecondPositive);
	RUN_TEST(suite, TestCalculatorBasicMultiplicationFirstNegative);
	RUN_TEST(suite, TestCalculatorBasicMultiplicationSecondNegative);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCalculatorBasicDivision(test_result_t* result)
{
	char* expression = "6/3";
	double res = 0;
	double expected_res = 2;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicDivisionFirstPositive(test_result_t* result)
{
	char* expression = "+6/3";
	double res = 0;
	double expected_res = 2;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicDivisionSecondPositive(test_result_t* result)
{
	char* expression = "6/+3";
	double res = 0;
	double expected_res = 2;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicDivisionFirstNegative(test_result_t* result)
{
	char* expression = "-6/3";
	double res = 0;
	double expected_res = -2;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicDivisionSecondNegative(test_result_t* result)
{
	char* expression = "6/-3";
	double res = 0;
	double expected_res = -2;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static void RunTestsCalculatorBasicDivision()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for basic division\n" COLOR_RESET);

	RUN_TEST(suite, TestCalculatorBasicDivision);
	RUN_TEST(suite, TestCalculatorBasicDivisionFirstPositive);
	RUN_TEST(suite, TestCalculatorBasicDivisionSecondPositive);
	RUN_TEST(suite, TestCalculatorBasicDivisionFirstNegative);
	RUN_TEST(suite, TestCalculatorBasicDivisionSecondNegative);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCalculatorBasicParentheses(test_result_t* result)
{
	char* expression = "(1)";
	double res = 0;
	double expected_res = 1;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicPlusParentheses(test_result_t* result)
{
	char* expression = "+(1)";
	double res = 0;
	double expected_res = 1;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicMinusParentheses(test_result_t* result)
{
	char* expression = "-(1)";
	double res = 0;
	double expected_res = -1;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorParenthesesWithAddition(test_result_t* result)
{
	char* expression = "(1+2)";
	double res = 0;
	double expected_res = 3;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorParenthesesWithAdditionAndMultiplication(test_result_t* result)
{
	char* expression = "(1+2*3)";
	double res = 0;
	double expected_res = 7;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorParenthesesNested(test_result_t* result)
{
	char* expression = "(1+(2*(3+4)))";
	double res = 0;
	double expected_res = 15;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorParenthesesDeepNested(test_result_t* result)
{
	char* expression = "((((((((((1))))))))))";
	double res = 0;
	double expected_res = 1;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorParenthesesOverrideMultiplication(test_result_t* result)
{
	char* expression = "(1+2)*3";
	double res = 0;
	double expected_res = 9;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorParenthesesOverridePower(test_result_t* result)
{
	char* expression = "(4^3)^2";
	double res = 0;
	double expected_res = 4096;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static void RunTestsCalculatorParentheses()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for basic parenthesis\n" COLOR_RESET);

	RUN_TEST(suite, TestCalculatorBasicParentheses);
	RUN_TEST(suite, TestCalculatorBasicPlusParentheses);
	RUN_TEST(suite, TestCalculatorBasicMinusParentheses);
	RUN_TEST(suite, TestCalculatorParenthesesWithAddition);
	RUN_TEST(suite, TestCalculatorParenthesesWithAdditionAndMultiplication);
	RUN_TEST(suite, TestCalculatorParenthesesNested);
	RUN_TEST(suite, TestCalculatorParenthesesDeepNested);
	RUN_TEST(suite, TestCalculatorParenthesesOverrideMultiplication);
	RUN_TEST(suite, TestCalculatorParenthesesOverridePower);
	

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCalculatorBasicPower(test_result_t* result)
{
	char* expression = "4^2";
	double res = 0;
	double expected_res = 16;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicPowerNegativeBase(test_result_t* result)
{
	char* expression = "(-4)^2";
	double res = 0;
	double expected_res = 16;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicPowerFirstPositive(test_result_t* result)
{
	char* expression = "+4^2";
	double res = 0;
	double expected_res = 16;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicPowerSecondPositive(test_result_t* result)
{
	char* expression = "4^+2";
	double res = 0;
	double expected_res = 16;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicPowerFirstNegative(test_result_t* result)
{
	char* expression = "-4^2";
	double res = 0;
	double expected_res = -16;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorBasicPowerSecondNegative(test_result_t* result)
{
	char* expression = "4^-1";
	double res = 0;
	double expected_res = 0.25;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static void RunTestsCalculatorBasicPower()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for basic power\n" COLOR_RESET);

	RUN_TEST(suite, TestCalculatorBasicPower);
	RUN_TEST(suite, TestCalculatorBasicPowerNegativeBase);
	RUN_TEST(suite, TestCalculatorBasicPowerFirstPositive);
	RUN_TEST(suite, TestCalculatorBasicPowerSecondPositive);
	RUN_TEST(suite, TestCalculatorBasicPowerFirstNegative);
	RUN_TEST(suite, TestCalculatorBasicPowerSecondNegative);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCalculatorBasicDivisionBy0(test_result_t* result)
{
	char* expression = "1/0";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = MATH_ERROR;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorAddThenDivideBy0(test_result_t* result)
{
	char* expression = "1+1/0";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = MATH_ERROR;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorDivideBy0ThanAdd(test_result_t* result)
{
	char* expression = "1/0+1";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = MATH_ERROR;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorDivideBy0WithParentheses(test_result_t* result)
{
	char* expression = "1/(1-1)";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = MATH_ERROR;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static void RunTestsCalculatorDivisionBy0()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for division by 0\n" COLOR_RESET);

	RUN_TEST(suite, TestCalculatorBasicDivisionBy0);
	RUN_TEST(suite, TestCalculatorAddThenDivideBy0);
	RUN_TEST(suite, TestCalculatorDivideBy0ThanAdd);
	RUN_TEST(suite, TestCalculatorDivideBy0WithParentheses);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCalculatorAddThenMultiply(test_result_t* result)
{
	char* expression = "1+2*3";
	double res = 0;
	double expected_res = 7;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorMultiplyThenAdd(test_result_t* result)
{
	char* expression = "1*2+3";
	double res = 0;
	double expected_res = 5;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorSubtractThenMultiply(test_result_t* result)
{
	char* expression = "1-2*3";
	double res = 0;
	double expected_res = -5;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorMultiplyThenSubtract(test_result_t* result)
{
	char* expression = "1*2-3";
	double res = 0;
	double expected_res = -1;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorAddThenDivide(test_result_t* result)
{
	char* expression = "12+4/2";
	double res = 0;
	double expected_res = 14;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorDivideThenAdd(test_result_t* result)
{
	char* expression = "12/4+2";
	double res = 0;
	double expected_res = 5;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorSubtractThenDivide(test_result_t* result)
{
	char* expression = "12-4/2";
	double res = 0;
	double expected_res = 10;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorDivideThenSubtract(test_result_t* result)
{
	char* expression = "12/4-2";
	double res = 0;
	double expected_res = 1;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static void RunTestsCalculatorPrecedence()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for precedence\n" COLOR_RESET);

	RUN_TEST(suite, TestCalculatorAddThenMultiply);
	RUN_TEST(suite, TestCalculatorMultiplyThenAdd);
	RUN_TEST(suite, TestCalculatorSubtractThenMultiply);
	RUN_TEST(suite, TestCalculatorMultiplyThenSubtract);
	RUN_TEST(suite, TestCalculatorAddThenDivide);
	RUN_TEST(suite, TestCalculatorDivideThenAdd);
	RUN_TEST(suite, TestCalculatorSubtractThenDivide);
	RUN_TEST(suite, TestCalculatorDivideThenSubtract);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCalculatorTest1(test_result_t* result)
{
	char* expression = "7+8";
	double res = 0;
	double expected_res = 15;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorTest2(test_result_t* result)
{
	char* expression = "8+8*3+-2^5";
	double res = 0;
	double expected_res = 0;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorTest3(test_result_t* result)
{
	char* expression = "8+8*3-2^";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorTest4(test_result_t* result)
{
	char* expression = "2/0";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = MATH_ERROR;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorTest5(test_result_t* result)
{
	char* expression = "8++8*((3-2)*5)";
	double res = 0;
	double expected_res = 48;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorTest6(test_result_t* result)
{
	char* expression = "3-2)*5";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorTest7(test_result_t* result)
{
	char* expression = "(3-2)*5+ 5*(4+4+4";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorTest8(test_result_t* result)
{
	char* expression = "4-3-2-1\n";
	double res = 0;
	double expected_res = -2;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static int TestCalculatorTest9(test_result_t* result)
{
	char* expression = "4^3^2^1\n";
	double res = 0;
	double expected_res = 262144;
	status_t status = SUCCESS;
	status_t expected_status = SUCCESS;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result) &&
			CheckResult(res, expected_res, result));
}

static void RunTestsCalculatorComplexExpressions()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests with complex expressions\n" COLOR_RESET);

	RUN_TEST(suite, TestCalculatorTest1);
	RUN_TEST(suite, TestCalculatorTest2);
	RUN_TEST(suite, TestCalculatorTest3);
	RUN_TEST(suite, TestCalculatorTest4);
	RUN_TEST(suite, TestCalculatorTest5);
	RUN_TEST(suite, TestCalculatorTest6);
	RUN_TEST(suite, TestCalculatorTest7);
	RUN_TEST(suite, TestCalculatorTest8);
	RUN_TEST(suite, TestCalculatorTest9);
	

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCalculatorEmptyExpression(test_result_t* result)
{
	char* expression = "";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorOnlyPlus(test_result_t* result)
{
	char* expression = "+";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorMissingFirstOperand(test_result_t* result)
{
	char* expression = "*1";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorMissingSecondOperand(test_result_t* result)
{
	char* expression = "1+";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorDoubleUnaryPlus(test_result_t* result)
{
	char* expression = "++1";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorDoubleUnaryMinus(test_result_t* result)
{
	char* expression = "--1";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorDoubleMultiplication(test_result_t* result)
{
	char* expression = "1**1";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorDoubleDivision(test_result_t* result)
{
	char* expression = "1//1";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorDoublePower(test_result_t* result)
{
	char* expression = "1^^1";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorMissingCloseParenthesis(test_result_t* result)
{
	char* expression = "(";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorMissingOpenParenthesis(test_result_t* result)
{
	char* expression = ")";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorMissingCloseParenthesisWithExpression(test_result_t* result)
{
	char* expression = "(1+2";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorMissingOpenParenthesisWithExpression(test_result_t* result)
{
	char* expression = "1+2)";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static int TestCalculatorOnlyParentheses(test_result_t* result)
{
	char* expression = "()";
	double res = 0;
	status_t status = SUCCESS;
	status_t expected_status = INVALID_EXPRESSION;

	status = Calculator(expression, &res);

	return (CheckStatus(status, expected_status, result));
}

static void RunTestsCalculatorInvalidExpressions()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests with invalid expressions\n" COLOR_RESET);

	RUN_TEST(suite, TestCalculatorEmptyExpression);
	RUN_TEST(suite, TestCalculatorOnlyPlus);
	RUN_TEST(suite, TestCalculatorMissingFirstOperand);
	RUN_TEST(suite, TestCalculatorMissingSecondOperand);
	RUN_TEST(suite, TestCalculatorDoubleUnaryPlus);
	RUN_TEST(suite, TestCalculatorDoubleUnaryMinus);
	RUN_TEST(suite, TestCalculatorDoubleMultiplication);
	RUN_TEST(suite, TestCalculatorDoubleDivision);
	RUN_TEST(suite, TestCalculatorDoublePower);
	RUN_TEST(suite, TestCalculatorMissingCloseParenthesis);
	RUN_TEST(suite, TestCalculatorMissingOpenParenthesis);
	RUN_TEST(suite, TestCalculatorMissingCloseParenthesisWithExpression);
	RUN_TEST(suite, TestCalculatorMissingOpenParenthesisWithExpression);
	RUN_TEST(suite, TestCalculatorOnlyParentheses);


	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static void RunTests()
{
	RunTestsCalculatorSingleNumber();
	RunTestsCalculatorBasicAddition();
	RunTestsCalculatorBasicSubtraction();
	RunTestsCalculatorBasicMultiplication();
	RunTestsCalculatorBasicDivision();
	RunTestsCalculatorBasicPower();
	RunTestsCalculatorParentheses();
	RunTestsCalculatorDivisionBy0();
	RunTestsCalculatorPrecedence();
	RunTestsCalculatorInvalidExpressions();
	RunTestsCalculatorComplexExpressions();
}

int main()
{
	RunTests();

	return 0;
}


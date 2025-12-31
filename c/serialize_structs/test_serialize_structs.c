#include <stdio.h> /* FILE */
#include <stdlib.h> /* system */
#include <stdio.h> /* printf */
#include <assert.h> /* assert */

#include "String.h"
#include "serialize_structs.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

static void CheckTest(test_status_t result, char* name);
static void ConstructBasicStudent(student_t* student);
static void ConstructDefaultStudent(student_t* student);
static void PrintStudent(student_t* student);
static int CompareStudents(student_t* s1, student_t* s2);

/* Tests Forward Declerations */
static test_status_t CheckSaveStudentToFileWithoutErrors();
static test_status_t CheckLoadStudentMatch();

/* gd test_serialize_structs.c serialize_structs.c ../pointers2/String.c -I ../pointers2/ */

static void CheckTest(test_status_t result, char* name)
{
	if (result == TEST_STATUS_SUCCESS)
	{
		printf("-> %s: ", name);
		printf(GREEN "PASSED\n\n" RESET);
	}
	else 
	{
		printf(RED "-> %s: FAILED\n\n" RESET, name);
	}
}

static void ConstructBasicStudent(student_t* student)
{
	StrCpy(student->first_name, "mars");
	StrCpy(student->last_name, "blue");
	
	student->grades.humanities.literature = 80.7;
	student->grades.humanities.history = 82.7;
	student->grades.humanities.art = 70.5;
	student->grades.real.physics = 85.4;
	student->grades.real.chemistry = 92.4;
	student->grades.real.biology = 77.4;
	student->grades.real.computers = 98.5;
	student->grades.real.maths = 96.2;
	student->grades.sport = 90.5;
}

static void ConstructDefaultStudent(student_t* student)
{
	StrCpy(student->first_name, "first_name");
	StrCpy(student->last_name, "last_name");
	
	student->grades.humanities.literature = 0;
	student->grades.humanities.history = 0;
	student->grades.humanities.art = 0;
	student->grades.real.physics = 0;
	student->grades.real.chemistry = 0;
	student->grades.real.biology = 0;
	student->grades.real.computers = 0;
	student->grades.real.maths = 0;
	student->grades.sport = 0;
}


static void PrintStudent(student_t* student)
{
	assert(NULL != student);
	
	printf("-- %s , %s --\n",student->first_name, student->last_name);
	
	printf("humanities: literature: %.2f\n", student->grades.humanities.literature);
	printf("humanities: history: %.2f\n", student->grades.humanities.history);
	printf("humanities: art: %.2f\n", student->grades.humanities.art);
	printf("real: physics: %.2f\n", student->grades.real.physics);
	printf("real: chemistry: %.2f\n", student->grades.real.chemistry);
	printf("real: biology: %.2f\n", student->grades.real.biology);
	printf("real: computers: %.2f\n", student->grades.real.computers);
	printf("real: maths: %.2f\n", student->grades.real.maths);
	printf("sport: %.2f\n", student->grades.sport);
}


static int CompareStudents(student_t* s1, student_t* s2)
{
	int same = 0;
	
	same += (s1->grades.humanities.literature != s2->grades.humanities.literature);
	same += (s1->grades.humanities.history != s2->grades.humanities.history);
	same += (s1->grades.humanities.art != s2->grades.humanities.art);
	same += (s1->grades.real.physics != s2->grades.real.physics);
	same += (s1->grades.real.chemistry != s2->grades.real.chemistry);
	same += (s1->grades.real.biology != s2->grades.real.biology);
	same += (s1->grades.real.computers != s2->grades.real.computers);
	same += (s1->grades.real.maths != s2->grades.real.maths);
	same += (s1->grades.sport != s2->grades.sport);
	
	return (0 == same);
}


int main()
{
	printf("\n\n---- TEST BASIC ----\n\n");
	
	CheckTest(CheckSaveStudentToFileWithoutErrors(), "CheckSaveStudentToFileWithoutErrors()");
	CheckTest(CheckLoadStudentMatch(), "CheckLoadStudentMatch()");

	return 0;
}

/* Test Implementations */

static test_status_t CheckSaveStudentToFileWithoutErrors()
{
	char* file_name = "test_CheckSaveStudentToFile";
	FILE* file_ptr = NULL;
	status_t status = SUCCESS;
	student_t student;
	
	ConstructBasicStudent(&student);
	
	file_ptr = fopen(file_name,"wb");
	status = SaveStudentToBinary(&student, file_ptr);
	fclose(file_ptr);
	
	system("hexdump -C test_CheckSaveStudentToFile");
	
	if (SUCCESS == status)
	{
		return TEST_STATUS_SUCCESS;
	}
	return TEST_STATUS_FAILURE;
}


static test_status_t CheckLoadStudentMatch()
{
	char* file_name = "test_CheckLoadStudentMatch";
	FILE* file_ptr = NULL;
	status_t status = SUCCESS;
	student_t student_saved;
	student_t student_loaded;
	
	ConstructBasicStudent(&student_saved);
	ConstructDefaultStudent(&student_loaded);
	
	file_ptr = fopen(file_name,"wb");
	status = SaveStudentToBinary(&student_saved, file_ptr);
	fclose(file_ptr);
	if (SUCCESS != status)
	{
		return TEST_STATUS_FAILURE;
	}
	
	printf("test_CheckLoadStudentMatch binary file (just entered)\n");
	system("hexdump -C test_CheckLoadStudentMatch");
	
	file_ptr = fopen(file_name,"rb");
	status = LoadStudentFromBinary(&student_loaded, file_ptr);
	fclose(file_ptr);
	if (SUCCESS != status)
	{
		return TEST_STATUS_FAILURE;
	}
	
	printf("Loaded Student:\n");
	PrintStudent(&student_loaded);
	
	if(0 == CompareStudents(&student_saved, &student_loaded))
	{
		return TEST_STATUS_FAILURE;
	}
	
	return TEST_STATUS_SUCCESS;
}








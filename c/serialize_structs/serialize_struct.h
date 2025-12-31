#ifndef __SERIALIZE_STRUCTS_H__
#define __SERIALIZE_STRUCTS_H__

#include <stdio.h> /* FILE */

#define NAME_SIZE (48)

typedef enum {SUCCESS,FILE_WRITE_ERROR, FILE_READ_ERROR} status_t;

typedef struct GradesR grades_r_t;
typedef struct GradesH grades_h_t;
typedef struct Grades grades_t;
typedef struct Student student_t;

struct GradesH
{
    float literature;
    float history;
    float art;
};

struct GradesR
{
    float physics;
    float chemistry;
    float biology;
    float computers;
    float maths;
};

struct Grades
{
    grades_h_t humanities;
    grades_r_t real;
    float sport;
};

struct Student
{
    char first_name[NAME_SIZE];
    char last_name[NAME_SIZE];
    grades_t grades;
};

status_t SaveStudentToBinary(student_t* student, FILE* file_ptr);
status_t LoadStudentFromBinary(student_t* student, FILE* file_ptr);

#endif /*__SERIALIZE_STRUCTS_H__*/



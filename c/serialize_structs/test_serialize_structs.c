#include <stdio.h>
#include <string.h>
#include "serialize_structs.h"

void CmpStruct(student_t* student1, student_t* student2)
{
    if (strcmp(student1->first_name, student2->first_name) == 0 &&
        strcmp(student1->last_name, student2->last_name) == 0 &&
        memcmp(&student1->grades, &student2->grades, sizeof(grades_t)) == 0)
    {
        printf("%s is good!\n", student2->first_name);
    }
    else
    {
        printf("%s********* no good *********\n", student2->first_name);
    }
}

int main()
{
    FILE* fp = NULL;
    student_t shahar;
    student_t shahar_copy;

    strncpy(shahar.first_name, "shahar", NAME_SIZE);
    strncpy(shahar.last_name, "bar natan", NAME_SIZE);

    shahar.grades.humanities.literature = 100.0;
    shahar.grades.humanities.history = 90.0;
    shahar.grades.humanities.art = 80.0;

    shahar.grades.real.physics = 95.0;
    shahar.grades.real.chemistry = 85.0;
    shahar.grades.real.biology = 75.0;
    shahar.grades.real.computers = 65.0;
    shahar.grades.real.maths = 55.0;

    shahar.grades.sport = 85.5;

    /*fp = fopen("shahar.bin", "wb");*/
    
    SaveStudentToBinary(&shahar, "shahar.bin");
    /*fclose(fp);*/
    
    /*fp = fopen("shahar.bin", "rb");*/
    LoadStudentFromBinary(&shahar_copy, "shahar.bin");
    /*fclose(fp);*/

    printf("\n--- Loaded Data Verification ---\n");
    printf("Full Name: %s %s\n", shahar_copy.first_name, shahar_copy.last_name);

    printf("\n[Humanities Grades]\n");
    printf("Literature: %.1f\n", shahar_copy.grades.humanities.literature);
    printf("History:    %.1f\n", shahar_copy.grades.humanities.history);
    printf("Art:        %.1f\n", shahar_copy.grades.humanities.art);

    printf("\n[Real Grades]\n");
    printf("Physics:    %.1f\n", shahar_copy.grades.real.physics);
    printf("Chemistry:  %.1f\n", shahar_copy.grades.real.chemistry);
    printf("Biology:    %.1f\n", shahar_copy.grades.real.biology);
    printf("Computers:  %.1f\n", shahar_copy.grades.real.computers);
    printf("Maths:      %.1f\n", shahar_copy.grades.real.maths);

    printf("\n[Other]\n");
    printf("Sport:      %.1f\n", shahar_copy.grades.sport);

	printf("\n--- compare all fields ---\n");
    CmpStruct(&shahar, &shahar_copy);

    return 0;
}

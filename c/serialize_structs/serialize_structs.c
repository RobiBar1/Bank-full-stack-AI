#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>				/* assert */

#include "String.h" 			/* StrLen, StrNCpy */
#include "serialize_structs.h"

#define JUNK 0
#define NUM_TO_WRITE 1

/* --------------------- Initialization Functions -------------------------- */
static grades_h_t InitGradesH(float literature, float history, float art)
{
    grades_h_t g = {JUNK};
    g.literature = literature;
    g.history = history;
    g.art = art;
    
    return g;
}

static grades_r_t InitGradesR(float physics, float chemistry, float biology, 
                              float computers, float maths)
{
    grades_r_t g = {JUNK};
    g.physics = physics;
    g.chemistry = chemistry;
    g.biology = biology;
    g.computers = computers;
    g.maths = maths;
    
    return g;
}

static grades_t InitGrades(float sport, grades_h_t hum, grades_r_t real)
{
    grades_t g = {JUNK};
    g.sport = sport;
    g.humanities = hum;
    g.real = real;
    return g;
}

/* ---------------------------- Helper functions ---------------------- */
student_t InitStudent(const char* first, const char* last, float sport_grade)
{
    student_t s = {""};
    size_t str_size = 0;
    
    assert(NULL != first);
    assert(NULL != last);
    
    /* Clear memory to avoid garbage in padding */
    memset(&s, 0, sizeof(student_t));
    str_size = StrLen(first);
    StrNCpy(s.first_name, first, str_size);
    s.first_name[str_size] = '\0';
	str_size = StrLen(last);
    StrNCpy(s.last_name, last, str_size);
    s.last_name[str_size] = '\0';

    s.grades = InitGrades(sport_grade, 
                          InitGradesH(80.0f, 85.5f, 90.0f), 
                          InitGradesR(70.0f, 75.0f, 80.0f, 95.5f, 100.0f));
    return s;
}

/* These functions focus on writing specific sub-structs */
static status_t WriteGradesH(FILE* fp, const grades_h_t* gh)
{
	assert(NULL != fp);
	assert(NULL != gh);
	
    if (NUM_TO_WRITE != fwrite(gh, sizeof(grades_h_t), NUM_TO_WRITE, fp))
    {
        return FILE_WRITE_ERROR;
    }
    
    return SUCCESS;
}

static status_t WriteGradesR(FILE* fp, const grades_r_t* gr)
{
	assert(NULL != fp);
	assert(NULL != gr);
	
    if (NUM_TO_WRITE != fwrite(gr, sizeof(grades_r_t), NUM_TO_WRITE, fp))
    {
        return FILE_WRITE_ERROR;
    }
        
    return SUCCESS;
}

static status_t WriteGrades(FILE* fp, const grades_t* g)
{
	assert(NULL != fp);
	assert(NULL != g);
	
    if (SUCCESS != WriteGradesH(fp, &g->humanities))
    {
    	return FILE_WRITE_ERROR;
    }
    
    if (SUCCESS != WriteGradesR(fp, &g->real))
    {
    	return FILE_WRITE_ERROR;
	}
    
    if (NUM_TO_WRITE != fwrite(&g->sport, sizeof(float), NUM_TO_WRITE, fp))
    {
    	return FILE_WRITE_ERROR;
    }

    return SUCCESS;
}

/* ------------------------------ Load Fcuntions -------------------------- */
static status_t ReadGradesH(FILE* fp, grades_h_t* gh)
{
	assert(NULL != fp);
	assert(NULL != gh);
	
    if (NUM_TO_WRITE != fread(gh, sizeof(grades_h_t), NUM_TO_WRITE, fp))
    {
    	return FILE_READ_ERROR;
    }
    
    return SUCCESS;
}

static status_t ReadGradesR(FILE* fp, grades_r_t* gr)
{
	assert(NULL != fp);
	assert(NULL != gr);
	
    if (NUM_TO_WRITE != fread(gr, sizeof(grades_r_t), NUM_TO_WRITE, fp))
    {
		return FILE_READ_ERROR;
	}
	
    return SUCCESS;
}

static status_t ReadGrades(FILE* fp, grades_t* g)
{
	assert(NULL != fp);
	assert(NULL != g);
	
    if (SUCCESS != ReadGradesH(fp, &g->humanities))
    {
    	return FILE_READ_ERROR;
    }
    if (SUCCESS != ReadGradesR(fp, &g->real))
    {
    	return FILE_READ_ERROR;
    }
    
    if (NUM_TO_WRITE != fread(&g->sport, sizeof(float), NUM_TO_WRITE, fp))
    {
    	return FILE_READ_ERROR;
    }

    return SUCCESS;
}


/* --------------------------- H.file Implementation ------------------- */

status_t SaveStudentToBinary(student_t* student, char* file_name)
{
    FILE* fp = NULL;
    
    assert(NULL != student);
    assert(NULL != file_name);

    fp = fopen(file_name, "wb");
    if (!fp)
    {
    	return FILE_OPEN_ERROR;
    }
    
    if (fwrite(student->first_name, sizeof(char), NAME_SIZE, fp) != NAME_SIZE) 
    {
    	fclose(fp);
    	return FILE_WRITE_ERROR; 
    }
    
    if (fwrite(student->last_name, sizeof(char), NAME_SIZE, fp) != NAME_SIZE) 
    {
    	fclose(fp);
    	return FILE_WRITE_ERROR;
    } 

    if (SUCCESS != WriteGrades(fp, &student->grades))
    {
    	fclose(fp);
    	return FILE_WRITE_ERROR;
    }

    if (SUCCESS != fclose(fp))
    {
    	return FILE_CLOSE_ERROR;
    }
    
    return SUCCESS;
}

status_t LoadStudentFromBinary(student_t* student, char* file_name)
{
    FILE* fp = NULL;
    
    assert(NULL != student);
    assert(NULL != file_name);

    fp = fopen(file_name, "rb");
    if (!fp)
    {
    	return FILE_OPEN_ERROR;
    }
    
    if (fread(student->first_name, sizeof(char), NAME_SIZE, fp) != NAME_SIZE) 
    {
    	fclose(fp);
    	return FILE_READ_ERROR; 
    }
	
    if (fread(student->last_name, sizeof(char), NAME_SIZE, fp) != NAME_SIZE) 
    {
    	fclose(fp);
    	return FILE_READ_ERROR;
    } 

    if (SUCCESS != ReadGrades(fp, &student->grades))
    {
    	fclose(fp);
    	return FILE_READ_ERROR;
    }

    if (SUCCESS != fclose(fp))
    {
    	return FILE_CLOSE_ERROR;
    }
	
    return SUCCESS;
}

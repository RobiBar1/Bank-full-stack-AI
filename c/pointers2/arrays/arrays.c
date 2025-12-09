#include <stdio.h>

int ToLower(int ch)
{
	if (ch >= 'A' && ch <= 'Z')
	{
		ch += 32; 
	}
	
	return ch;
}

void EnvironmentVariblesPrint(const char* envp[])
{
	printf("%s", envp[0]);
	printf("%s", envp[1]);
}

void AllDataTypesPrint(void)
{
    char char_;
    signed char signed_char;
    unsigned char unsigned_char;

    short short_;
    short int short_int;
    signed short signed_short;
    signed short int signed_short_int;
    unsigned short unsigned_short;
    unsigned short int unsigned_short_int;

    int int_;
    signed signed_;
    signed int signed_int;
    unsigned unsigned_;
    unsigned int unsigned_int;

    long long_;
    long int long_int;
    signed long signed_long;
    signed long int signed_long_int;
    unsigned long unsigned_long;
    unsigned long int unsigned_long_int; 

    /* (unsigned long) cast for maximum C89 compatibility */
    
    printf("--- CHAR ---\n");
    printf("char:                   %lu bytes\n", (unsigned long)sizeof(char_));
    printf("signed char:            %lu bytes\n", (unsigned long)sizeof(signed_char));
    printf("unsigned char:          %lu bytes\n", (unsigned long)sizeof(unsigned_char));
    printf("\n");

    printf("--- SHORT ---\n");
    printf("short:                  %lu bytes\n", (unsigned long)sizeof(short_));
    printf("short int:              %lu bytes\n", (unsigned long)sizeof(short_int));
    printf("signed short:           %lu bytes\n", (unsigned long)sizeof(signed_short));
    printf("signed short int:       %lu bytes\n", (unsigned long)sizeof(signed_short_int));
    printf("unsigned short:         %lu bytes\n", (unsigned long)sizeof(unsigned_short));
    printf("unsigned short int:     %lu bytes\n", (unsigned long)sizeof(unsigned_short_int));
    printf("\n");

    printf("--- INT ---\n");
    printf("int:                    %lu bytes\n", (unsigned long)sizeof(int_));
    printf("signed:                 %lu bytes\n", (unsigned long)sizeof(signed_));
    printf("signed int:             %lu bytes\n", (unsigned long)sizeof(signed_int));
    printf("unsigned:               %lu bytes\n", (unsigned long)sizeof(unsigned_));
    printf("unsigned int:           %lu bytes\n", (unsigned long)sizeof(unsigned_int));
    printf("\n");

    printf("--- LONG ---\n");
    printf("long:                   %lu bytes\n", (unsigned long)sizeof(long_));
    printf("long int:               %lu bytes\n", (unsigned long)sizeof(long_int));
    printf("signed long:            %lu bytes\n", (unsigned long)sizeof(signed_long));
    printf("signed long int:        %lu bytes\n", (unsigned long)sizeof(signed_long_int));
    printf("unsigned long:          %lu bytes\n", (unsigned long)sizeof(unsigned_long));
    printf("unsigned long int:      %lu bytes\n", (unsigned long)sizeof(unsigned_long_int));
    printf("\n");
}



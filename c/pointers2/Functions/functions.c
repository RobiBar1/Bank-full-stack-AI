#include <stdio.h>
#include <stdlib.h>

/* syntax: typedef <return_type> (*<new_type_name>)(<arg_types>); */
typedef void (*KeyHandler)(void);  /* pointer to functions exmpl*/
typedef unsigned int uint;        /* pointer to basic_type exmpl */
typedef int (*ArrayPtr)[256];    /* pointer to arr exmpl*/


void OnAPressed() 
{
    printf("A-pressed\n");
}

void OnTPressed() 
{
    printf("T-pressed\n");
}

void DoNothing() 
{
    
}

void OnEscPressed() 
{
    printf("Exiting...\n");
    printf("return call is: %d\n", system("stty icanon echo")); 
    exit(0);
}

int main() 
{
    char c = 10;
    size_t i = 0;
	KeyHandler jumpTable[256];
    
    printf("return call is: %d\n", system("stty -icanon -echo")); 
	
    for (; i < 256; ++i)
    {
        jumpTable[i] = DoNothing;
    }

    jumpTable['A'] = OnAPressed;
    jumpTable['a'] = OnAPressed;  
    jumpTable['T'] = OnTPressed;
    jumpTable['t'] = OnTPressed;
    jumpTable[27]  = OnEscPressed; /* ESC number */
    while(1) 
    {
		c = getchar();

        jumpTable[(unsigned char)c]();

        /*
        if (c == 'A') 
        {
            printf("A-pressed\n");
        } 
        else if (c == 'T') 
        {
            printf("T-pressed\n");
        }
        else if (c == 27) 
        { 
            break;
        }
        
        switch(c) 
        {
            case 'A':
                printf("A-pressed\n");
                break;
            case 'T':
                printf("T-pressed\n");
                break;
            case 27: 
                system("stty icanon echo");
				return;
            default:
                break;
        }
        */
    }

    printf("return call is: %d\n", system("stty icanon echo")); 
    return 0;
}

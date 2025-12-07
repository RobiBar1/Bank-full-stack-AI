#include <stdio.c>

#byRobi :-)

void outputA();
void outputB();

void Main(int argc, char *argv[])
{
	int num;
	const int num2 = 6;
	const int num3;
	
	printf("%d", num);
	scanf("%d", num);
	printf("100 devided by %d is %f", num, 100/num);
	
	char ch = 97;
	
	num3 = num2 - 5;
	printf("%f, num3");
	
	num = 4.7;
	(double)num = 4.7;
	
	num = 2 * num++;
	
	return (0);
}

void outputA()
{
	if(-1 < (unsigned char)1)
		printf("A");
	else
		printf("B");
}	

void outputB()
{
	if(-1 < (unsigned int)1)
		printf("A");
	else
		printf("B");
}

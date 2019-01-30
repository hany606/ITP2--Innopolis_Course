#include<stdio.h>
int x = 7;
int y = 5;

char nS[100];


void addition(int x, int y)
{
	printf("SUM = %d",x+y);
}

int multiplication(int x, int y)
{
	return x*y;
}

void charCodeSum(char x, char y)
{
	printf("%c%c",x,y);
}

typedef enum
{
	Fall,
	Spring,
	Winter,
	Summer
}season;

void printMessage(season s)
{

	if(s == Winter)
		printf("Winter is Coming");
	else if(s == Fall)
		printf("Fall is Coming");
	else if(s == Spring)
		printf("Spring is Coming");
	else
		printf("Summer is Coming");
	printf("\n");

}


long long int power(int x,int n)
{
	long long int out = x;
	for(int i = 0; i < n-1; i++)
	{
		out *= x;
	}
	return out;
}
//
int len(char s[])
{
	int i = 0;
	for(i = 0; ; i++)
	{
		if(s[i] == '\0')
			break;
	}
	return i;
}

char* space(char s[])
{
	int size = len(s);
	int nWspace = 0;
	for(int i = 0; i < size; i++)
	{	
		if(s[i] != ' ')
			nWspace++;
	}
	
	
	int x = 0;

	for(int i = 0; i < size; i++)
	{

		if(s[i] != ' ')
		{
			nS[x] = s[i]; 
			x++;
		}
	}
	printf("%s\n", "asdasd");
	// printf("%s\n", nS);
	// return nS;
	return nS;
}

int main()
{
	
	printf("Hello World\n");
	
	printf("Addition: %d\n",x+y);
	printf("Subtraction: %d\n",x-y);
	printf("Multiplication: %d\n",x*y);
	printf("Division: %f\n",x*1.0/y);
	printf("Modulus: %d\n",x%y);
	
	//3--------------------------	
	long long int x1 = -2147483648;
	long long int x2 = -2147483649;
	
	printf("x1 = %lld, x2 = %lld\n",x1,x2);
	int xi1 = (int)x1;
	int xi2 = (int)x2;
	printf("xi1 = %d, xi2 = %d\n",xi1,xi2);
	
	int xii1 = -2147483648;
	int xii2 = -2147483649;
	
	long long int li1 = (int)xii1;
	long long int li2 = (int)xii2;
	printf("li1 = %d, li2 = %d\n",li1,li2);

	//4--------------------------
	addition(1,2);
	printf("%d\n",multiplication(1,1));
	charCodeSum('c','a');

	//5--------------------------
	season s1 = Winter;
	printMessage(s1);

	//6--------------------------
	printf("Octal %#o\n",123456);
	printf("decimal %d\n",456);
	printf("hexa %#x\n",123456);
	
	//7--------------------------
	//a++ will return the value first then increament but ++a will increase then return

	//8--------------------------
	char csmsg[13] = "Hello World!\n";
	for(int i = 0; i < 13; i++)
	{
		printf("%c",csmsg[i]);
	}

	//--------------------------
	//Extra Tasks:
	//length of string
	//remove spaces from string 
	//implement power function

	//using the power of formating of printf
	printf("\nExtra Tasks:\nPower: %d\nString length: %d\nDelete space: %s\n",power(2,3),len("sad"),space("sp ace"));
	//--------------------------
	return 0;

}
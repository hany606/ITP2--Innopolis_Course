#include<stdio.h>
int x = 7;
int y = 5;

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

void space(char s[], char t[])
{
	int size = len(s);
	int nWspace = 0;
	for(int i = 0; i < size; i++)
	{	
		if(s[i] != ' ')
			nWspace++;
	}
	// char nS[size-nWspace];
	int x = 0;
	for(int i = 0; i < size; i++)
	{

		if(s[i] != ' ')
		{
			t[x] = s[i]; 
			x++;
		}
	}
	printf("%s\n", t);
	// printf("%s\n", nS);
	// return nS;
}

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

int main()
{
	printf("%d\n",power(2,3));
	printf("%d\n",len("sad"));
	space("asd sad","asd sad");
	
//	printf("Hello World\n");
//	
//	printf("Addition: %d\n",x+y);x
//	printf("Subtraction: %d\n",x-y);
//	printf("Multiplication: %d\n",x*y);
//	printf("Division: %f\n",x*1.0/y);
//	printf("Modulus: %d\n",x%y);
//	
//	
//	long long int x1 = -2147483648;
//	long long int x2 = -2147483649;
//	
//	int xi1 = (int)x1;
//	int xi2 = (int)x2;
//	printf("xi1 = %d, xi2 = %d\n",xi1,xi2);
//	
//	int xii1 = -2147483648;
//	int xii2 = -2147483649;
//	
//	long long int li1 = (int)xii1;
//	long long int li2 = (int)xii2;
//	printf("li1 = %d, li2 = %d\n",li1,li2);
//	
//	season s1 = Winter;
//	printMessage(s1);
//
//	printf("Octal %o\n",123456);
//	printf("decimal %o\n",123456);
//	printf("hexa %x\n",123456);
//
//
//	char[12] csmsg = "Hello World!";
//	
//	for(int i = 0; i < 11; i++)
//	{
//		printf("%c",csmsg[i]);
//	}

	//length of string
	//remove spaces from string 
	//implement power function
	return 0;

}
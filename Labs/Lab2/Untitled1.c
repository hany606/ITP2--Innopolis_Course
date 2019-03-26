#include<stdio.h>
int x = 7;
int y = 5;

const int BufferSize = 100;
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
	long long int out = 1;
	for(int i = 0; i < n;i++)
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

//modifing the argument itself
void modifyString(char s[])
{
	s[0] = 'm';
}

char* reverse(char s[])
{
	int n = len(s);

	for(int i = 0 ; i < BufferSize; i++)
	{
		nS[i] = ' ';
	}


	for(int i = 0 ; i < n; i++)
	{
		nS[i] = s[n-i-1];
	}
	return nS;
	
}
void mreverse(char s[])
{
	int n = len(s);
	for(int i = 0 ; i < n/2; i++)
	{
		char tmp= s[n-i-1];
		s[n-i-1] = s[i];
		s[i] = tmp; 
	}
	
}

/*
void func(char/int/.. s[])
char s[] means you are passing the pointer to the first element of the array not the array.
It is considered as passing the pointer by value not by reference. (Check it again)

Questions??
Ask the professor
int len = 10;
char s[len];
Why does this work in some compilers and others no??????
-maybe the compiler change the type to const
-maybe store the array in the heap and deal with it as it is dealing with the stack....
*/

void removeSpace(char s[]){
	int n = len(s);
	int j;
	for(int i = 0; i < n; i++)
	{
		if(s[i] == ' ')
		{

			for(j = i; j < n; j++)
				s[j] = s[j+1];	
			// printf("\n space");
			// printf("%s",s);
		}
	}
	s[j] = '\0';

}




int check(char s[])
{
	int size = len(s);
	int n = 0;
	for(int i = 0; ; i++)
	{
		if((s[i] >= 'a' && s[i]<= 'z') || (s[i] >= 'A' && s[i]<= 'Z'))
			n++;
		if(s[i] == '\0')
			break;
	}
	char ns[n];
	int x = 0;
	for(int i = 0; i < size; i++)
	{
		if((s[i] >= 'a' && s[i]<= 'z'))
		{
			ns[x] = s[i];
			x++; 
		}
		if (s[i] >= 'A' && s[i]<= 'Z')
		{
			ns[x] = (char)s[i]-('A'-'a');
			x++;
		}
	}
	char* rs = reverse(ns);

	int flag = 0;
	for(int i = 0; i < n; i++)
	{
		if(rs[i] != rs[n-i-1])
			flag = 1;
	}
	return (1-flag);
}

//detail about the string number of words, char, lines
//use  files, *.txt, wrapper functions -> ?? (maybe as I understand - functions that have other pre implemented function and something extra)
void detail(char s[])
{
	int n = len(s);
	for(int i = 0; i < n; i++)
	{

	}
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
	printf("\nExtra Tasks:\nPower: %d\nString length: %d\nDelete space: %s\n",power(2,0),len("sad"),space("sp ace"));
	//--------------------------
	
	printf("Check = %d", check("pan-Ap"));

	char j[] = "abcd";
	modifyString(j);
	printf("\n%s",j);
	mreverse(j);
	printf("\n%s",j);
	char spaceWord[] = "a b c d e f g";
	removeSpace(spaceWord);
	printf("\n%s",spaceWord);
	printf("z");
	return 0;


}
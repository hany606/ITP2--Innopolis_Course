#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>

enum testEnum {
   fir = 0,
   sec, 
   th = 5
};

enum enumList {a,b,c};

// It is working as the default type of return int
test() {
   return 123;
}

int main(int argc, char **argv) {
   enum enumList l;
   enum testEnum te;
   char stri[12];
   int i = (-5)%3;
   printf("%d\n",i);
   printf("123456\n");
   scanf("%s",&stri);
   printf("line: %s\n",stri);

   /*
   https://cs.nyu.edu/courses/spring05/V22.0201-001/c_tutorial/classes/String.html
   const int StringSize = 10;
   //1
   char s0[StringSize];

   //2
   char* s;
   s = (char*) malloc(strlen(StringSize) * sizeof(char));
   
   //3
   char *s[StringSize]; //array of char pointers

   //4
   char *s = " asdasd" //read only string or we can change the place it points to but we cannot change the content
   
   */
   // The only explanation for that that this s is pointing to a part in the heap not in the stack
   char *s = "Hello";
   int h = 10;
   //But the below will not work
   //*s = 'a';
   
   s = "568asdlkmsalkmsadlkmasdlkmd";
   printf("%s - %d",s,h); 

   printf("\n%6.1f",1235.84651681);
   int a, b, c;

   int arr[] = {1,2,2};
   // This will not give you an error but will give you wrong data that should not be accessed
   int *po = arr;
   // po -= 5;
   printf("\n%d\n",*po);

   // //Will give you an error as this is only read only as it gives a pointer to a place stored the literal nothing else
   // printf("--------------\n");
   // char *ptrMessage = "Hello World\n";
   // // ptrMessage++;
   // *ptrMessage = 'c';
   // printf("%s",ptrMessage);

   // https://stackoverflow.com/questions/3168306/print-text-instead-of-value-from-c-enum
   printf("Enum test\n");
   te = sec;
   //it can compare also by the value of the enum
   if(te == 1)
      printf("sec\n");
   for(int i = fir; i <= th; i++)
   {
      printf("%d",i);
   }

   printf("\n");

   int x = 4;
   if(x%2 == 0)
      if(x < 0)
         printf("neg");
      //if we add else if in this case else will considered for the big if
      // else if(x == 0);
      //    printf("Zero");
      else {
         printf("asdasd]n");
         printf("Pos");
      }
   else
      printf("odd");

      printf("\n");

      // a.exe par1
      printf("%s",argv[0]);
      printf("%c",argv[0][0]);

      printf("%s",*(argv+0));
      printf("%c",*(*(argv+0)));

      int *mi = (int*)malloc(sizeof(int)); // it filled with garbage
      printf("\nMalloc:\n%p\n",mi);
      printf("%d\n-------------\n", *mi);

      signed int *ci = (signed int*)calloc(1,sizeof(int));
      printf("%p\n",ci);
      *ci = INT_MAX;
      printf("%d\n", *ci);
      //over flow not error. because the maximum of the 
      printf("%d\n", ++*ci);
      //No over flow, it casted implicitly to long long int 
      printf("%lld\n", ++*ci);
      printf("%lld\n", 5+*ci);
      printf("-------------\n");
 
      //The factor of the size doesn't matter 1 or 2 as it will cast to int ptr but can we use the rest of the memory 
      //that we reserved?? I think no in the end it will cast to the specific pointer and you only can use the size of the specific pointer
      unsigned int *ci1 = (unsigned int*)calloc(1,2*sizeof(int));
      printf("%p\n",ci1);
      *ci1 = INT_MAX;
      printf("%d\n", ci1);
      printf("%d\n", ++*ci1); //overflow because the implicit casting to signed int
      //over flow not error. because the maximum of the 
      printf("%u\n", ++*ci1);
      //No over flow, it casted implicitly to long long int 
      printf("%lld\n", ++*ci1);
      printf("%lld\n", 5+*ci1);
      printf("-------------\n");

      // The same example of signed int
      // int *ci2 = (int*)calloc(1,sizeof(int));
      // printf("%p\n",ci2);
      // *ci2 = INT_MAX;
      // //over flow as the specifier and the int storage
      // printf("%d\n", ++*ci2);
      // //No over flow, I think that the int pointer casted to long long int pointer 
      // printf("%lld\n", ++*ci2);
      // printf("%lld\n", 5+*ci2);
      // printf("-------------\n");
      // int *ci = (int*)calloc(1,2*sizeof(int));
      // printf("%p\n",ci);
      // *ci = INT_MAX;
      // //over flow as the specifier and the int storage
      // printf("%d\n", ++*ci);
      // //No over flow, I think that the int pointer casted to long long int pointer 
      // printf("%lld\n", ++*ci);
      // printf("%lld\n", 5+*ci);

      int *ci2 = (int*)calloc(1,2*sizeof(int));
      printf("%p\n",ci2);
      //overflow then make it zero
      *ci2 = INT_MAX + -1*INT_MIN;
      printf("%d\n", ++*ci2);
      printf("%lld\n", ++*ci2);
      //No over flow, I think that the int pointer casted to long long int pointer 
      printf("%lld\n", ++*ci2);
      printf("%lld\n", 5+*ci2);
      printf("-------------\n");

      // unsigned int *ci2 = (unsigned int*)calloc(1,1*sizeof(int));
      // printf("%p\n",ci2);
      // //it will not make overflow as it is unsigned
      // *ci2 = INT_MAX + -1*INT_MIN -2;
      // printf("%u\n", ++*ci2);
      // printf("%lld\n", ++*ci2);
      // //No over flow, I think that the int pointer casted to long long int pointer 
      // printf("%lld\n", ++*ci2); //will make overflow here
      // printf("%lld\n", 5+*ci2);
      // printf("-------------\n");

      //it doesn't matter if 2 or 1 as factor as in the end we only can point to the int pointer area
      int *ci3 = (int*)calloc(1,2*sizeof(int));
      printf("%p\n",ci3);
      *ci3 = INT_MAX;
      //over flow as the specifier and the int storage
      printf("%d\n", ++*ci3);
      //No over flow, I think that the value casted to long long int format nothing else (yes)
      printf("%lld\n", ++*ci3);
      printf("%lld\n", 5+*ci3);
      printf("%d\n", INT_MAX+1);
      printf("%lld\n", INT_MAX+1);
      printf("-------------\n");



      // 2*sizeof(int) = sizeof(long long int)
      long long int *ci5 = (long long int*)calloc(1,2*sizeof(int));
      printf("%p\n",ci5);
      //No over flow because the long long int
      *ci5 = INT_MAX;
      printf("%lld\n", ++*ci5);


      free(mi);
      free(ci);
      free(ci1);
      free(ci2);
      free(ci3);
      free(ci5);



   

      int arre[12];
      printf("%d", sizeof(arre)/sizeof(arre[0]));

      int tvar = 55;
      int **dptr;
      int *ptr1;
      ptr1 = &tvar;
      dptr = &ptr1;
      **dptr = 10;
      //Why this was working
      // dptr = &tvar;
      // *dptr = 10;
      printf("\n%d\n",(**dptr));

}



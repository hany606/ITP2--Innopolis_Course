#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main() {
    char stri[12];
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
    char *s[StringSize];
    
    */
   // The only explanation for that that this s is pointing to a part in the heap not in the stack
   char *s = "Hello";
   int h = 10;
   s = "568asdlkmsalkmsadlkmasdlkmd";
   printf("%s - %d",s,h); 

   printf("\n%6.0f",1235.84651681);
   int a, b, c;

   

  

}



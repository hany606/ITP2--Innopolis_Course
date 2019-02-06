#include<stdio.h>
#include<math.h>

//histogram for letters which is taken from a file
//add that it will percentage

int histogram(char s[], int num[])
{
    int count = 0;
    for(int i = 0; i <  26; i++)
    {
        num[i] = 0;
    }
    for(int i = 0; s[i] != '\0'; i++)
    {
        if(!(s[i] >= 'a' && s[i] <= 'z'))
            continue;

        num[(char) s[i]-'a']++;
        count++;
    }

    return count;
}

int main()
{
    char s[] = "abcd e ASc abcdcaaaaaaaaaaaaaaaaaaaaaaaahhhhhhhhhhhhhhhhhhhhhhhhbbbbbbbbbbbbbbbcccccccccceeeeeeeeerrrrrrrrrrrraaaaaz";
    const int MAX_BUFFER = 255;

    int histo [26];    
    float par = 5.0;


    //Not working as pipline file and as file reader??
    // scanf("%s",&s);
    // get-content .\file.txt | .\a.exe

    // FILE *fp;
    // char s[MAX_BUFFER];
    // fp = fopen("/file.txt", "r");
    // fscanf(fp, "%s", s);

    int len = histogram(s,histo);


    //printing the histogram
    for(int i = 0; i < 26; i++)
    {
        printf("%c - %d \t- ",(char) 'a'+i, histo[i]);
        for(int j = 0 ; j < ceil(histo[i]/par); j++)
        {
            printf("*");
        }
        printf("\n");
    }



}
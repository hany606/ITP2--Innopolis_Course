#include<stdio.h>
#include<math.h>
#include<ctype.h>

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

long long fact(int n){
    long long fa = 1;
    for(int i = n; i > 0; i--)
    {
        fa *= i;
    }

    return fa;
    // if(n <= 1)
    //     return 1;
    // return fact(n-1)*fact(n);

}

int strong(int mn, int mx){
    for(int i = mn; i <= mx; i++)
    {
        long long sum = 0;
        int count = 1;
        printf("Number %d:\n",i);
        int dup = i;
        while(dup > 0)
        {
            /*
                123/10 = 3
                120/100 = 2
                100/1000 = 1
                100/1000 = 0.1
                
           */
            //problem: extract digits easy: %10 /10
            int dig = (dup%(long)(pow(10,count)))/(pow(10,count-1));
            printf("%d",dig);
            sum += fact(dig);
            //printf("Finish fact\n");
            dup -= dig*pow(10,count-1);
            printf(":%d, ", dup);
            count++;
        }
        printf("\n");
        if(sum == i)
            return i;
    }
    return mn-1;
}

void toBin(const int cn)
{
    const int size = ceil(log2(cn));
    int arr[size];

    
    int n = cn;


    int x = 0;
    while(n != 0)
    {
        arr[size - x-1] = n%2;
        // printf("%d",n%2);
        n /= 2;
        x++;
    }
    
    for(int i = 0; i < size; i ++)
    {   
        printf("%d",arr[i]);   
    }
    printf("\n");

}


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

void wordsHistogram(char s[]){

    int sum[10];
    int othersLength = 0;
    for(int i = 0; i < 10; i++)
        sum[i] = 0;
    int numAll = len(s);
    int insideWord = 0;
    int wordLen = 0;
    int mxrep = 0;
    for(int i = 0; i < numAll; i++){
        if(isalnum(s[i])){
            wordLen++;
            insideWord = 1;
        }
        else if(s[i] == ' ')
        {
            insideWord = 0;
            if(wordLen <= 10 && wordLen > 0)
                sum[wordLen] ++;
            else
                othersLength++;
            
            wordLen = 0;

        }
        
    }
    if(insideWord == 1)
    {
        if(wordLen <= 10 && wordLen > 0)
                sum[wordLen] ++;
        else
                othersLength++;
    }
    for(int i = 0; i < 10; i++)
    {
        printf("%d: %d\n",sum[i], i);
    }
    printf("Greater than 10 letters: %d\n",othersLength);


    for(int i = 0; i < 10; i++)
        mxrep = fmax(mxrep, sum[i]);
    //printf("\nmx%d\n",mxrep);
        // Vertical histogram representation
    for(int i =0 ;i < 10; i++)
    {
        printf("%d\t",sum[i]);
    }
    printf("\n");

    for(int i = 0 ;i < mxrep; i++)
    {
        //printf("asdasd\n");
        //problem in rep
        for (int j = 0; j <10; j++)
        {
            if(sum[j]-- > 0)
                printf("*\t");
            else 
                printf("\t");
        }
        printf("\n");
    }
    printf("\n");

    printf("Greater than 10 letters: %d\n",othersLength);



}

int main()
{
    char s[] = "abcd e ASc asd iijoun kndjnkjn abcdcaa aaaaaaaaaaaaaaaaaaaaaahhhhhhhhhhhhhhhhhhhhhhhhbbbbbbbbbbbbbbbcccccccccceeeeeeeeerrrrrrrrrrrraaaaaz";
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

    int minRange = 2, maxRange = 101;
    int strongNumber = strong(minRange,maxRange);
    if(strongNumber >= minRange)
        printf("Strong num from %d to %d: %d\n",minRange, maxRange, strongNumber);
    else
        printf("No Strong Number from %d to %d", minRange, maxRange);

    wordsHistogram(s);

    toBin(47);


}
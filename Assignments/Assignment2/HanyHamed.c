#include<stdio.h>
#include<stdlib.h>

#define inputFileNamePrefix "input"
#define outputFileNamePrefix "HanyHamedOutput"
#define fileExtension ".txt"
#define MAXBUFFER 50

// enum TokenFlag{
//     Space,
//     NewLine,
//     EndOfFile
// }globalReadTokenFlag;


int readToken(char *token,int maxBuffer,FILE* fptr){
    char tmp;
    int counter = 0;
    int flag = 0;
    /**
     * flag = 1 -> space
     *      = 2 -> newLine
     *      = 3 -> EOF
     **/
    while(counter < maxBuffer){
        tmp = fgetc(fptr);
        if(tmp == ' ')
            flag = 1;
        
        else if(tmp == '\n')
            flag = 2;
        
        else if(tmp == EOF)
            flag = 3;
        
        if(flag > 0)
           return flag;
        
        *(token+counter) = tmp;
        counter++;
    }
}

int main(){
    for(int i = 1 ; i  <= 50; i++){
        char inputFileName[12];
        char outputFileName[21];
        sprintf(inputFileName,"%s%d%s",inputFileNamePrefix,i,fileExtension);
        sprintf(outputFileName,"%s%d%s",outputFileNamePrefix,i,fileExtension);

        FILE *inputFileptr = fopen(inputFileName,"r");
        if(inputFileptr == NULL)
            break;

        FILE *outputFileptr = fopen(outputFileName,"w");

        // printf("%s\t%s\n", inputFileName,outputFileName);

        //Take the data from the File
        /**
         * CoursesName #req.Labs #students 
         * P
         * Prof.Name [Trained Courses]
         * T
         * TA.Name [Trained Courses]
         * S
         * Student_1stName 2ndName ID [Required Courses]
         * 
         **/
        
        //Read courses
        printf("\n##### Start Reading Courses #####\n");
        for(int x = 0; ; x++){
            char *courseName = (char*) calloc(1, MAXBUFFER);
            char *tmp = (char*) calloc(1, MAXBUFFER);
            int numberRequiredLabs;
            int numberStudents;            
            int state;

            state = readToken(courseName,MAXBUFFER,inputFileptr);
            if(courseName[0] == 'P' && courseName[1] == '\0'){
                free(courseName);
                free(tmp);
                break;
            }
            state = readToken(tmp,MAXBUFFER,inputFileptr);
            numberRequiredLabs = atoi(tmp);
            state = readToken(tmp,MAXBUFFER,inputFileptr);
            numberStudents = atoi(tmp);

            printf("\n%d. %s %d %d\n", x+1, courseName, numberRequiredLabs, numberStudents);

        }
        printf("\n##### End Reading Courses #####\n");

        printf("\n##### Start Reading Professors #####\n");
        for(int x = 0; ; x++){
            
            char *professorFirstName = (char*) calloc(1, MAXBUFFER);
            char *professorLastName  = (char*) calloc(1, MAXBUFFER);
            int state;

            state = readToken(professorFirstName,MAXBUFFER,inputFileptr);
            if(professorFirstName[0] == 'T' && professorFirstName[1] == '\0'){
                free(professorFirstName);
                free(professorLastName);
                break;
            }
            state = readToken(professorLastName,MAXBUFFER,inputFileptr);

            printf("\n%d. %s %s", x+1, professorFirstName, professorLastName);
            
            while(1){
                char *trainedCourse = (char*) calloc(1, MAXBUFFER);           
                state = readToken(trainedCourse,MAXBUFFER,inputFileptr);
                //New line
                printf(" %s", trainedCourse);
                free(trainedCourse);
                if(state == 2)
                    break;
            }
            
            printf("\n");

        
        }
        printf("\n##### End Reading Professors #####\n");

        printf("\n##### Start Reading TAs #####\n");
        for(int x = 0; ; x++){
            
            char *TAFirstName = (char*) calloc(1, MAXBUFFER);
            char *TALastName  = (char*) calloc(1, MAXBUFFER);
            int state;

            state = readToken(TAFirstName,MAXBUFFER,inputFileptr);
            if(TAFirstName[0] == 'S' && TAFirstName[1] == '\0'){
                free(TAFirstName);
                free(TALastName);
                break;
            }
            state = readToken(TALastName,MAXBUFFER,inputFileptr);

            printf("\n%d. %s %s", x+1, TAFirstName, TALastName);
            
            while(1){
                char *trainedCourse = (char*) calloc(1, MAXBUFFER);           
                state = readToken(trainedCourse,MAXBUFFER,inputFileptr);
                //New line
                printf(" %s", trainedCourse);
                free(trainedCourse);
                if(state == 2)
                    break;
            }
            
            printf("\n");

        
        }
        printf("\n##### End Reading TAs #####\n");

        printf("\n##### Start Reading Students #####\n");
        for(int x = 0; ; x++){
            
            char *studentFirstName = (char*) calloc(1, MAXBUFFER);
            char *studentLastName  = (char*) calloc(1, MAXBUFFER);
            char *studentID        = (char*) calloc(1, MAXBUFFER);
            
            int state;

            state = readToken(studentFirstName,MAXBUFFER,inputFileptr);
            state = readToken(studentLastName,MAXBUFFER,inputFileptr);
            state = readToken(studentID,MAXBUFFER,inputFileptr);

            printf("\n%d. %s %s %s", x+1, studentFirstName, studentLastName, studentID);
            
            while(1){
                char *trainedCourse = (char*) calloc(1, MAXBUFFER);           
                state = readToken(trainedCourse,MAXBUFFER,inputFileptr);
                //New line
                printf(" %s", trainedCourse);
                free(trainedCourse);
                if(state == 2 || state == 3)
                    break;
            }
            printf("\n");
            if(state == 3){
                free(studentFirstName);
                free(studentLastName);
                free(studentID);
                break;
            }
        
        }
        printf("\n##### End Reading Students #####\n");

        fclose(inputFileptr);
        fclose(outputFileptr);
    }

    FILE *endFileptr = fopen("h.hamed@innopolis.university.txt","w");
    fclose(endFileptr);
}
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

typedef struct courseStruct
{
    int runFlag:1;
    char *name;
    int numLabs;
    int numStudents;

} Course;

typedef struct professorStruct
{
    int numTrainedCourses;
    int numTakenCourses;
    char *firstName;
    char *lastName;
    char **courses;
} Professor;

typedef struct TAStruct
{
    int numTrainedCourses;
    int numTakenCourses;
    char *firstName;
    char *lastName;
    char **courses;
} TA;

typedef struct studentStruct
{
    int numRequiredCourses;
    int numTakenCourses;
    char *firstName;
    char *lastName;
    char *id;
    char **courses;
} Student;



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
        
        if(flag > 0){
            // *(token+counter) = '\0';    
            return flag;
        }        
        *(token+counter) = tmp;
        counter++;
    }
}

void setCourse(Course *cptr, char name[], int numReqLabs, int numStudents){
    cptr->name = name;
    cptr->numLabs = numReqLabs;
    cptr->numStudents = numStudents;
}

void setProfessor(Professor *pptr, char fName[], char lName[], char **courses, int nC){
    pptr->courses = courses;
    pptr->firstName = fName;
    pptr->lastName = lName;
    pptr->numTakenCourses = 0;
    pptr->numTrainedCourses = nC;
}

void setTA(TA *tptr, char fName[], char lName[] , char **courses, int nC){
    tptr->courses = courses;
    tptr->firstName = fName;
    tptr->lastName = lName;
    tptr->numTakenCourses = 0;
    tptr->numTrainedCourses = nC;
}

void setStudent(Student *sptr, char fName[], char lName[], char id[], char **courses, int nC){
    sptr->courses = courses;
    sptr->firstName = fName;
    sptr->lastName = lName;
    sptr->numTakenCourses = 0;
    sptr->numRequiredCourses = nC;
}

int main(){

    for(int i = 1 ; i  <= 50; i++){
        char inputFileName[12];
        char outputFileName[21];

        int numCourses = 0;
        int numProf = 0;
        int numTA = 0;
        int numStudents = 0;

        Course *coursesList = (Course*) calloc(numCourses+1, sizeof(Course));
        Professor *professorsList = (Professor*) calloc(numProf+1, sizeof(Professor));
        TA *TAsList = (TA*) calloc(numTA+1, sizeof(TA));
        Student *studentsList = (Student*) calloc(numStudents+1, sizeof(Student));


        sprintf(inputFileName,"%s%d%s",inputFileNamePrefix,i,fileExtension);
        sprintf(outputFileName,"%s%d%s",outputFileNamePrefix,i,fileExtension);

        FILE *inputFileptr = fopen(inputFileName,"r");
        if(inputFileptr == NULL){
            sprintf(inputFileName,"%s%d%s",inputFileNamePrefix,i+1,fileExtension);
            FILE *inputFileptr = fopen(inputFileName,"r");
            if(inputFileptr == NULL)
                break;

            else{
                FILE *outputFileptr = fopen(outputFileName,"w");
                fprintf(outputFileptr,"%s","Invalid input.");
                // fclose(inputFileptr);
                fclose(outputFileptr);
                continue;
            }
        }
            
        FILE *outputFileptr = fopen(outputFileName,"w");

        // printf("%s\t%s\n", inputFileName,outputFileName);

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
                // free(courseName);
                // free(tmp);
                break;
            }
            state = readToken(tmp,MAXBUFFER,inputFileptr);
            numberRequiredLabs = atoi(tmp);
            state = readToken(tmp,MAXBUFFER,inputFileptr);
            numberStudents = atoi(tmp);

            printf("\n%d. %s %d %d\n", x+1, courseName, numberRequiredLabs, numberStudents);
            setCourse((coursesList+numCourses),courseName, numberRequiredLabs, numberStudents);
            coursesList = (Course*) realloc(coursesList, ((++numCourses)+1)*sizeof(Course));

        }
        printf("\n##### End Reading Courses #####\n");

        printf("\n##### Start Reading Professors #####\n");
        for(int x = 0; ; x++){

            int numCourses = 0;
            char *professorFirstName = (char*) calloc(1, MAXBUFFER);
            char *professorLastName  = (char*) calloc(1, MAXBUFFER);
            char **courses = (char**) calloc(numCourses+1,sizeof(char*));

            int state;

            state = readToken(professorFirstName,MAXBUFFER,inputFileptr);

            if(professorFirstName[0] == 'T' && professorFirstName[1] == '\0'){
                // free(professorFirstName);
                // free(professorLastName);
                break;
            }
            state = readToken(professorLastName,MAXBUFFER,inputFileptr);
            printf("\n%d. %s %s", x+1, professorFirstName, professorLastName);

            while(1){
                printf("\n 651516 %s",professorLastName);
                                break;

                state = readToken(*(courses)+numCourses),MAXBUFFER,inputFileptr);
                //                 printf("\n 651516 %s",professorLastName);

                // //New line
                // printf(" %s",*(courses+numCourses));
                if(state == 2)
                    break;
                
                courses = (char**) realloc(courses, ((++numCourses)+1)*sizeof(char*));        
            }
            break;
            printf("\n");
            setProfessor((professorsList+numProf),professorFirstName, professorLastName,courses,numCourses);
            professorsList = (Professor*) realloc(professorsList, ((++numProf)+1)*sizeof(Professor));

            // free(courses);        
        }
        printf("\n##### End Reading Professors #####\n");

        // printf("\n##### Start Reading TAs #####\n");
        // for(int x = 0; ; x++){
            
        //     int numCourses = 0;
        //     char *TAFirstName = (char*) calloc(1, MAXBUFFER);
        //     char *TALastName  = (char*) calloc(1, MAXBUFFER);
        //     char **courses = (char**) calloc(numCourses+1,sizeof(char*));

        //     int state;

        //     state = readToken(TAFirstName,MAXBUFFER,inputFileptr);
        //     if(TAFirstName[0] == 'S' && TAFirstName[1] == '\0'){
        //         // free(TAFirstName);
        //         // free(TALastName);
        //         break;
        //     }
        //     state = readToken(TALastName,MAXBUFFER,inputFileptr);

        //     printf("\n%d. %s %s", x+1, TAFirstName, TALastName);
            
        //     while(1){
        //         state = readToken(*(courses+numCourses),MAXBUFFER,inputFileptr);
        //         //New line
        //         printf(" %s",*(courses+numCourses));
        //         if(state == 2)
        //             break;
        //         courses = (char**) realloc(courses, ((++numCourses)+1)*sizeof(char*));
        //     }
            
        //     printf("\n");
        //     setTA((TAsList+numTA),TAFirstName, TALastName,courses,numCourses);
        //     TAsList = (TA*) realloc(TAsList, ((++numTA)+1)*sizeof(TA));
        //     // free(courses); 

        
        // }
        // printf("\n##### End Reading TAs #####\n");

        // printf("\n##### Start Reading Students #####\n");
        // for(int x = 0; ; x++){

        //     int numCourses = 0;
        //     char *studentFirstName = (char*) calloc(1, MAXBUFFER);
        //     char *studentLastName  = (char*) calloc(1, MAXBUFFER);
        //     char *studentID        = (char*) calloc(1, MAXBUFFER);
        //     char **courses = (char**) calloc(numCourses+1,sizeof(char*));

            
        //     int state;

        //     state = readToken(studentFirstName,MAXBUFFER,inputFileptr);
        //     state = readToken(studentLastName,MAXBUFFER,inputFileptr);
        //     state = readToken(studentID,MAXBUFFER,inputFileptr);

        //     printf("\n%d. %s %s %s", x+1, studentFirstName, studentLastName, studentID);
            
        //     while(1){
        //         state = readToken(*(courses+numCourses),MAXBUFFER,inputFileptr);
        //         //New line
        //         printf(" %s",*(courses+numCourses));
        //         // free(trainedCourse);
        //         if(state == 2 || state == 3)
        //             break;
        //         courses = (char**) realloc(courses, ((++numCourses)+1)*sizeof(char*));
        //     }
        //     printf("\n");
        //     setStudent((studentsList+numStudents),studentFirstName, studentLastName, studentID, courses, numCourses);
        //     studentsList = (Student*) realloc(studentsList, ((++numStudents)+1)*sizeof(Student));

        //     if(state == 3){
        //         // free(studentFirstName);
        //         // free(studentLastName);
        //         // free(studentID);
        //         break;
        //     }
        
        // }
        // printf("\n##### End Reading Students #####\n");

        fclose(inputFileptr);
        fclose(outputFileptr);
    }

    FILE *endFileptr = fopen("h.hamed@innopolis.university.txt","w");
    fclose(endFileptr);
}
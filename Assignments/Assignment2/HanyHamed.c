#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>


#define inputFileNamePrefix "input"
#define outputFileNamePrefix "HanyHamedOutput"
#define fileExtension ".txt"
#define MAXBUFFER 50

int numCourses = 0;
int numProf = 0;
int numTA = 0;
int numStudents = 0;
unsigned long long int mnTotalPoints = ULLONG_MAX;
Schedule bestSchedule;

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

typedef struct courseStruct
{
    int runFlag:1;
    char *name;
    int numLabs;
    int numStudents;
    Professor professor;
    TA* TAs;
    Student* students;
} Course;

typedef struct badnessStruct
{
    int type;
    Professor professor;
    TA ta;
    Student student;
    int num;
    Course course;
}Badness;


typedef struct scheduleStruct
{
    int numCourses;
    int numBadness;
    Course *courses;
    Badness *badness;
    unsigned long long int totalPoints;
}Schedule;




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


void solve(Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList,unsigned long long int points){
    int badnessPoints = points;
    //local schedule
    if(points > mnTotalPoints){
        return;
    }

    if(sCoursesList == NULL){
        //compare with the global minimum and store the minimum
    }
    //Check if the course can run or not: Is there enough professors
    //Select a professor who are able to teach this course
    //loop over the professors
    int flagProf = 0;
    for(int p = 0; p < numProf; p++){
        int state = 0;
        Professor *professor = (sProfessorsList+p);
        //if he has two classes, continue
        if(professor->numTakenCourses == 2)
            continue;
        //Check if he is trained or not
        int trainedProfessor = 0;
        for(int j = 0; j < professor->numTrainedCourses; j++){
            if(strcmp(professor->courses[j],*sCoursesList->name) == 0){    //trained professor
                trainedProfessor = 1;
                break;   
            }
        }
        //if he has one class, he will teach this course if he is trained no badness points
        //if he has zero class, he will teach this course in both ways (trained, untrained) but with different badness points
            //if trained, no badness points
            //if untrained, 5 points
            
        //if trained professor and can take the class have 0 or 1 courses
        if(trainedProfessor == 1){
            professor->numTakenCourses++;
            flagProf = 1;
        }
        
        //he is not trained
        else{
            //He has already a class and he cannot take this class as he is not trained
            if(professor->numTakenCourses == 1)
                continue;
            //He has zero class
            else{
                professor->numTakenCourses += 2;
                badnessPoints += 5;
                flagProf = 1;
                //Badness, memo it, increase the size of badness
            }
        }
        if(flagProf == 0)
            break;
      
        //Prof will teach the course (There is no difference in the procedure to teach except the badness poitns)
        //Select the TAs
        //loop over TAs
        //Check if the course can run or not
        int flagTA = 0;
        int taLabs = 0;
        for(int t = 0; t < numTA; t++){
            TA *ta = (sTAsList+t);
            if(ta->numTakenCourses == 4){
                continue;
            }
            for(int j = 0; j < ta->numTrainedCourses; j++){
                if(strcmp(ta->courses[j],sCoursesList->name) == 0){
                    taLabs += 4-(ta->numTakenCourses);
                    break;
                }
            }
            if(sCoursesList->numLabs <= taLabs){
                flagTA = 1;
                break;
            }
        }
        //If available TAs:
        if(flagTA == 1){
            //Get combination of allowed TAs
            for(int nLabs = 0; nLabs < sCoursesList->numLabs; nLabs++){
                for(int t = 0; t < numTA; t++){
                    TA *ta = (sTAsList+t);
                    //get an allowed TA
                    int allowed = 0;
                    for(int j = 0; j < ta->numTrainedCourses; j++){
                        if(strcmp(ta->courses[j],sCoursesList->name) == 0){
                            allowed = 1;
                        }
                    }
                    if(allowed == 0)
                        continue;
                    for(int tt = 1; tt <= (4-ta->numTakenCourses); tt++){
                        //will take tt from this TA to fill the labs and the rest for other TAs
                        //we need to choose the other TAs
                        
                    }
                }
                //Select the students
                //Get combinination of allowed Students
                //calc
                //select other course
                solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,badnessPoints);
            }
            
        }
        
        //If not available TAs, there is no possibility to run the course, whoever the professor
        else
            break;
    }

    //don't forget to clear what you have changed in the professors, TAs, Students
    //badness points, memo it, increase the size of badness
    solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,points+20);    //will not run the course
}

int main(){

    for(int i = 1 ; i  <= 50; i++){
        char inputFileName[12];
        char outputFileName[21];

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
            char **courses = (char**) calloc(1,sizeof(char **));

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
                *(courses+numCourses) = (char*) calloc(MAXBUFFER,sizeof(char *));
                state = readToken(courses[numCourses],MAXBUFFER,inputFileptr);

                if(state == 2)
                    break;
                
                courses = (char**) realloc(courses, ((++numCourses)+1)*sizeof(char*));
        
            }
            printf("\n");
            // for(int j = 0 ; j < numCourses+1; j++){
            //     printf("Course: %s\n",courses[j]);
            // }
            setProfessor((professorsList+numProf),professorFirstName, professorLastName,courses,numCourses+1);
            professorsList = (Professor*) realloc(professorsList, ((++numProf)+1)*sizeof(Professor));

            // free(courses);        
        }
        printf("\n##### End Reading Professors #####\n");

        printf("\n##### Start Reading TAs #####\n");
        for(int x = 0; ; x++){
            
            int numCourses = 0;
            char *TAFirstName = (char*) calloc(1, MAXBUFFER);
            char *TALastName  = (char*) calloc(1, MAXBUFFER);
            char **courses = (char**) calloc(numCourses+1,sizeof(char*));

            int state;

            state = readToken(TAFirstName,MAXBUFFER,inputFileptr);
            if(TAFirstName[0] == 'S' && TAFirstName[1] == '\0'){
                // free(TAFirstName);
                // free(TALastName);
                break;
            }
            state = readToken(TALastName,MAXBUFFER,inputFileptr);

            printf("\n%d. %s %s", x+1, TAFirstName, TALastName);
            
            while(1){
                *(courses+numCourses) = (char*) calloc(MAXBUFFER,sizeof(char *));
                state = readToken(*(courses+numCourses),MAXBUFFER,inputFileptr);
                //New line
                printf(" %s",*(courses+numCourses));
                if(state == 2)
                    break;
                courses = (char**) realloc(courses, ((++numCourses)+1)*sizeof(char*));
            }
            
            printf("\n");
            setTA((TAsList+numTA),TAFirstName, TALastName,courses,numCourses+1);
            TAsList = (TA*) realloc(TAsList, ((++numTA)+1)*sizeof(TA));
            // free(courses); 

        
        }
        printf("\n##### End Reading TAs #####\n");

        printf("\n##### Start Reading Students #####\n");
        for(int x = 0; ; x++){

            int numCourses = 0;
            char *studentFirstName = (char*) calloc(1, MAXBUFFER);
            char *studentLastName  = (char*) calloc(1, MAXBUFFER);
            char *studentID        = (char*) calloc(1, MAXBUFFER);
            char **courses = (char**) calloc(numCourses+1,sizeof(char*));

            
            int state;

            state = readToken(studentFirstName,MAXBUFFER,inputFileptr);
            state = readToken(studentLastName,MAXBUFFER,inputFileptr);
            state = readToken(studentID,MAXBUFFER,inputFileptr);

            printf("\n%d. %s %s %s", x+1, studentFirstName, studentLastName, studentID);
            
            while(1){
                *(courses+numCourses) = (char*) calloc(MAXBUFFER,sizeof(char *));
                state = readToken(*(courses+numCourses),MAXBUFFER,inputFileptr);
                //New line
                printf(" %s",*(courses+numCourses));
                // free(trainedCourse);
                if(state == 2 || state == 3)
                    break;
                courses = (char**) realloc(courses, ((++numCourses)+1)*sizeof(char*));
            }
            printf("\n");
            setStudent((studentsList+numStudents),studentFirstName, studentLastName, studentID, courses, numCourses+1);
            studentsList = (Student*) realloc(studentsList, ((++numStudents)+1)*sizeof(Student));

            if(state == 3){
                // free(studentFirstName);
                // free(studentLastName);
                // free(studentID);
                break;
            }
        
        }
        printf("\n##### End Reading Students #####\n");

        // //Print lists in order to ensure that they are correct
        // for(int x = 0 ; x < numCourses; x++){
        //     printf("%s\n",(coursesList+x)->name);
        // }

        // for(int x = 0 ; x < numProf; x++){
        //     printf("^^^%s\n", (professorsList+x)->firstName);
        //     for(int j = 0; j < (professorsList+x)->numTrainedCourses; j++){
        //       printf("%s\n",(professorsList+x)->courses[j]);

        //     }
        // }
        // for(int x = 0 ; x < numTA; x++){
        //     printf("%s\n",(TAsList+x)->firstName);
        // }

        // for(int x = 0 ; x < numStudents; x++){
        //     printf("%s\n",(studentsList+x)->firstName);
        // }
        
        printf("\n##### Start Logic #####\n");


        //Allocate memory for courses and badness
        // =
        solve(coursesList, professorsList, TAsList, studentsList, 0);
        // schedule.courses = (Course*) calloc(schedule.numCourses, sizeof(Course));
        // schedule.badness = (Badness*) calloc(schedule.numBadness, sizeof(Badness));


        // for(int c = 0; c < numCourses; c++){

        //     Course *course = (coursesList+c);
        //     printf("\nCourse: %s\n",course->name);
        //     int flagProf = 0;

            
        //     for(int p = 0; p < numProf; p++){
        //         Professor *professor = (professorsList+p);
        //         printf("\nProf: %s\n",professor->firstName);

        //         if(professor->numTakenCourses < 2){    //Then this professor is valid for the course
        //             flagProf = 1;
        //             //two possiblities -> has already one course (give him trained) or zero(two possiblities, give him untrained or give him two trained)
        //             if(professor->numTakenCourses == 0){
        //                 //2 possibilities give him 1 untrained or give him two trained
        //                 professor->numTakenCourses += 2;
        //                 //get TAs for the course
                        


        //                 //get students
        //                 //Go to other courses
        //                 //if it is the last
        //                 //calc

        //                 //clear
        //                 professor->numTakenCourses -= 2;
        //             }
        //             //possiblity to take two trained courses
        //             int trainedProfessor = 0;
        //             for(int j = 0; j < professor->numTrainedCourses; j++){
        //                 if(strcmp(professor->courses[j],course->name) == 0){    //trained professor
        //                     trainedProfessor = 1;
        //                     break;   
        //                 }
        //             }
        //             if(trainedProfessor == 1){
        //                 professor->numTakenCourses++;
        //                 //get TAs
        //                 //get Students
        //                 //Go to other courses
        //                 //if it is the last
        //                 //calc
        //                 //clear
        //                 professor->numTakenCourses -= 1;
        //             }
        //         }
        //     }
        //     if(flagProf == 0){
        //         ///Can't run the course because of there is no available professors
        //         //badness
        //         //calculate
        //         //move on to the next course, to check it
        //         continue;
        //     }

        //     free(schedule.courses);
        //     free(schedule.badness);
        // }

        printf("\n##### End Logic #####\n");


        free(coursesList);
        free(professorsList);
        free(TAsList);
        free(studentsList);
        fclose(inputFileptr);
        fclose(outputFileptr);
    }

    FILE *endFileptr = fopen("h.hamed@innopolis.university.txt","w");
    fclose(endFileptr);
}
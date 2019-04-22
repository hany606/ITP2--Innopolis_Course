#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include<math.h>


#define inputFileNamePrefix "input"
#define outputFileNamePrefix "HanyHamedOutput"
#define fileExtension ".txt"
#define MAXBUFFER 50

int numCourses = 0;
int numProf = 0;
int numTA = 0;
int numStudents = 0;
unsigned long long int mnTotalPoints = ULLONG_MAX;

#define Min(x,y) (x >= y ? y : x)


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
    Professor *professor;
    TA *TAs;
    Student *students;
} Course;

typedef struct badnessStruct
{
    int type;
    Professor *professor;       //pointer to the professor
    TA *ta;                     //pointer to the TA
    Student *student;           //pointer to the student
    int num;
    Course *course;             //pointer to the course 
}Badness;


typedef struct scheduleStruct
{
    int numCourses;
    int numBadness;
    Course **courses;       //array of pointers for the courses
    Badness *badness;       //array of badness
    unsigned long long int totalPoints;
}Schedule;



Schedule bestSchedule;
void solve(Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList,unsigned long long int points, Schedule *schedule);


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


void combinationStudents(Student *st, Student *combStudent, int index, int r, int start, int end, Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList,unsigned long long int points, Schedule *schedule){
    if(index == r){
        //Get all combinination of allowed Students
        //select other course
        // schedule->courses->students = (Student*) calloc(sCoursesList->numStudents,sizeof(Student));
        sCoursesList->students = (Student*) calloc(sCoursesList->numStudents,sizeof(Student));
        // printf("\n=ST==\n\t");
        for(int i = 0; i < r; i++){
            // schedule->courses->students[i] = combStudent[i];
            sCoursesList->students[i] = combStudent[i];
            // printf("%s/",combStudent[i].firstName);
        }
        // printf("\n");
        solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,points,schedule);
        // free(sCoursesList->TAs);
        return;
    }
    for(int i = start; i <= end && end-i+1 >= r-index; i++){
        *(combStudent+index) = *(st+i);
        combinationStudents(st, combStudent, index+1,r, i+1, end, sCoursesList, sProfessorsList, sTAsList, sStudentsList, points,schedule);
    }


}

void combinationTAs(TA *tas, TA *combTA, int index, int r,int start, int end,Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList,unsigned long long int points, Schedule *schedule){
    if (index == r)  
    {  
        // schedule->courses->TAs = (TA*) calloc(r,sizeof(TA));
        sCoursesList->TAs = (TA*) calloc(r,sizeof(TA));

        //Select the allowed students
        // printf("\n==TA==\n\t");
        for(int i = 0; i < r; i++){
            // schedule->courses->TAs[i] = combTA[i];
            sCoursesList->TAs[i] = combTA[i];
            // printf("%s/",combTA[i].firstName);
        }
        // printf("\n");

        int numavailableStudents = 0;
        Student *availableStudents = (Student*) calloc(numStudents, sizeof(Student));

        for(int s = 0; s < numStudents; s++){
            Student *st = (sStudentsList+s);
            for(int j = 0; j < st->numRequiredCourses; j++){
                if(strcmp(st->courses[j],sCoursesList->name) == 0){
                    *(availableStudents+numavailableStudents++) = *st;
                    break;
                }
            }
        }
        //get all combinations of Students
        Student *local_Students = (Student*) calloc(sCoursesList->numStudents,sizeof(Student));
        int tmpav = Min(sCoursesList->numStudents,numavailableStudents);
        combinationStudents(availableStudents,local_Students,0,tmpav,0,numavailableStudents-1,sCoursesList,sProfessorsList,sTAsList,sStudentsList,points,schedule);
        // free(sCoursesList->TAs);
        return;
    }
    // printf("%d %d %d %d", index, r, start, end);
    for(int i = start; i <= end && end-i+1 >= r-index; i++){
        // printf("//\n");
        *(combTA+index) = *(tas+i);
        combinationTAs(tas, combTA, index+1,r, i+1, end, sCoursesList, sProfessorsList, sTAsList, sStudentsList, points,schedule);
    }


}

void printSchedule(Schedule *schedule){

    printf("#######################################\n");
        printf("Num of courses%d\n",schedule->numCourses);

        for(int i = 0; i < schedule->numCourses; i++){
            printf("Course: %s \nProf. %s\nTAs:\n",schedule->courses[i]->name, (schedule->courses[i]->professor->firstName));

        //     for(int j = 0; j < (schedule->courses[i])->numLabs; j++ )
        //     {
        //         printf("\t%s",schedule->courses[i]->TAs[j].firstName);
        //     }
        //     printf("\nStudents:\n");
        //     for(int j = 0; j < (schedule->courses[i])->numStudents; j++ )
        //     {
        //         printf("\t%s",schedule->courses[i]->students[j].firstName);
        //     }
            printf("\n");
        }

        // printf("Num of badness: %d\n",schedule->numBadness);
        // for(int j = 0; j < schedule->numBadness; j++){
        //     if(schedule->badness[j].type == 1)
        //         printf("Badness#%d: Type%d: Course:%s\n", j+1, schedule->badness[j].type,schedule->badness[j].course->name);

        //     else if(schedule->badness[j].type == 3)
        //         printf("Badness#%d: Type%d: Prof:%s: Course:%s\n", j+1, schedule->badness[j].type,schedule->badness[j].professor->firstName,schedule->badness[j].course->name);
        // }   
        
        printf("#######################################\n");
}

void clean(Schedule *s, Schedule *cs, Course *c, Course *cc, Professor *p, Professor *cp, TA *t, TA *ct, Student *st, Student *cst){
    if(s != NULL && cs != NULL){
        // Schedule *tmp = s;
        // s = (Schedule*) realloc(tmp,sizeof(Schedule));
        memcpy(s,cs,sizeof(Schedule));
    }
    if(c != NULL && cc != NULL){
        // Course *tmp = c;
        // c = (Course*) realloc(tmp,sizeof(Course));      
        memcpy(c,cc,sizeof(Course));
    }
    if(p != NULL && cp != NULL){
        // Professor *tmp = p;
        // p = (Professor*) realloc(tmp,sizeof(Professor));
        memcpy(p,cp,sizeof(Professor));
    }
    if(t != NULL && ct != NULL){    
        // TA *tmp = t;
        // t = (TA*) realloc(tmp,sizeof(TA));
        memcpy(t,ct,sizeof(TA));
    }
    if(st != NULL && cst != NULL){
        // Student *tmp = st;
        // st = (Student*) realloc(tmp,sizeof(Student));
        memcpy(st,cst,sizeof(Student));
    }
    free(cs);
    free(cc);
    free(cp);

}


void solve(Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList,unsigned long long int points,Schedule *schedule){
    printSchedule(schedule);
    printf("------------------------------------------------------------------\n");

    // Course *local_course = (Course*) calloc(numCourses, sizeof(Course));
    // memcpy(local_course,sCoursesList, sizeof(Course*));
    // // printf("%d", res);
    // local_course->name = "asd";

    int badnessPoints = points;
    //local schedule
    if(points > mnTotalPoints){
        return;
    }
    if(sCoursesList->name == NULL){
        //compare with the global minimum and store the minimum
        //calc
        printf("calc -> Leaf\n");
        printf("END branch\n");

        printSchedule(schedule);
        return;
    }
    // printf("^%s\n",sCoursesList->name);
    //Check if the course can run or not: Is there enough professors
    //Select a professor who are able to teach this course
    //loop over the professors
    int flagProf = 0;
    for(int p = 0; p < numProf; p++){
        Professor *professor = (sProfessorsList+p);
        Professor *profCpy = (Professor*) calloc(1,sizeof(Professor));
        Course *courseCpy = (Course*) calloc(1,sizeof(Course));
        Schedule *scheduleCpy = (Schedule*) calloc(1,sizeof(Schedule));

        memcpy(scheduleCpy,schedule,sizeof(Schedule));
        memcpy(courseCpy,sCoursesList,sizeof(Course));
        memcpy(profCpy,professor,sizeof(Professor));


        // printf("ooasm11111dopasd\n");
        // Badness *assd = (Badness*) calloc(1,sizeof(Badness));
        // assd->type=12;
        // printf("%d",assd->type);
        // printf("ooasm11111dopasd\n");

        // Badness *badnesscpy = (Badness*) calloc(1,sizeof(Badness));
        // printf("56168681616\n");
        // memcpy(badnesscpy,schedule->badness,sizeof(Badness));
        // printf("ooasmdopasd\n");
    

        //if he has two classes, continue
        if(professor->numTakenCourses == 2){
            printf("%s has two classes\n",professor->firstName);
            // clean(NULL, NULL, sCoursesList,courseCpy,professor,profCpy, NULL, NULL,NULL,NULL);
            continue;
        }
        //Check if he is trained or not
        int trainedProfessor = 0;
        // printf("\t%s\n",professor->firstName);
        for(int j = 0; j < professor->numTrainedCourses; j++){
            if(strcmp(professor->courses[j],sCoursesList->name) == 0){    //trained professor
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
            printf("+1%s\n",professor->firstName);
        }
        
        //he is not trained
        else{
            //He has already a class and he cannot take this class as he is not trained
            if(professor->numTakenCourses == 1){
                printf("%s has 1 classes and untrained\n",professor->firstName);
                // clean(schedule, scheduleCpy, sCoursesList,courseCpy,professor,profCpy, NULL, NULL,NULL,NULL);
                // printf("asdasdasdasdasfosdmgo,\n");
                // printSchedule(schedule);
                continue;
            }   
            //He has zero class
            else{
                professor->numTakenCourses += 2;
                badnessPoints += 5;
                flagProf = 2;
                printf("+2%s\n",professor->firstName);
                //Badness, memo it, increase the size of badness
                // badnessPoints +=
                schedule->totalPoints +=5;

                // int num = schedule->numBadness;
                // Badness *bdc = schedule->badness;

                // schedule->badness = (Badness*) realloc(bdc,(num+1)*sizeof(Badness));
                printf("lhdsfiundszdoijndsfc\n");
                // schedule->badness[num].professor = professor;
                // schedule->badness[num].type = 3;
                // schedule->badness[num].course = sCoursesList;
                // schedule->totalPoints += 5;
                // schedule->numBadness++;
            }
        }

        sCoursesList->professor = professor;
        // sCoursesList->professor = professor;
        int num = schedule->numCourses;
        // schedule->courses = (Course**) realloc(schedule->courses,(num+1)*sizeof(Course*));


        // printf("%s\n",sCoursesList->name);
        schedule->courses[num] = sCoursesList;

        // printf("%s\n",schedule->courses[0].name);
        schedule->numCourses++;
        // printf("asdas%d\n",schedule->numCourses);

        // printf("%d\n",schedule->numCourses);
        // schedule->courses[schedule->numCourses-1]->name = "65";
        // printf("%s:%s\n",schedule->courses[schedule->numCourses-1]->name, sCoursesList->name);

        //Prof will teach the course (There is no difference in the procedure to teach except the badness poitns)
        //Select the TAs
        //loop over TAs
        //Check if the course can run or not

//         int flagTA = 0;
//         int taLabs = 0;
//         int numavailableTAs = 0;
//         TA *availableTAs = (TA*) calloc(numTA*4, sizeof(TA));

//         for(int t = 0; t < numTA; t++){
//             TA *ta = (sTAsList+t);
//             if(ta->numTakenCourses == 4){
//                 continue;
//             }
//             for(int j = 0; j < ta->numTrainedCourses; j++){
//                 if(strcmp(ta->courses[j],sCoursesList->name) == 0){
//                     taLabs += 4-(ta->numTakenCourses);
//                     for(int tmp = 0; tmp < (4-(ta->numTakenCourses)); tmp++){
//                         *(availableTAs+numavailableTAs) = *ta;
//                         (availableTAs+numavailableTAs)->firstName[0] = (char) (tmp+'0');
// numavailableTAs++;
                        
//                     }
//                     break;
//                 }
//             }
//             if(sCoursesList->numLabs <= taLabs){
//                 flagTA = 1;
//                 // break;
//             }
//         }
//         //If available TAs:
//         if(flagTA == 1){
//             //Get all the combination of allowed TAs //get all applicable combinations
//             TA *local_TAs = (TA*) calloc(sCoursesList->numLabs,sizeof(TA));

//             // memcpy(local_TAs, TAsList,sizeof(TA*));
//             int tmpav = Min(sCoursesList->numLabs,numavailableTAs);
//             combinationTAs(availableTAs,local_TAs,0,tmpav,0,numavailableTAs-1,sCoursesList,sProfessorsList,sTAsList,sStudentsList,badnessPoints,schedule);
//         }
        
//         //If not available TAs, there is no possibility to run the course, whoever the professor
//         else
//             break;
        solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,schedule->totalPoints,schedule);    //will not run the course
        // printSchedule(schedule);
        // printf("%s:%d\n",professor->firstName,professor->numTakenCourses);
        // printf("%s:%d\n",profCpy->firstName,profCpy->numTakenCourses);

        // printSchedule(scheduleCpy);
        clean(schedule, scheduleCpy, sCoursesList,courseCpy,professor,profCpy, NULL, NULL,NULL,NULL);
        // Badness *bdc = schedule->badness;
        // schedule->badness = (Badness*) realloc(bdc,sizeof(Badness));
        // memcpy(schedule->badness,badnesscpy,sizeof(Badness));
        // free(badnesscpy);
        printf("Endloop\n");
        printSchedule(schedule);
        // memcpy(professor,profCpy,sizeof(Professor));
        // // printf("%s:%d\n",professor->firstName,professor->numTakenCourses);
        // memcpy(schedule,scheduleCpy,sizeof(Schedule));
        // memcpy(sCoursesList,courseCpy,sizeof(Course));
    }
    printf("END***\n");
    //don't forget to clear what you have changed in the professors, TAs, Students
    //badness points, memo it, increase the size of badness
    printf("------------------------------------------------------------------\n");
    printSchedule(schedule);
    Schedule *scheduleCpy = (Schedule*) calloc(1,sizeof(Schedule));
    // Badness *badnesscpy2 = (Badness*) calloc(1,sizeof(Badness));
    
    memcpy(scheduleCpy,schedule,sizeof(Schedule));
    // memcpy(badnesscpy2,schedule->badness,sizeof(Badness));
    

    // int num = schedule->numBadness;
    // Badness *bdc = schedule->badness;
    // schedule->badness = (Badness*) realloc(bdc,(num+1)*sizeof(Badness));
    // schedule->badness[num].type = 1;
    // schedule->badness[num].course = sCoursesList;
    // schedule->totalPoints += 20;
    // schedule->numBadness++; 
    
    solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,schedule->totalPoints,schedule);    //will not run the course
    memcpy(schedule,scheduleCpy,sizeof(Schedule));
    // memcpy(schedule->badness,badnesscpy2,sizeof(Badness));
    free(scheduleCpy);
    // free(badnesscpy2);


    printf("----------------\n");
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
            (coursesList+numCourses)->name = NULL;

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
        // solve(coursesList, professorsList, TAsList, studentsList, 0);
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
        // printf("%s\n",coursesList->name);
        // int flagTA = 0;
        // int taLabs = 0;
        // int numavailableTAs = 0;
        // TA *availableTAs = (TA*) calloc(numTA*4, sizeof(TA));

        // for(int t = 0; t < numTA; t++){
        //     TA *ta = (TAsList+t);
        //     if(ta->numTakenCourses == 4){
        //         continue;
        //     }
        //     for(int j = 0; j < ta->numTrainedCourses; j++){
        //         if(strcmp(ta->courses[j],coursesList->name) == 0){
        //             taLabs += 4-(ta->numTakenCourses);
        //             for(int tmp = 0; tmp < (4-(ta->numTakenCourses)); tmp++){
        //                 *(availableTAs+numavailableTAs++) = *ta;
        //             }
        //             break;
        //         }
        //     }
        //     if(coursesList->numLabs <= taLabs){
        //         flagTA = 1;
        //         // break;
        //     }
        // }
        // //If available TAs:
        // if(flagTA == 1){
        //     //Get all the combination of allowed TAs //get all applicable combinations
        //     TA *local_TAs = (TA*) calloc(coursesList->numLabs,sizeof(TA));

        //     // memcpy(local_TAs, TAsList,sizeof(TA*));
        //     for(int j = 0 ; j < numavailableTAs; j++)
        //         printf("^^%s\n",availableTAs[j].firstName);
        //     combinationTAs(availableTAs,local_TAs,0,coursesList->numLabs,0,numavailableTAs-1);
        // }
                // printf("asdasd");

        Schedule *schedule = (Schedule*) calloc(1, sizeof(Schedule));
        schedule->courses = (Course**) calloc(1,numCourses*sizeof(Course*));
        schedule->numCourses = 0;
        schedule->badness = (Badness*) calloc(1,sizeof(Badness));
        // schedule->courses[0]->professor = (Professor*) calloc(1,sizeof(Professor));
        // schedule->courses[0]->TAs = (TA*) calloc(1,sizeof(TA));
        // schedule->courses[0]->students = (Student*) calloc(1,sizeof(Student));


        solve(coursesList,professorsList,TAsList,studentsList,0,schedule);
        
        printf("%s\n",coursesList->name);
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
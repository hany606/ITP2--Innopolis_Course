#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include<math.h>


#define inputFileNamePrefix "input"
#define outputFileNamePrefix "HanyHamedOutput"
#define fileExtension ".txt"
#define MAXBUFFER 50
#define BADNESSSIZE 100

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
    // Course **takenCourses;
    char **takenCourses;
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
    int numCurrentStudents;
    Professor *professor;
    TA **TAs;
    Student **students;
} Course;

typedef struct badnessStruct
{
    int type;
    Professor *professor;       //pointer to the professor
    TA *ta;                     //pointer to the TA
    Student *student;           //pointer to the student
    int num;
    Course *course;             //pointer to the course
    char *courseName;
}Badness;


typedef struct scheduleStruct
{
    int numCourses;
    int numBadness;
    Course **courses;       //array of pointers for the courses
    // Badness *badness;       //array of badness
    unsigned long long int totalPoints;
}Schedule;


typedef struct courseStruct1
{
    int runFlag:1;
    char *name;
    int numLabs;
    int numStudents;
    int numCurrentStudents;
    Professor professor;    //professor
    TA *TAs;               //array of TAs
    Student *students;     //array of students
} BestCourse;

typedef struct scheduleStruct1
{
    int numCourses;
    int numBadness;
    BestCourse *courses;       //array of courses
    unsigned long long int totalPoints;
}BestSchedule;

typedef struct badnessStruct1
{
    int type;
    Professor professor;       //the professor
    TA ta;                     //the TA
    Student student;           //the student
    int num;
    Course course;             //the course
    char *courseName;
}BestBadness;



BestSchedule *bestSchedule;
BestBadness *bestBadness;
void solve(Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList,Schedule *schedule, Badness *badness);


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
    sptr->id = id;
}




void printSchedule(Schedule *schedule,Badness *badness){

    printf("#######################################\n");
        printf("Num of courses%d\n",schedule->numCourses);

        for(int i = 0; i < schedule->numCourses; i++){
            printf("Course: %s \nProf. %s\nTAs:\n",schedule->courses[i]->name, (schedule->courses[i]->professor->firstName));

            for(int j = 0; j < (schedule->courses[i])->numLabs; j++ )
            {
                printf("\t%s %s",schedule->courses[i]->TAs[j]->firstName,schedule->courses[i]->TAs[j]->lastName);
            }
            printf("\nStudents:\n");
            // printf("%s \n",schedule->courses[i]->students[0]->firstName);
            for(int j = 0; j < (schedule->courses[i])->numCurrentStudents; j++ )
            {
                printf("\t%s %s",schedule->courses[i]->students[j]->firstName, schedule->courses[i]->students[j]->lastName);
            }
            printf("\n");
        }

        printf("Num of badness: %d\n",schedule->numBadness);
        for(int j = 0; j < schedule->numBadness; j++){
            if(badness[j].type == 1)
                printf("Badness#%d: Type%d: Course:%s (Can't run)\n", j+1, badness[j].type,badness[j].course->name);
            else if(badness[j].type == 2)
                printf("Badness#%d: Type%d: Prof:%s: (Not assigned)\n", j+1, badness[j].type,badness[j].professor->firstName);
            else if(badness[j].type == 3)
                printf("Badness#%d: Type%d: Prof:%s: Course:%s (Not trained)\n", j+1, badness[j].type,badness[j].professor->firstName,badness[j].course->name);
        
            else if(badness[j].type == 4)
                printf("Badness#%d: Type%d: Prof:%s: (is lacking class)\n", j+1, badness[j].type,badness[j].professor->firstName);

            else if(badness[j].type == 5)
                printf("Badness#%d: Type%d: TA:%s: Lab:%s (is lacking lab(s))\n", j+1, badness[j].type,badness[j].ta->firstName,badness[j].course->name);

            else if(badness[j].type == 6)
                printf("Badness#%d: Type%d: Student:%s: Course:%s (is lacking course)\n", j+1, badness[j].type,badness[j].student->firstName,badness[j].courseName);
        }
        
        printf("#######################################\n");
}

void printBestSchedule(BestSchedule *schedule,BestBadness *badness){

    printf("#######################################\n");
        printf("Score: %llu\n",schedule->totalPoints);
        printf("Num of courses%d\n",schedule->numCourses);

        for(int i = 0; i < schedule->numCourses; i++){
            printf("Course: %s \nProf. %s\nTAs:\n",schedule->courses[i].name, (schedule->courses[i].professor.firstName));

            for(int j = 0; j < (schedule->courses[i]).numLabs; j++ )
            {
                printf("\t%s %s",schedule->courses[i].TAs[j].firstName,schedule->courses[i].TAs[j].lastName);
            }
            printf("\nStudents:\n");
            // printf("%s \n",schedule->courses[i]->students[0]->firstName);
            for(int j = 0; j < (schedule->courses[i]).numCurrentStudents; j++ )
            {
                printf("\t%s %s",schedule->courses[i].students[j].firstName,schedule->courses[i].students[j].lastName);
            }
            printf("\n");
        }

        printf("Num of badness: %d\n",schedule->numBadness);
        for(int j = 0; j < schedule->numBadness; j++){
            if(badness[j].type == 1)
                printf("Badness#%d: Type%d: Course:%s (Can't run)\n", j+1, badness[j].type,badness[j].course.name);
            else if(badness[j].type == 2)
                printf("Badness#%d: Type%d: Prof:%s: (Not assigned)\n", j+1, badness[j].type,badness[j].professor.firstName);
            else if(badness[j].type == 3)
                printf("Badness#%d: Type%d: Prof:%s: Course:%s (Not trained)\n", j+1, badness[j].type,badness[j].professor.firstName,badness[j].course.name);
        
            else if(badness[j].type == 4)
                printf("Badness#%d: Type%d: Prof:%s: (is lacking class)\n", j+1, badness[j].type,badness[j].professor.firstName);

            else if(badness[j].type == 5)
                printf("Badness#%d: Type%d: TA:%s: Lab:%s (is lacking lab(s))\n", j+1, badness[j].type,badness[j].ta.firstName,badness[j].course.name);

            else if(badness[j].type == 6)
                printf("Badness#%d: Type%d: Student:%s: Course:%s (is lacking course)\n", j+1, badness[j].type,badness[j].student.firstName,badness[j].courseName);
        }
        
        printf("#######################################\n");
}

void formatOutput(BestSchedule *schedule,BestBadness *badness, FILE *fptr){

    for(int i = 0; i < schedule->numCourses; i++){
        fprintf(fptr,"%s\n%s %s\n",schedule->courses[i].name,schedule->courses[i].professor.firstName,schedule->courses[i].professor.lastName);

        for(int j = 0; j < (schedule->courses[i]).numLabs; j++ )
        {
            fprintf(fptr,"%s %s\n",schedule->courses[i].TAs[j].firstName,schedule->courses[i].TAs[j].lastName);
        }

        for(int j = 0; j < (schedule->courses[i]).numCurrentStudents; j++ )
        {
            fprintf(fptr,"%s %s %s\n",schedule->courses[i].students[j].firstName,schedule->courses[i].students[j].lastName,schedule->courses[i].students[j].id);
        }
        fprintf(fptr,"\n");
    }

    for(int j = 0; j < schedule->numBadness; j++){
        if(badness[j].type == 1)
            fprintf(fptr,"%s cannot be run.\n",badness[j].course.name);
        
        else if(badness[j].type == 2)
            fprintf(fptr,"%s %s is unassigned.\n", badness[j].professor.firstName,badness[j].professor.lastName);

        else if(badness[j].type == 3)
            fprintf(fptr,"%s %s is not trained for %s.\n", badness[j].professor.firstName,badness[j].professor.lastName,badness[j].course.name);

        else if(badness[j].type == 4)
            fprintf(fptr,"%s %s is lacking class.\n", badness[j].professor.firstName,badness[j].professor.lastName);

        else if(badness[j].type == 5)
            fprintf(fptr,"%s %s is lacking %d lab(s).\n", badness[j].ta.firstName,badness[j].ta.lastName,badness[j].num);

        else if(badness[j].type == 6)
            fprintf(fptr,"%s %s is lacking %s.\n", badness[j].student.firstName,badness[j].student.lastName,badness[j].courseName);
    }
    fprintf(fptr,"Total score is %llu.\n",schedule->totalPoints);
}


void clean(Schedule *s, Schedule *cs, Course *c, Course *cc, Professor *p, Professor *cp, TA *t, TA *ct, Student *st, Student *cst){
    if(s != NULL && cs != NULL){
        // Schedule *tmp = s;
        // s = (Schedule*) realloc(tmp,sizeof(Schedule));
        memcpy(s,cs,sizeof(Schedule));
        free(cs);

    }
    if(c != NULL && cc != NULL){
        // Course *tmp = c;
        // c = (Course*) realloc(tmp,sizeof(Course));      
        memcpy(c,cc,sizeof(Course));
        free(cc);

    }
    if(p != NULL && cp != NULL){
        // Professor *tmp = p;
        // p = (Professor*) realloc(tmp,sizeof(Professor));
        memcpy(p,cp,sizeof(Professor));
        free(cp);

    }
    if(t != NULL && ct != NULL){    
        // TA *tmp = t;
        // t = (TA*) realloc(tmp,sizeof(TA));
        memcpy(t,ct,sizeof(TA));
        free(ct);

    }
    if(st != NULL && cst != NULL){
        // Student *tmp = st;
        // st = (Student*) realloc(tmp,sizeof(Student));
        memcpy(st,cst,sizeof(Student));
        free(cst);
    }

}


void combinationStudents(Student **st, Student **combStudent, int index, int r, int start, int end, Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList,unsigned long long int points, Schedule *schedule,Badness *badness){
    if(index == r){
        // for(int i = 0; i < r; i++){
        //     printf("%s%d\n",combStudent[i]->firstName,combStudent[i]->numRequiredCourses);
        // }

        Course *courseCpy = (Course*) calloc(1,sizeof(Course));
        Schedule *scheduleCpy = (Schedule*) calloc(1,sizeof(Schedule));
        memcpy(scheduleCpy,schedule,sizeof(Schedule));
        memcpy(courseCpy,sCoursesList,sizeof(Course));

        Student *studentCpy = (Student*) calloc(r,sizeof(Student));
        sCoursesList->students = (Student**) calloc(r,sizeof(Student*));
        for(int i = 0; i < r; i++){
            memcpy((studentCpy+i),combStudent[i],sizeof(Student));
        }
        for(int i = 0; i < r; i++){

            sCoursesList->students[i] = combStudent[i];
            int num = combStudent[i]->numTakenCourses;
            if(num == 0){
                combStudent[i]->takenCourses = (char**) calloc(1,sizeof(char*));
            }
            combStudent[i]->takenCourses[num]= sCoursesList->name;
            combStudent[i]->numTakenCourses++;
            combStudent[i]->takenCourses = (char**) realloc(combStudent[i]->takenCourses,(num+1)*sizeof(char*));
            sCoursesList->numCurrentStudents++;
            // printf("%s%d\n",taCpy[i].firstName,taCpy[i].numTakenCourses);
            //printf("%s/",combTA[i].firstName);
        
            // printf("%s%d\n",taCpy[i].firstName,taCpy[i].numTakenCourses);

            // sCoursesList->TAs[i]->numTakenCourses++;
            // int num = schedule->numCourses;
            //printf("%s/",combTA[i].firstName);
        }

    //     //Get all combinination of allowed Students
    //     //select other course
        solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,schedule,badness);

        for(int i = 0; i < r; i++){
            memcpy(combStudent[i],(studentCpy+i),sizeof(Student));  //return to the original
        }
        free(studentCpy);
        free(sCoursesList->students);    //free the array of pointers
        clean(schedule,scheduleCpy,sCoursesList,courseCpy,NULL,NULL,NULL,NULL,NULL,NULL);
        return;
    }
    for(int i = start; i <= end && end-i+1 >= r-index; i++){
        // printf("//\n");
        combStudent[index] = st[i];
        combinationStudents(st, combStudent, index+1,r, i+1, end, sCoursesList, sProfessorsList, sTAsList, sStudentsList, points,schedule,badness);
    }  


}


void combinationTAs(TA **tas, TA **combTA, int index, int r,int start, int end,Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList,unsigned long long int points, Schedule *schedule, Badness *badness){
    if (index == r)  
    {
        // for(int i = 0; i < r; i++){
        //     printf("%s%d\n",combTA[i]->firstName,combTA[i]->numTakenCourses);
        // }
        // printf("========================cOMB====================\n");
        // for(int i = 0; i < numTA; i++){
        //     printf("%s:%d\n",sTAsList[i].firstName,sTAsList[i].numTakenCourses);
        // }
        // schedule->courses->TAs = (TA*) calloc(r,sizeof(TA));
        Course *courseCpy = (Course*) calloc(1,sizeof(Course));
        Schedule *scheduleCpy = (Schedule*) calloc(1,sizeof(Schedule));
        memcpy(scheduleCpy,schedule,sizeof(Schedule));
        memcpy(courseCpy,sCoursesList,sizeof(Course));

        TA *taCpy = (TA*) calloc(r,sizeof(TA));
        sCoursesList->TAs = (TA**) calloc(r,sizeof(TA*));
        for(int i = 0; i < r; i++){
            memcpy((taCpy+i),combTA[i],sizeof(TA));
        }
        for(int i = 0; i < r; i++){

            sCoursesList->TAs[i] = combTA[i];
            combTA[i]->numTakenCourses++;
            // printf("%s%d\n",taCpy[i].firstName,taCpy[i].numTakenCourses);
            //printf("%s/",combTA[i].firstName);
        }
        // for(int i = 0; i < r; i++){
        //     printf("TA#%d: %s\n",i+1,sCoursesList->TAs[i]->firstName);
        // }
        // printf("----------------\n");
        // Select the allowed students
        // printf("\n==TA==\n\t");
        
        // printf("\n");


        int numavailableStudents = 0;
        Student **availableStudents = (Student**) calloc(numStudents, sizeof(Student*));

        //Get all the students who need that course
        for(int s = 0; s < numStudents; s++){
            Student *st = (sStudentsList+s);
            //Check if he need the course or not
            for(int j = 0; j < st->numRequiredCourses; j++){
                if(strcmp(st->courses[j],sCoursesList->name) == 0){
                    availableStudents[numavailableStudents++] = st;     //point to it
                    break;
                }
            }
        }


        //If available TAs:
        if(numavailableStudents > 0){
            //Get all the combination of allowed TAs //get all applicable combinations
            Student **local_students = (Student**) calloc(sCoursesList->numStudents,sizeof(Student*));

            // memcpy(local_TAs, TAsList,sizeof(TA*));
            int tmpav = Min(sCoursesList->numStudents,numavailableStudents);
            // printf("============================================\n");
            // for(int i = 0; i < numTA; i++){
            //     printf("%s:%d\n",sTAsList[i].firstName,sTAsList[i].numTakenCourses);
            // }
            combinationStudents(availableStudents,local_students,0,tmpav,0,numavailableStudents-1,sCoursesList,sProfessorsList,sTAsList,sStudentsList,schedule->totalPoints,schedule,badness);
            
            // printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
            // for(int i = 0; i < numTA; i++){
            //     printf("%s:%d\n",sTAsList[i].firstName,sTAsList[i].numTakenCourses);
            // }
            free(availableStudents);
            free(local_students);
        }
        //if there is no students, run the course
        else
            solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,schedule,badness);
        
        
        // solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,points,schedule,badness);

        for(int i = 0; i < r; i++){
            memcpy(combTA[i],(taCpy+i),sizeof(TA));  //return to the original
        }
        free(taCpy);
        free(sCoursesList->TAs);    //free the array of pointers
        clean(schedule,scheduleCpy,sCoursesList,courseCpy,NULL,NULL,NULL,NULL,NULL,NULL);

        // printf("^^^^^^^^^^^^^^^COMB^^^^^^^^^^\n");
        // for(int i = 0; i < numTA; i++){
        //     printf("%s:%d\n",sTAsList[i].firstName,sTAsList[i].numTakenCourses);
        // }
        return;
    }
    // printf("%d %d %d %d", index, r, start, end);
    for(int i = start; i <= end && end-i+1 >= r-index; i++){
        // printf("//\n");
        combTA[index] = tas[i];
        combinationTAs(tas, combTA, index+1,r, i+1, end, sCoursesList, sProfessorsList, sTAsList, sStudentsList, points,schedule,badness);
    }   
    


}


void solve(Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList,Schedule *schedule, Badness *badness){
    // printSchedule(schedule,badness);
    // printf("------------------------------------------------------------------\n");

    // Course *local_course = (Course*) calloc(numCourses, sizeof(Course));
    // memcpy(local_course,sCoursesList, sizeof(Course*));
    // // printf("%d", res);
    // local_course->name = "asd";

    //local schedule
    if(schedule->totalPoints >= mnTotalPoints){
        return;
    }
    if(sCoursesList->name == NULL){
        //compare with the global minimum and store the minimum
        //calc
        // printf("calc -> Leaf\n");
        // printf("END branch\n");
        // printf("Score: %llu \n", schedule->totalPoints);
        //rule 2, 4, 5, 6
        Schedule *scheduleCpy = (Schedule*) calloc(1,sizeof(Schedule));
        memcpy(scheduleCpy,schedule,sizeof(Schedule));
        Badness *badnesscpy = (Badness*) calloc(BADNESSSIZE,sizeof(Badness));
        memcpy(badnesscpy,badness,sizeof(Badness));

        for(int i = 0; i < numProf; i++){
            if(sProfessorsList[i].numTakenCourses == 0){
                //Not assigned
                int num = schedule->numBadness;
                badness[num].professor = (sProfessorsList+i);
                badness[num].type = 2;
                schedule->totalPoints += 10;
                schedule->numBadness++;     
            }
            else if(sProfessorsList[i].numTakenCourses == 1){
                //is lacking class
                int num = schedule->numBadness;
                badness[num].professor = (sProfessorsList+i);
                badness[num].type = 4;
                schedule->totalPoints += 5;
                schedule->numBadness++;     
            }
        }
        if(schedule->totalPoints >= mnTotalPoints)
            return;
        for(int i = 0; i < numTA; i++){
            if(sTAsList[i].numTakenCourses < 4){
                //is lacking lab
                int num = schedule->numBadness;
                badness[num].ta = (sTAsList+i);
                badness[num].type = 5;
                badness[num].num = 4-sTAsList[i].numTakenCourses;
                schedule->totalPoints += 2*(badness[num].num);
                schedule->numBadness++;
            }
        }
        if(schedule->totalPoints >= mnTotalPoints)
            return;


        for(int i = 0; i < numStudents; i++){
            // printf("%s: Take:%d, Req:%d \n",sStudentsList[i].firstName,sStudentsList[i].numTakenCourses,sStudentsList[i].numRequiredCourses);
            if(sStudentsList[i].numTakenCourses < sStudentsList[i].numRequiredCourses){
                for(int j = 0; j < sStudentsList[i].numRequiredCourses; j++){
                    int flag = 0;
                    for(int x = 0; x < sStudentsList[i].numTakenCourses; x++){
                        //That it is matched
                        if(strcmp(sStudentsList[i].takenCourses[x],sStudentsList[i].courses[j]) == 0){
                            flag = 1;
                            break;
                        }
                    }
                    if(!flag){
                        //is lacking class
                        int num = schedule->numBadness;
                        badness[num].student = (sStudentsList+i);
                        // printf("Unhappy Student %s\n",(sStudentsList+i)->firstName);
                        badness[num].type = 6;
                        schedule->totalPoints += 1;
                        schedule->numBadness++;
                        // badness[num].course = (sCoursesList+i);
                        // badness[num].courseName = (char**) calloc(MAXBUFFER,sizeof(char*));
                        badness[num].courseName = sStudentsList[i].courses[j];
                        // printf("@@@@@@@@@@%s\n",sStudentsList[i].courses[j]);
                        // badness[num].course->name = sStudentsList[i].courses[j];

                    }
                }
            }
        }
        
        // printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
        // printf("New Score: %llu\n",schedule->totalPoints);

        if(mnTotalPoints > schedule->totalPoints){
            mnTotalPoints = schedule->totalPoints;
            // printSchedule(schedule,badness);

            bestSchedule->numCourses = schedule->numCourses;
            bestSchedule->numBadness = schedule->numBadness;
            bestSchedule->totalPoints = schedule->totalPoints;

            for(int i = 0; i < schedule->numBadness; i++){
                if(badness[i].course != NULL)
                    bestBadness[i].course = *badness[i].course;
                bestBadness[i].num = badness->num;
                if(badness[i].professor != NULL)
                    bestBadness[i].professor = *badness[i].professor;
                if(badness[i].student != NULL)
                    bestBadness[i].student = *badness[i].student;
                if(badness[i].ta != NULL)
                    bestBadness[i].ta = *badness[i].ta;
                if(badness[i].courseName != NULL)
                    bestBadness[i].courseName = badness[i].courseName;
                bestBadness[i].type = badness[i].type;
            }
            
            for(int i = 0; i < schedule->numCourses; i++){
                // printf("%s\n",schedule->courses[i]->name);
                bestSchedule->courses[i].name = schedule->courses[i]->name;
                bestSchedule->courses[i].numCurrentStudents = schedule->courses[i]->numCurrentStudents;
                bestSchedule->courses[i].numLabs = schedule->courses[i]-> numLabs;
                bestSchedule->courses[i].numStudents = schedule->courses[i]->numStudents;
                bestSchedule->courses[i].professor = *schedule->courses[i]->professor;

                // printf("Finish Course\n");
                for(int x = 0; x < schedule->courses[i]->numLabs; x++){
                    bestSchedule->courses[i].TAs[x].firstName = schedule->courses[i]->TAs[x]->firstName;
                    bestSchedule->courses[i].TAs[x].lastName = schedule->courses[i]->TAs[x]->lastName;
                }

                for(int x = 0; x < schedule->courses[i]->numCurrentStudents; x++){
                    bestSchedule->courses[i].students[x].firstName = schedule->courses[i]->students[x]->firstName;
                    bestSchedule->courses[i].students[x].lastName = schedule->courses[i]->students[x]->lastName;
                    bestSchedule->courses[i].students[x].id = schedule->courses[i]->students[x]->id;
                    // printf("%s : %s\n", bestSchedule->courses[i].students[x].lastName,schedule->courses[i]->students[x]->lastName);
                }
                // printf("Finish Course\n");
                // for(int x = 0; x < schedule->courses[i]->numLabs; x++){
                //     printf("%s %s\n",bestSchedule->courses[i].TAs[x].firstName,bestSchedule->courses[i].TAs[x].lastName);
                // }
                // printf("Finis\n");
                // printf("Finish Students\n");
                
                // printf("Finish TAs\n");

            }
            // printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$4\n");
            // printBestSchedule(bestSchedule,bestBadness);
            // printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$4\n");
            // printSchedule(schedule,badness);
            // printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$4\n");
            
        }
        clean(schedule, scheduleCpy, NULL,NULL,NULL,NULL, NULL, NULL,NULL,NULL);
        memcpy(badness,badnesscpy,sizeof(Badness));
        free(badnesscpy);
        return;
    }
    // printf("^%s\n"  ,sCoursesList->name);
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

        Badness *badnesscpy = (Badness*) calloc(BADNESSSIZE,sizeof(Badness));
        memcpy(badnesscpy,badness,sizeof(Badness));
    
        //if he has two classes, continue
        if(professor->numTakenCourses == 2){
            // printf("%s has two classes\n",professor->firstName);
            clean(schedule, scheduleCpy, sCoursesList,courseCpy,professor,profCpy, NULL, NULL,NULL,NULL);
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
            // printf("+1%s\n",professor->firstName);
        }
        
        //he is not trained
        else{
            //He has already a class and he cannot take this class as he is not trained
            if(professor->numTakenCourses == 1){
                // printf("%s has 1 classes and untrained\n",professor->firstName);
                clean(schedule, scheduleCpy, sCoursesList,courseCpy,professor,profCpy, NULL, NULL,NULL,NULL);
                continue;
            }   
            //He has zero class
            else{
                professor->numTakenCourses += 2;
                flagProf = 2;
                // printf("+2%s\n",professor->firstName);
                int num = schedule->numBadness;
                badness[num].professor = professor;
                badness[num].type = 3;
                badness[num].course = sCoursesList;
                schedule->totalPoints += 5;
                schedule->numBadness++;
            }
        }

        sCoursesList->professor = professor;
        int num = schedule->numCourses;
        schedule->courses[num] = sCoursesList;
        schedule->numCourses++;

        //Prof will teach the course (There is no difference in the procedure to teach except the badness poitns)
        //Select the TAs
        //loop over TAs
        //Check if the course can run or not

        int flagTA = 0;
        int taLabs = 0;
        int numavailableTAs = 0;
        TA **availableTAs = (TA**) calloc(numTA*4, sizeof(TA*));

        for(int t = 0; t < numTA; t++){
            TA *ta = (sTAsList+t);
            //optmization part if the ta already have full classes, continue to the next
            if(ta->numTakenCourses == 4){
                continue;
            }
            //if not check if he is trained for that or not and add him as available
            for(int j = 0; j < ta->numTrainedCourses; j++){
                if(strcmp(ta->courses[j],sCoursesList->name) == 0){
                    taLabs += 4-(ta->numTakenCourses);
                    for(int tmp = 0; tmp < (4-(ta->numTakenCourses)); tmp++){
                        availableTAs[numavailableTAs] = ta;     //point to it
                        numavailableTAs++;
                    }
                    break;
                }
            }
        }

        //If available TAs:
        if(sCoursesList->numLabs <= taLabs){
            // printf("AVAILABLE TAs\n");

            //Get all the combination of allowed TAs //get all applicable combinations
            TA **local_TAs = (TA**) calloc(sCoursesList->numLabs,sizeof(TA*));

            int tmpav = Min(sCoursesList->numLabs,numavailableTAs);

            combinationTAs(availableTAs,local_TAs,0,tmpav,0,numavailableTAs-1,sCoursesList,sProfessorsList,sTAsList,sStudentsList,schedule->totalPoints,schedule,badness);
            

            free(availableTAs);
            free(local_TAs);
        }
        
        //If not available TAs, there is no possibility to run the course, whoever the professor
        else{
            // printf("NOT AVAILABLE TAs\n");
            clean(schedule, scheduleCpy, sCoursesList,courseCpy,professor,profCpy, NULL, NULL,NULL,NULL);
            free(availableTAs);
            break;
        }
        // solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,schedule->totalPoints,schedule,badness);    //will not run the course
        clean(schedule, scheduleCpy, sCoursesList,courseCpy,professor,profCpy, NULL, NULL,NULL,NULL);

        memcpy(badness,badnesscpy,sizeof(Badness));
        free(badnesscpy);
        
    }
    // printf("END***\n");
    
    // printf("------------------------------------------------------------------\n");
    // printSchedule(schedule,badness);
    Schedule *scheduleCpy = (Schedule*) calloc(1,sizeof(Schedule));
    Badness *badnesscpy2 = (Badness*) calloc(BADNESSSIZE,sizeof(Badness));
    
    memcpy(scheduleCpy,schedule,sizeof(Schedule));
    memcpy(badnesscpy2,badness,sizeof(Badness));
    

    int num = schedule->numBadness;

    badness[num].type = 1;
    badness[num].course = sCoursesList;
    schedule->totalPoints += 20;
    schedule->numBadness++; 
    
    solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,schedule,badness);    //will not run the course
    memcpy(schedule,scheduleCpy,sizeof(Schedule));
    memcpy(badness,badnesscpy2,sizeof(Badness));
    free(scheduleCpy);
    free(badnesscpy2);


    // printf("----------------\n");
}

int main(){
    int upperbound = 0;
    for(int i = 1 ; i  <= 50; i++){
        char inputFileName[12];
        char outputFileName[21];


        sprintf(inputFileName,"%s%d%s",inputFileNamePrefix,i,fileExtension);
        sprintf(outputFileName,"%s%d%s",outputFileNamePrefix,i,fileExtension);

        FILE *inputFileptr = fopen(inputFileName,"r");
        if(inputFileptr != NULL)
            upperbound = i;
    }
    // printf("%d\n",upperbound);

    for(int i = 1 ; i  <= upperbound; i++){
        // printf("File #%d\n",i);
        char inputFileName[12];
        char outputFileName[21];


        sprintf(inputFileName,"%s%d%s",inputFileNamePrefix,i,fileExtension);
        sprintf(outputFileName,"%s%d%s",outputFileNamePrefix,i,fileExtension);

        FILE *inputFileptr = fopen(inputFileName,"r");
        if(inputFileptr == NULL){
            FILE *outputFileptr = fopen(outputFileName,"w");
            fprintf(outputFileptr,"%s","Invalid input.");
            fclose(outputFileptr);
            continue;
        }


        // printf("State: Opened\n");
        Course *coursesList = (Course*) calloc(numCourses+1, sizeof(Course));
        Professor *professorsList = (Professor*) calloc(numProf+1, sizeof(Professor));
        TA *TAsList = (TA*) calloc(numTA+1, sizeof(TA));
        Student *studentsList = (Student*) calloc(numStudents+1, sizeof(Student));    
        FILE *outputFileptr = fopen(outputFileName,"w");

        // printf("%s\t%s\n", inputFileName,outputFileName);

        //Read courses
        // printf("\n##### Start Reading Courses #####\n");
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

            // printf("\n%d. %s %d %d\n", x+1, courseName, numberRequiredLabs, numberStudents);
            setCourse((coursesList+numCourses),courseName, numberRequiredLabs, numberStudents);
            coursesList = (Course*) realloc(coursesList, ((++numCourses)+1)*sizeof(Course));
            (coursesList+numCourses)->name = NULL;

        }
        // printf("\n##### End Reading Courses #####\n");

        // printf("\n##### Start Reading Professors #####\n");
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
            // printf("\n%d. %s %s", x+1, professorFirstName, professorLastName);

            while(1){
                *(courses+numCourses) = (char*) calloc(MAXBUFFER,sizeof(char *));
                state = readToken(courses[numCourses],MAXBUFFER,inputFileptr);

                if(state == 2)
                    break;
                
                courses = (char**) realloc(courses, ((++numCourses)+1)*sizeof(char*));

            }
            setProfessor((professorsList+numProf),professorFirstName, professorLastName,courses,numCourses+1);
            professorsList = (Professor*) realloc(professorsList, ((++numProf)+1)*sizeof(Professor));

        }
        // printf("\n##### End Reading Professors #####\n");

        // printf("\n##### Start Reading TAs #####\n");
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

            // printf("\n%d. %s %s", x+1, TAFirstName, TALastName);
            
            while(1){
                *(courses+numCourses) = (char*) calloc(MAXBUFFER,sizeof(char *));
                state = readToken(*(courses+numCourses),MAXBUFFER,inputFileptr);
                //New line
                // printf(" %s",*(courses+numCourses));
                if(state == 2)
                    break;
                courses = (char**) realloc(courses, ((++numCourses)+1)*sizeof(char*));
            }
            
            // printf("\n");
            setTA((TAsList+numTA),TAFirstName, TALastName,courses,numCourses+1);
            TAsList = (TA*) realloc(TAsList, ((++numTA)+1)*sizeof(TA));
            // free(courses); 

        
        }
        // printf("\n##### End Reading TAs #####\n");

        // printf("\n##### Start Reading Students #####\n");
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

            // printf("\n%d. %s %s %s", x+1, studentFirstName, studentLastName, studentID);
            
            while(1){
                *(courses+numCourses) = (char*) calloc(MAXBUFFER,sizeof(char *));
                state = readToken(*(courses+numCourses),MAXBUFFER,inputFileptr);
                //New line
                // printf(" %s",*(courses+numCourses));
                // free(trainedCourse);
                if(state == 2 || state == 3)
                    break;
                courses = (char**) realloc(courses, ((++numCourses)+1)*sizeof(char*));
            }
            // printf("\n");
            setStudent((studentsList+numStudents),studentFirstName, studentLastName, studentID, courses, numCourses+1);
            studentsList = (Student*) realloc(studentsList, ((++numStudents)+1)*sizeof(Student));

            if(state == 3){
                // free(studentFirstName);
                // free(studentLastName);
                // free(studentID);
                break;
            }
        
        }
        // printf("\n##### End Reading Students #####\n");

        
        
        // printf("\n##### Start Logic #####\n");

        

        Schedule *schedule = (Schedule*) calloc(1, sizeof(Schedule));
        Badness *badness = (Badness*) calloc(BADNESSSIZE,sizeof(Badness));
        schedule->courses = (Course**) calloc(1,numCourses*sizeof(Course*));
        schedule->numCourses = 0;
        schedule->totalPoints = 0;

        bestSchedule = (BestSchedule*) calloc(1, sizeof(BestSchedule));
        bestSchedule->courses = (BestCourse*) calloc(1,numCourses*sizeof(BestCourse));
        for(int x = 0; x < numCourses; x++){
            bestSchedule->courses[x].TAs = (TA*) calloc(1,(numTA+1)*sizeof(TA));  //array of TAs
            bestSchedule->courses[x].students = (Student*) calloc(1,(numStudents+1)*sizeof(Student));   //array of students
        }

        bestBadness = (BestBadness*) calloc(BADNESSSIZE,sizeof(Badness));


        solve(coursesList,professorsList,TAsList,studentsList,schedule,badness);
        
        // printf("\n##### End Logic #####\n");


        // printf("#######FINAL######\n");
        // printBestSchedule(bestSchedule,bestBadness);
        // printf("#######FINAL######\n");
        formatOutput(bestSchedule,bestBadness,outputFileptr);
        free(coursesList);
        free(professorsList);
        free(TAsList);
        free(studentsList);
        free(badness);
        fclose(inputFileptr);
        free(bestSchedule);
        free(bestBadness);
        fclose(outputFileptr);
    }

    FILE *endFileptr = fopen("HanyHamedEmail.txt","w");
    fprintf(endFileptr,"h.hamed.elanwar@gmail.com\n");
    fclose(endFileptr);
}
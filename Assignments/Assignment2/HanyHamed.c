//Include the necessary librariies
#include<stdio.h>   //for I/O
#include<stdlib.h>  //to use calloc, realloc, ....
#include<limits.h>  //To get the maximum limit of point to minimize
#include<string.h>  //for string manipulation


/**
 * Introduction to Programming 2 (ITP2)
 * Innopolis University, Sprinng 2019
 * Assignment 2, University Scheduling problem
 * Author: Hany Hamed
 * Version: V2.3.2 (Final)
 */

#define inputFileNamePrefix "input"             //The prefix of the name of the input file
#define outputFileNamePrefix "HanyHamedOutput"  //The prefix of the name of the output file
#define fileExtension ".txt"                    //the extension of the files
#define MAXBUFFER 100                            //The maximum buffer for the names and the strings in the program
#define BADNESSSIZE 500                         //The maximum number of badness points that can appear
#define Min(x,y) (x >= y ? y : x)               //Macro for the calculate the minimum of 2 vars 

int numCourses = 0;                                 //Global var to store the num of courses in the inputs
int numProf = 0;                                    //Global var to store the num of professors in the inputs
int numTA = 0;                                      //Global var to store the num of TAs in the inputs
int numStudents = 0;                                //Global var to store the num of students in the inputs
unsigned long long int mnTotalPoints = ULLONG_MAX;  //Global variable to store the minimum point that we have got, used as conditional optimization




//Struct to store the professors
typedef struct professorStruct
{
    int numTrainedCourses;      //Number of the trained courses that the professor has
    int numTakenCourses;        //Number of the courses that the professor will take(teach)
    char *firstName;            //Array of Char (String) to store the first name
    char *lastName;             //String to store the last name
    char **courses;             //Array of Array of Char(Array of strings) to store the name of courses that he is trained for
} Professor;

//Struct to store the TA
typedef struct TAStruct
{
    int numTrainedCourses;      //Number of the trained courses that the TA has
    int numTakenCourses;        //Number of the courses that the TA will take(teach)
    char *firstName;            //Array of Char (String) to store the first name
    char *lastName;             //String to store the last name
    char **courses;             //Array of Array of Char(Array of strings) to store the name of courses that he is trained for
} TA;


//Struct to store the Student
typedef struct studentStruct
{
    int numRequiredCourses;     //Number of the required courses for the student
    char **courses;             //Array of strings to store the name of the courses that the student want to take during the semester
    int numTakenCourses;        //Number of the courses that the student will learn in the semester
    char **takenCourses;        //Array of strings to store the names of the courses that the student will take
    char *firstName;            //String to store the first name
    char *lastName;             //String to store the last name
    char *id;                   //String to store the id
} Student;


//Struct to store the course
typedef struct courseStruct
{
    char *name;                 //Name of the course
    int numLabs;                //Number of the labs needed for the course to be runned
    int numStudents;            //Store the maximum number of students that can enroll in the course
    int numCurrentStudents;     //the final number of students that enrolled in the course
    Professor *professor;       //Pointer to the professor of the course
    TA **TAs;                   //Array of pointers to the TAs who are responsible for the course
    Student **students;         //Array of pointers to the students who will take the course
} Course;


//Struct to store the badness of the scheule, it can contain all types of badness
typedef struct badnessStruct
{
    int type;                   //To store the type of the badness (Used for printing the correct printing of the error)
    Professor *professor;       //Pointer to the professor (Used in some type of badness)
    TA *ta;                     //Pointer to the TA (Used in some types of badness)
    Student *student;           //Pointer to the student (Used in some types of badness)
    Course *course;             //Pointer to the course (Used in some types of badness)
    int num;                    //Store the number of labs/courses (Used in some types of badness)
    char *courseName;           //String to store the course name (Used in some types of badness)
}Badness;

//Struct to store the schedule
typedef struct scheduleStruct
{
    int numCourses;                         //Number of the courses in the schedule
    int numBadness;                         //Number of badnesses in the schedule
    Course **courses;                       //Array of pointers to the courses
    unsigned long long int totalPoints;     //Total number of badness points in the schedule
}Schedule;


//Struct to store the course that exist in the best schedule (Here we are storing value not pointers of structs)
typedef struct bestCourseStruct
{
    char *name;                     //Name of the course
    int numLabs;                    //Numbr of the labs
    int numStudents;                //Number of students
    int numCurrentStudents;         //Number of students who already enrolled in the course
    Professor professor;            //The professor of the course
    TA *TAs;                        //Array of TAs
    Student *students;              //Array of students
} BestCourse;

//Struct to store the best schedule (Here we are storing value not pointers of structs)
typedef struct bestScheduleStruct
{
    int numCourses;                     //Number of courses in the schedule
    int numBadness;                     //Number of badnesses
    BestCourse *courses;                //Array of courses
    unsigned long long int totalPoints; //total points of the schedule
}BestSchedule;

//Struct to store the best badness of the schedule (Here we are storing value not pointers of structs)
typedef struct bestBadnessStruct
{
    int type;                   //The type of the badness
    Professor professor;        //The professor
    TA ta;                      //The TA
    Student student;            //The student
    int num;                    //The number of labs/courses
    Course course;              //The course
    char *courseName;           //The coourse name
}BestBadness;


BestSchedule *bestSchedule;     //Global BestSchedule in order to save the most optimal solution
BestBadness *bestBadness;       //Global BestBadness in order to save the most optimal solution


/*
 * Prototype Function:  solve 
 * --------------------
 * Prototype for the function in the begining in order to be used in other functions without any problems, 
        as it calls some function (combinationTAs) and the other functions calls it recursivly
    
    Functionallity:
        Get the most optimal solution of the schedule by get the combination of the professors for specific course, 
    then call combinationTA to get the combination of TAs for the course, 
    then inside combinationTA call combinationStudents, to get the combination of students for the course,
    then call again solve for the next course.
    If it covered all the courses, it will reach leaf, calculate the schedule and compare it the global optimal

    Parameters:
        sCoursesList:       Pointer to the first course in the list of courses (Pointer to the courses list)
        sProfessorsList:    Pointer to the list of professors
        sTAsList:           Pointer to the list of TAs
        sStudentsList:      Pointer to the list of students
        schedule:           Pointer to the schedule that it is passed with the recursive calls
        badness:            Pointer to the badness that it is passes with the recursive calls
 */
void solve(Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList,Schedule *schedule, Badness *badness);


/*
 * Function:  readToken 
 * --------------------
 * It reads char by char and combine and check the end
 *  Read token and return the token.
 *  token: a pointer to the array that we will put the token in it
 *  maxBuffer: the max size of the buffer of the input
 *  fptr: the pointer to the file that we need to take the input from
 *  returns: flag:
 * 
     * flag = 1 -> space
     *      = 2 -> newLine
     *      = 3 -> EOF
 */
int readToken(char *token,FILE* fptr){
    int flag = 0;
    //Read until the buffer ends
    for(int i = 0; i < MAXBUFFER; i++){
        char tmp = fgetc(fptr); //get the char from the input
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
        *(token+i) = tmp;     //Store the char in the string
    }
}

/*
 * Function:  setCourse 
 * --------------------
 * Set the course cptr with the specific parameters
 * cptr: pointer to the course
 */
void setCourse(Course *cptr, char name[], int numReqLabs, int numStudents){
    cptr->name = name;
    cptr->numLabs = numReqLabs;
    cptr->numStudents = numStudents;
}

/*
 * Function:  setProfessors 
 * --------------------
 * Set the professors pptr with the specific parameters
 * pptr: pointer to the professor
 */
void setProfessor(Professor *pptr, char fName[], char lName[], char **courses, int nC){
    pptr->courses = courses;
    pptr->firstName = fName;
    pptr->lastName = lName;
    pptr->numTakenCourses = 0;
    pptr->numTrainedCourses = nC;
}

/*
 * Function:  setTa 
 * --------------------
 * Set the TA tptr with the specific parameters
 * tptr: pointer to the TA
 */
void setTA(TA *tptr, char fName[], char lName[] , char **courses, int nC){
    tptr->courses = courses;
    tptr->firstName = fName;
    tptr->lastName = lName;
    tptr->numTakenCourses = 0;
    tptr->numTrainedCourses = nC;
}

/*
 * Function:  setStudent 
 * --------------------
 * Set the student sptr with the specific parameters
 * spt: pointer to the course
 */
void setStudent(Student *sptr, char fName[], char lName[], char id[], char **courses, int nC){
    sptr->courses = courses;
    sptr->firstName = fName;
    sptr->lastName = lName;
    sptr->numTakenCourses = 0;
    sptr->numRequiredCourses = nC;
    sptr->id = id;
}

/*
 * Function:  printSchedule 
 * --------------------
 * For debugging to print the schedule and the badness
 */
void printSchedule(Schedule *schedule,Badness *badness){

    printf("#######################################\n");
    printf("Num of courses%d\n",schedule->numCourses);
    //iterate over the courses and print them, with professors, TAs and students
    for(int i = 0; i < schedule->numCourses; i++){
        printf("Course: %s \nProf. %s\nTAs:\n",schedule->courses[i]->name, (schedule->courses[i]->professor->firstName));

        for(int j = 0; j < (schedule->courses[i])->numLabs; j++ )
        {
            printf("\t%s %s",schedule->courses[i]->TAs[j]->firstName,schedule->courses[i]->TAs[j]->lastName);
        }
        printf("\nStudents:\n");
        // printf("%s \n",schedule->courses[i]->students[0]->firstName);
        printf("NUMBER OF %d\n",schedule->courses[i]->numCurrentStudents);
        for(int j = 0; j < (schedule->courses[i])->numCurrentStudents; j++ )
        {
            if(schedule->courses[i]->students[j] == NULL)
                break;
            printf("\t%s %s",schedule->courses[i]->students[j]->firstName, schedule->courses[i]->students[j]->lastName);
        }
        printf("\n");
    }

    //iterate over the badness print all the badness and with their format
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

/*
 * Function:  printBestSchedule 
 * --------------------
 * For debugging to print the bestschedule and the bestbadness
 * It is the same structure as printSchedule
 * but bestschedule, bestbadness have values not pointers so, access to the data is different
 */
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

/*
 * Function:  formatOutput 
 * --------------------
 * to output the schedule with the specific format to the file stream
 */
void formatOutput(BestSchedule *schedule,BestBadness *badness, FILE *fptr){

    //Iterate over the courses, print the professor, TAs, Students
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

    //Iterate over the badness and print them in the correct format
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
    fprintf(fptr,"Total score is %llu.\n",schedule->totalPoints);   //print the score of the badness
}

/*
 * Function:  clean 
 * --------------------
 * copy the old version of struct to the modified struct to store it as previuos case before the recursive calls
 * as in the recursive calls, we don't pass by values, we pass references.
 * So, it restore the old version before the call and free the struct from the memory
 */
void clean(Schedule *s, Schedule *cs, Course *c, Course *cc, Professor *p, Professor *cp, TA *t, TA *ct, Student *st, Student *cst){
    if(s != NULL && cs != NULL){
        memcpy(s,cs,sizeof(Schedule));
        free(cs);

    }
    if(c != NULL && cc != NULL){   
        memcpy(c,cc,sizeof(Course));
        free(cc);

    }
    if(p != NULL && cp != NULL){
        memcpy(p,cp,sizeof(Professor));
        free(cp);

    }
    if(t != NULL && ct != NULL){
        memcpy(t,ct,sizeof(TA));
        free(ct);

    }
    if(st != NULL && cst != NULL){
        memcpy(st,cst,sizeof(Student));
        free(cst);
    }

}

/*
 * Function:  combinationStudents 
 * --------------------
 * get the all combination of students in specific list of student from st and store it in combstudent
 * Here we are passing the whole information of the branch of the recursive calls in sCoursesList, sProfessorsList,sTAsList,sStudentsList, schedule, badness
 * It get all the combination and call solve to choose a combination of the next course
 * Parameters:
 *      st:                 (original array) pointer to the array of possible student in the course
 *      combStudent:        (combination array) pointer to the array that will store the combination during the recursive calls of the same function
 *      indes:              the index of the current element in the combination array
        r:                  the number of elements needed in the combination array
        start:              the start of the original array (the index of the current element of the original array)
        end:                the end of the original array (the size of the array)
        sCoursesList:       Pointer to the first course in the list of courses (Pointer to the courses list)
        sProfessorsList:    Pointer to the list of professors
        sTAsList:           Pointer to the list of TAs
        sStudentsList:      Pointer to the list of students
        schedule:           Pointer to the schedule that it is passed with the recursive calls
        badness:            Pointer to the badness that it is passes with the recursive calls

 */
void combinationStudents(Student **st, Student **combStudent, int index, int r, int start, int end, Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList, Schedule *schedule,Badness *badness){
    //If we have all the elements we need, then we have full set of a possible combination
    if(index == r){
        //Take a copy from the course, schedule, student before modified it
        //Reserve enough space in the memory
        Course *courseCpy = (Course*) calloc(1,sizeof(Course));
        Schedule *scheduleCpy = (Schedule*) calloc(1,sizeof(Schedule));
        Student *studentCpy = (Student*) calloc(r,sizeof(Student));
        //Copy the data from the original to the copy
        memcpy(scheduleCpy,schedule,sizeof(Schedule));
        memcpy(courseCpy,sCoursesList,sizeof(Course));

        //Reserve space in the course for students
        sCoursesList->students = (Student**) calloc(r,sizeof(Student*));

        //Copy the content of the students to the copy from combStudents which is pointer to the students
        for(int i = 0; i < r; i++){
            memcpy((studentCpy+i),combStudent[i],sizeof(Student));
        }
        //Iterate over the combination of students
        for(int i = 0; i < r; i++){
            sCoursesList->students[i] = combStudent[i];     //register the students in the courses
            combStudent[i]->takenCourses = (char**) calloc(numCourses,sizeof(char*));
            int num = combStudent[i]->numTakenCourses;      //get the number of the course that the student taken until now (Works as index)
            combStudent[i]->takenCourses[num]= sCoursesList->name;  //Store the name of the course in the correct place in the array of strings of the names of the courses
            //Reallocate the memory for bigger courses
            combStudent[i]->numTakenCourses++;                      //increase the number
        }
        sCoursesList->numCurrentStudents = r;

        //Call solve for the next courses with this state
        solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,schedule,badness);

        //Restore the state before the chagnes
        //By coppy back from the copies
        for(int i = 0; i < r; i++){
            memcpy(combStudent[i],(studentCpy+i),sizeof(Student));  //return to the original
        }
        free(studentCpy);                   //free the copy
        free(sCoursesList->students);       //free the array of pointers
        clean(schedule,scheduleCpy,sCoursesList,courseCpy,NULL,NULL,NULL,NULL,NULL,NULL);
        return;
    }
    //iterate from the begining to the end and select student, then recursive call with increased in the index of chosen and in the index of the original array
    //  which means look to the rest of the array
    for(int i = start; i <= end && end-i+1 >= r-index; i++){
        combStudent[index] = st[i];
        combinationStudents(st, combStudent, index+1,r, i+1, end, sCoursesList, sProfessorsList, sTAsList, sStudentsList,schedule,badness);
    }  
}

/*
 * Function:  combinationTAs 
 * --------------------
 * get the all combination of TAs in specific list of TAs from tas and store it in combTA
 * Here we are passing the whole information of the branch of the recursive calls in sCoursesList, sProfessorsList,sTAsList,sStudentsList, schedule, badness
 * It get all the combination and get the valid students for the course (Who want to take the course) and call combStudents in order to get
 *  the combination of students
 * Parameters:
 *      tas:                (original array) pointer to the array of possible TAs in the course
 *      combTA:             (combination array) pointer to the array that will store the combination during the recursive calls of the same function
 *      indes:              the index of the current element in the combination array
        r:                  the number of elements needed in the combination array
        start:              the start of the original array (the index of the current element of the original array)
        end:                the end of the original array (the size of the array)
        sCoursesList:       Pointer to the first course in the list of courses (Pointer to the courses list)
        sProfessorsList:    Pointer to the list of professors
        sTAsList:           Pointer to the list of TAs
        sStudentsList:      Pointer to the list of students
        schedule:           Pointer to the schedule that it is passed with the recursive calls
        badness:            Pointer to the badness that it is passes with the recursive calls

 */
void combinationTAs(TA **tas, TA **combTA, int index, int r,int start, int end,Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList, Schedule *schedule, Badness *badness){
    //If we have all the elements we need, then we have full set of a possible combination
    if (index == r)  
    {
        //Take a copy from the course, schedule, TA before modified it
        //Reserve enough space in the memory
        Course *courseCpy = (Course*) calloc(1,sizeof(Course));
        Schedule *scheduleCpy = (Schedule*) calloc(1,sizeof(Schedule));
        TA *taCpy = (TA*) calloc(r,sizeof(TA));
        //Copy the data from the original to the copy
        memcpy(scheduleCpy,schedule,sizeof(Schedule));
        memcpy(courseCpy,sCoursesList,sizeof(Course));

        //Reserve space in the course for TAs
        sCoursesList->TAs = (TA**) calloc(r,sizeof(TA*));
        //Copy the content of the TAs to the copy from combTAs which is pointer to the TAs
        for(int i = 0; i < r; i++){
            memcpy((taCpy+i),combTA[i],sizeof(TA));
        }
        //Iterate over the TAs
        for(int i = 0; i < r; i++){
            sCoursesList->TAs[i] = combTA[i];   //register the TAs in the courses
            combTA[i]->numTakenCourses++;       //increment of the counter
        }

        //Get the available students to get the set to make a combination of them
        int numavailableStudents = 0;   //Counter fot the available students
        //Array of pointer to the available students
        Student **availableStudents = (Student**) calloc(numStudents, sizeof(Student*));

        //Get all the students who need that course
        //Iterate over the whole list of students
        for(int s = 0; s < numStudents; s++){
            Student *st = (sStudentsList+s);    //The current studnet
            //Iterate over the courses he needs
            //Check if he need the course or not
            for(int j = 0; j < st->numRequiredCourses; j++){
                //If he need this course add him in the list and break
                if(strcmp(st->courses[j],sCoursesList->name) == 0){
                    availableStudents[numavailableStudents++] = st;     //point to it
                    break;
                }
            }
        }

        //If there are students who need the course:
        if(numavailableStudents > 0){
            //Get all the combination of allowed students
            //this will store the combination of students
            Student **local_students = (Student**) calloc(sCoursesList->numStudents,sizeof(Student*));
            int tmpav = Min(sCoursesList->numStudents,numavailableStudents);    //choose the minimum the needed number of students for the course or the available students
            combinationStudents(availableStudents,local_students,0,tmpav,0,numavailableStudents-1,sCoursesList,sProfessorsList,sTAsList,sStudentsList,schedule,badness);
            //free the memory
            free(local_students);
        }
        //if there is no students, run the course and go to look on the next course
        else
            solve((sCoursesList+1),sProfessorsList, sTAsList, sStudentsList,schedule,badness);
        
        //Restore the state before the chagnes
        //By copy back from the copies
        for(int i = 0; i < r; i++){
            memcpy(combTA[i],(taCpy+i),sizeof(TA));  //return to the original
        }
        free(availableStudents);    //free the array of pointers
        free(taCpy);
        free(sCoursesList->TAs);    //free the array of pointers
        clean(schedule,scheduleCpy,sCoursesList,courseCpy,NULL,NULL,NULL,NULL,NULL,NULL);

        return;
    }
    //iterate from the begining to the end and select TA, then recursive call with increased in the index of chosen and in the index of the original array
    //  which means look to the rest of the array
    for(int i = start; i <= end && end-i+1 >= r-index; i++){
        combTA[index] = tas[i];
        combinationTAs(tas, combTA, index+1,r, i+1, end, sCoursesList, sProfessorsList, sTAsList, sStudentsList,schedule,badness);
    }   
    


}

/*
 * Function:  solve 
 * --------------------
    Functionallity:
        Get the most optimal solution of the schedule by get the combination of the professors for specific course, 
    then call combinationTA to get the combination of TAs for the course, 
    then inside combinationTA call combinationStudents, to get the combination of students for the course,
    then call again solve for the next course.
    If it covered all the courses, it will reach leaf, calculate the schedule and compare it the global optimal

    Parameters:
        sCoursesList:       Pointer to the first course in the list of courses (Pointer to the courses list)
        sProfessorsList:    Pointer to the list of professors
        sTAsList:           Pointer to the list of TAs
        sStudentsList:      Pointer to the list of students
        schedule:           Pointer to the schedule that it is passed with the recursive calls
        badness:            Pointer to the badness that it is passes with the recursive calls
 */
void solve(Course *sCoursesList, Professor *sProfessorsList, TA *sTAsList, Student *sStudentsList,Schedule *schedule, Badness *badness){
    //if we have better schedule dont' go deep in this schedule and finish the whole branch
    if(schedule->totalPoints >= mnTotalPoints){
        return;
    }
    //If we reached to the leaf of the branch
    if(sCoursesList->name == NULL){
        //Take the copies from the schedule and badness
        Schedule *scheduleCpy = (Schedule*) calloc(1,sizeof(Schedule));
        Badness *badnesscpy = (Badness*) calloc(BADNESSSIZE,sizeof(Badness));
        memcpy(badnesscpy,badness,sizeof(Badness));
        memcpy(scheduleCpy,schedule,sizeof(Schedule));

        //Calulate some rule of badness

        //Calculate if the professor is unassigned or lacking class
        //iterate over the list and check
        for(int i = 0; i < numProf; i++){
            //if he has no classes
            if(sProfessorsList[i].numTakenCourses == 0){
                //Not assigned
                //mark a badness and update badness and schedule
                int num = schedule->numBadness;
                badness[num].professor = (sProfessorsList+i);
                badness[num].type = 2;
                schedule->totalPoints += 10;
                schedule->numBadness++;     
            }
            //if he is lacking class
            else if(sProfessorsList[i].numTakenCourses == 1){
                //is lacking class
                //mark a badness and update badness and schedule
                int num = schedule->numBadness;
                badness[num].professor = (sProfessorsList+i);
                badness[num].type = 4;
                schedule->totalPoints += 5;
                schedule->numBadness++;     
            }
        }
        //if we have better schedule, return
        if(schedule->totalPoints >= mnTotalPoints)
            return;

        //Calculate if the TA is lacking class
        //iterate over the list and check
        for(int i = 0; i < numTA; i++){
            //if he is lacking lab
            if(sTAsList[i].numTakenCourses < 4){
                //is lacking lab
                //Calculate if the professor is lacking class with the number of the lacking
                int num = schedule->numBadness;
                badness[num].ta = (sTAsList+i);
                badness[num].type = 5;
                badness[num].num = 4-sTAsList[i].numTakenCourses;
                schedule->totalPoints += 2*(badness[num].num);
                schedule->numBadness++;
            }
        }
        //if we have better schedule, return
        if(schedule->totalPoints >= mnTotalPoints)
            return;

        //Calculate if the student didn't take his required courses
        //iterate over the list and check
        for(int i = 0; i < numStudents; i++){
            //if he has less than the number he required
            if(sStudentsList[i].numTakenCourses < sStudentsList[i].numRequiredCourses){
                //iterate over his courses list, and the courses he needs and get the name of that course
                for(int j = 0; j < sStudentsList[i].numRequiredCourses; j++){
                    int flag = 0;
                    for(int x = 0; x < sStudentsList[i].numTakenCourses; x++){
                        //That it is matched, that means that he required and take it
                        if(strcmp(sStudentsList[i].takenCourses[x],sStudentsList[i].courses[j]) == 0){
                            flag = 1;
                            break;
                        }
                    }
                    //if he didn't take that course that he wants
                    if(!flag){
                        //is lacking class
                        //mark a badness and update badness and schedule
                        int num = schedule->numBadness;
                        badness[num].student = (sStudentsList+i);
                        badness[num].type = 6;
                        schedule->totalPoints += 1;
                        schedule->numBadness++;
                        badness[num].courseName = sStudentsList[i].courses[j];
                    }
                }
            }
        }

        //if he has less points after the new caluclation, then it is better schedule
        if(mnTotalPoints > schedule->totalPoints){
            mnTotalPoints = schedule->totalPoints;              //store the minimum points that we have now
            //Copy this schedule to the global variable with the whole data

            bestSchedule->numCourses = schedule->numCourses;
            bestSchedule->numBadness = schedule->numBadness;
            bestSchedule->totalPoints = schedule->totalPoints;

            //copy the badness
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

            //copy the courses, TAs, Professors, Students
            for(int i = 0; i < schedule->numCourses; i++){
                bestSchedule->courses[i].name = schedule->courses[i]->name;
                bestSchedule->courses[i].numCurrentStudents = schedule->courses[i]->numCurrentStudents;
                bestSchedule->courses[i].numLabs = schedule->courses[i]-> numLabs;
                bestSchedule->courses[i].numStudents = schedule->courses[i]->numStudents;
                bestSchedule->courses[i].professor = *schedule->courses[i]->professor;

                for(int x = 0; x < schedule->courses[i]->numLabs; x++){
                    bestSchedule->courses[i].TAs[x].firstName = schedule->courses[i]->TAs[x]->firstName;
                    bestSchedule->courses[i].TAs[x].lastName = schedule->courses[i]->TAs[x]->lastName;
                }

                for(int x = 0; x < schedule->courses[i]->numCurrentStudents; x++){
                    bestSchedule->courses[i].students[x].firstName = schedule->courses[i]->students[x]->firstName;
                    bestSchedule->courses[i].students[x].lastName = schedule->courses[i]->students[x]->lastName;
                    bestSchedule->courses[i].students[x].id = schedule->courses[i]->students[x]->id;

                }

            }
        }
        

        //if not the optimal, restore the original state of the schedule and free
        clean(schedule, scheduleCpy, NULL,NULL,NULL,NULL, NULL, NULL,NULL,NULL);
        memcpy(badness,badnesscpy,sizeof(Badness));
        free(badnesscpy);
        return;
    }

    //Check if the course can run or not: Is there enough professors
    //Select a professor who are able to teach this course
    //loop over the professors
    for(int p = 0; p < numProf; p++){
        Professor *professor = (sProfessorsList+p);     //the current professor
        //Allocate place for copies
        Professor *profCpy = (Professor*) calloc(1,sizeof(Professor));
        Course *courseCpy = (Course*) calloc(1,sizeof(Course));
        Schedule *scheduleCpy = (Schedule*) calloc(1,sizeof(Schedule));
        Badness *badnesscpy = (Badness*) calloc(BADNESSSIZE,sizeof(Badness));

        //Take copies
        memcpy(scheduleCpy,schedule,sizeof(Schedule));
        memcpy(courseCpy,sCoursesList,sizeof(Course));
        memcpy(profCpy,professor,sizeof(Professor));
        memcpy(badnesscpy,badness,sizeof(Badness));
    
        //if he has two classes, continue
        if(professor->numTakenCourses == 2){
            //clean, restore and free
            clean(schedule, scheduleCpy, sCoursesList,courseCpy,professor,profCpy, NULL, NULL,NULL,NULL);
            continue;
        }
        //Check if he is trained or not
        int trainedProfessor = 0;
        for(int j = 0; j < professor->numTrainedCourses; j++){
            //if he has this course in his list as trained
            if(strcmp(professor->courses[j],sCoursesList->name) == 0){
                trainedProfessor = 1;
                break;   
            }
        }

        //if he has one class, he will teach this course if he is trained no badness points
        //if he has zero class, he will teach this course in both ways (trained, untrained) but with different badness points
            //if trained, no badness points
            //if untrained, 5 points
            
        //if trained professor and can take the class have 0 or 1 courses
        if(trainedProfessor == 1)
            professor->numTakenCourses++;
        
        
        //if he is not trained
        else{
            //He has already a class and he cannot take this class as he is not trained
            if(professor->numTakenCourses == 1){
                clean(schedule, scheduleCpy, sCoursesList,courseCpy,professor,profCpy, NULL, NULL,NULL,NULL);
                continue;
            }   
            //He has zero class
            else{
                professor->numTakenCourses += 2;
                //mark a badness of untrained professor
                int num = schedule->numBadness;
                badness[num].professor = professor;
                badness[num].type = 3;
                badness[num].course = sCoursesList;
                schedule->totalPoints += 5;
                schedule->numBadness++;
            }
        }
        //Prof will teach the course (There is no difference in the procedure to teach except the badness poitns)

        //assign the professor to the course
        sCoursesList->professor = professor;
        int num = schedule->numCourses;
        //add the course to the schedule
        schedule->courses[num] = sCoursesList;
        schedule->numCourses++; //increment the number of the courses in the schdule

        //Select the TAs
        //Check if the course can run or not
        int flagTA = 0;     //flag to see if the course can run because of the TAs or not
        int taLabs = 0;     //store the number of the labs that can give for that course
        int numavailableTAs = 0;
        TA **availableTAs = (TA**) calloc(numTA*4, sizeof(TA*));    //to store the availabe TAs
        //iterate over the TAs
        for(int t = 0; t < numTA; t++){
            TA *ta = (sTAsList+t);
            //optmization part if the ta already have full classes, continue to the next
            if(ta->numTakenCourses == 4){
                continue;
            }
            //if not check if he is trained for that or not and add him as available
            //iterate over the courses that he have
            for(int j = 0; j < ta->numTrainedCourses; j++){
                //if he is trained on this course
                if(strcmp(ta->courses[j],sCoursesList->name) == 0){
                    taLabs += 4-(ta->numTakenCourses);      //increment the number of the labs by the avalability of the TA
                    //Add the TA to the list with multiplicity
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
            //Get all the combination of allowed TAs
            //this will store the combination of students
            TA **local_TAs = (TA**) calloc(sCoursesList->numLabs,sizeof(TA*));
            //choose the minimum the needed number of TAs for the course or the available TAs
            int tmpav = Min(sCoursesList->numLabs,numavailableTAs);
            //call this function to get the combinations of TAs for the course
            combinationTAs(availableTAs,local_TAs,0,tmpav,0,numavailableTAs-1,sCoursesList,sProfessorsList,sTAsList,sStudentsList,schedule,badness);
            free(availableTAs);
            free(local_TAs);
        }
        
        //If not available TAs, there is no possibility to run the course, whoever the professor
        else{
            //restore the state of the course
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
    //get the upper bound of files number
    //iterate on them and check the max file that will open
    int upperbound = 0;
    char *eptr;
    for(int i = 1 ; i  <= 50; i++){
        char inputFileName[12];
        char outputFileName[21];
        //concatnate the name of the input and output files
        sprintf(inputFileName,"%s%d%s",inputFileNamePrefix,i,fileExtension);
        sprintf(outputFileName,"%s%d%s",outputFileNamePrefix,i,fileExtension);

        FILE *inputFileptr = fopen(inputFileName,"r");
        if(inputFileptr != NULL)
            upperbound = i;
    }
    // printf("%d\n",upperbound);

    //iterate on files
    for(int i = 1 ; i  <= upperbound; i++){
        //initialize the global vars
        mnTotalPoints = ULLONG_MAX;
        numCourses = 0;                                 //Global var to store the num of courses in the inputs
        numProf = 0;                                    //Global var to store the num of professors in the inputs
        numTA = 0;                                      //Global var to store the num of TAs in the inputs
        numStudents = 0;
        
        int invalidFlag = 0;
        // printf("File #%d\n",i);
        char inputFileName[12];
        char outputFileName[21];

        //concatnate the name of the input and output files
        sprintf(inputFileName,"%s%d%s",inputFileNamePrefix,i,fileExtension);
        sprintf(outputFileName,"%s%d%s",outputFileNamePrefix,i,fileExtension);

        //open the file
        FILE *inputFileptr = fopen(inputFileName,"r");
        //if is not exist
        if(inputFileptr == NULL){
            //print invalid input and close it
            FILE *outputFileptr = fopen(outputFileName,"w");
            fprintf(outputFileptr,"%s","Invalid input.");
            fclose(outputFileptr);
            continue;
        }



        //create lists for courses, professors, TAs, students
        Course *coursesList = (Course*) calloc(numCourses+1, sizeof(Course));
        Professor *professorsList = (Professor*) calloc(numProf+1, sizeof(Professor));
        TA *TAsList = (TA*) calloc(numTA+1, sizeof(TA));
        Student *studentsList = (Student*) calloc(numStudents+1, sizeof(Student));
        //open the output file  
        FILE *outputFileptr = fopen(outputFileName,"w");


        //Read courses
        for(int x = 0; ; x++){
            char *tmp = (char*) calloc(1, MAXBUFFER); //var to store the token
            char *courseName = (char*) calloc(1, MAXBUFFER);    //store the name of the course
            int numberRequiredLabs;
            int numberStudents;       
            int state;  //store the state of the token

            state = readToken(courseName,inputFileptr);   //read the name
            //if is just P, the end of courses
            if(courseName[0] == 'P' && courseName[1] == '\0'){
                // free(courseName);
                // free(tmp);
                break;
            }


            //check if the name is T, P or S this will be invalid
            if(strcmp(courseName,"T") == 0 || strcmp(courseName,"P") == 0 || strcmp(courseName,"S") == 0)
                invalidFlag = 1;

            //check that the name is only English letter not 
            for(int j = 0; j < MAXBUFFER && courseName[j] != '\0'; j++){
                if(!((courseName[j] >= 'a' && courseName[j] <= 'z') ||(courseName[j] >= 'A' && courseName[j] <= 'Z'))){
                    
                    invalidFlag = 1;
                    break;
                }
            }
            state = readToken(tmp,inputFileptr);      //read the number of required labs
            //checking for leading zeros or negative number
            if(tmp[0] == '-' || tmp[0] == '0')
                invalidFlag = 1;
            
            numberRequiredLabs = strtol(tmp, &eptr, 10);    //convert to int
            //checking at least one lab
            if(numberRequiredLabs == 0)
                invalidFlag = 1;
            state = readToken(tmp,inputFileptr);      //read the number of students
            //checking for leading zeros or negative number
            if(tmp[0] == '-' || tmp[0] == '0')
                invalidFlag = 1;
            numberStudents = strtol(tmp, &eptr, 10);        //convert to int
            //checking at least one student
            if(numberStudents == 0)
                invalidFlag = 1;
            //set the curse in the list
            setCourse((coursesList+numCourses),courseName, numberRequiredLabs, numberStudents);
            //make the array list bigger
            coursesList = (Course*) realloc(coursesList, ((++numCourses)+1)*sizeof(Course));
            (coursesList+numCourses)->name = NULL;

        }
        //--------------------------------------------------------------------------
        //Read the professors
        for(int x = 0; ; x++){

            int numCourses_loc = 0;
            char *professorFirstName = (char*) calloc(1, MAXBUFFER);
            char *professorLastName  = (char*) calloc(1, MAXBUFFER);
            char **courses = (char**) calloc(1,sizeof(char *));        //store the array of courses

            int state;

            state = readToken(professorFirstName,inputFileptr);

            //if it is T, then it is the end
            if(professorFirstName[0] == 'T' && professorFirstName[1] == '\0'){
                // free(professorFirstName);
                // free(professorLastName);
                break;
            }
            state = readToken(professorLastName,inputFileptr);

            //check if the first or last name is T, P or S
            if(strcmp(professorFirstName,"T") == 0 || strcmp(professorLastName,"T") == 0 || strcmp(professorFirstName,"P") == 0 || strcmp(professorLastName,"P") == 0 ||strcmp(professorFirstName,"S") == 0 || strcmp(professorLastName,"S") == 0)
                invalidFlag = 1;
            
            //check that the name is only English letter not 
            for(int j = 0; j < MAXBUFFER && professorFirstName[j] != '\0'; j++){
                if(!((professorFirstName[j] >= 'a' && professorFirstName[j] <= 'z') ||(professorFirstName[j] >= 'A' && professorFirstName[j] <= 'Z'))){
                    invalidFlag = 1;
                    break;
                }
            }

            for(int j = 0; j < MAXBUFFER && professorLastName[j] != '\0'; j++){
                if(!((professorLastName[j] >= 'a' && professorLastName[j] <= 'z') ||(professorLastName[j] >= 'A' && professorLastName[j] <= 'Z'))){
                    invalidFlag = 1;
                    break;
                }
            }
            //read the courses untill it is the endline
            while(1){
                *(courses+numCourses_loc) = (char*) calloc(MAXBUFFER,sizeof(char));
                state = readToken(courses[numCourses_loc],inputFileptr);

                //Check that this course exists in the list of courses
                int found = 0;
                for(int j = 0; j < numCourses; j++){
                    if(strcmp(coursesList[j].name, courses[numCourses_loc]) == 0){
                        found = 1;
                        break;
                    }
                }
                if(found != 1){
                    invalidFlag = 1;
                }
                //check that the name is only English letter not 
                for(int j = 0; j < MAXBUFFER && courses[numCourses_loc][j] != '\0'; j++){
                    if(!((courses[numCourses_loc][j] >= 'a' && courses[numCourses_loc][j] <= 'z') ||(courses[numCourses_loc][j] >= 'A' && courses[numCourses_loc][j] <= 'Z'))){
                        
                        invalidFlag = 1;
                        break;
                    }
                }

                courses = (char**) realloc(courses, ((++numCourses_loc)+1)*sizeof(char*));
                //if it is endline, then it is end of courses lists
                if(state == 2)
                    break;
            }
            //if there is no given courses for him
            if(numCourses_loc == 0)
                invalidFlag = 1;

            setProfessor((professorsList+numProf),professorFirstName, professorLastName,courses,numCourses_loc);
            professorsList = (Professor*) realloc(professorsList, ((++numProf)+1)*sizeof(Professor));

        }
        //--------------------------------------------------------------------------
        //Start reading TAs
        for(int x = 0; ; x++){
            
            int numCourses_loc = 0;
            char *TAFirstName = (char*) calloc(1, MAXBUFFER*sizeof(char));
            char *TALastName  = (char*) calloc(1, MAXBUFFER*sizeof(char));
            char **courses = (char**) calloc(1,sizeof(char*));

            int state;

            state = readToken(TAFirstName,inputFileptr);
            if(TAFirstName[0] == 'S' && TAFirstName[1] == '\0'){
                break;
            }
            state = readToken(TALastName,inputFileptr);


            //check that the name is only English letter not 
            for(int j = 0; j < MAXBUFFER && TAFirstName[j] != '\0'; j++){
                if(!((TAFirstName[j] >= 'a' && TAFirstName[j] <= 'z') ||(TAFirstName[j] >= 'A' && TAFirstName[j] <= 'Z'))){
                    invalidFlag = 1;
                    break;
                }
            }
            for(int j = 0; j < MAXBUFFER && TALastName[j] != '\0'; j++){
                if(!((TALastName[j] >= 'a' && TALastName[j] <= 'z') ||(TALastName[j] >= 'A' && TALastName[j] <= 'Z'))){
                    invalidFlag = 1;
                    break;
                }
            }
            //check if the first or last name is T, P or S this will be invalid
            if(strcmp(TAFirstName,"T") == 0 || strcmp(TALastName,"T") == 0 || strcmp(TAFirstName,"P") == 0 || strcmp(TALastName,"P") == 0 ||strcmp(TAFirstName,"S") == 0 || strcmp(TALastName,"S") == 0)
                invalidFlag = 1;
            while(1){
                courses[numCourses_loc] = (char*) calloc(1, MAXBUFFER*sizeof(char));
                state = readToken(courses[numCourses_loc],inputFileptr);

                //Check that this course exists in the list of courses
                int found = 0;
                for(int j = 0; j < numCourses; j++){
                    if(strcmp(coursesList[j].name, courses[numCourses_loc]) == 0){
                        found = 1;
                        break;
                    }
                }
                if(found != 1){
                    invalidFlag = 1;
                }

                //check that the name is only English letter not 
                for(int j = 0; j < MAXBUFFER && courses[numCourses_loc][j] != '\0'; j++){
                    if(!((courses[numCourses_loc][j] >= 'a' && courses[numCourses_loc][j] <= 'z') ||(courses[numCourses_loc][j] >= 'A' && courses[numCourses_loc][j] <= 'Z'))){
                        
                        invalidFlag = 1;
                        break;
                    }
                }

                courses = (char**) realloc(courses, ((++numCourses_loc)+1)*sizeof(char*));
                //New line
                if(state == 2)
                    break;
            }
            //if there is no given courses for him
            if(numCourses_loc == 0)
                invalidFlag = 1;

            setTA((TAsList+numTA),TAFirstName, TALastName,courses,numCourses_loc);
            TAsList = (TA*) realloc(TAsList, ((++numTA)+1)*sizeof(TA));

        
        }
        //--------------------------------------------------------------------------
        //Start reading students
        for(int x = 0; ; x++){

            int numCourses_loc = 0;
            char *studentFirstName = (char*) calloc(1, MAXBUFFER);
            char *studentLastName  = (char*) calloc(1, MAXBUFFER);
            char *studentID        = (char*) calloc(1, MAXBUFFER);
            char **courses = (char**) calloc(1,sizeof(char*));

            
            int state;

            state = readToken(studentFirstName,inputFileptr);
            //if it end of file, then this is the end of students
            if(state == 3){
                break;
            }

            state = readToken(studentLastName,inputFileptr);
            state = readToken(studentID,inputFileptr);

            //check if the first or last name is T, P or S this will be invalid
            if(strcmp(studentFirstName,"T") == 0 || strcmp(studentLastName,"T") == 0 || strcmp(studentFirstName,"P") == 0 || strcmp(studentLastName,"P") == 0 ||strcmp(studentFirstName,"S") == 0 || strcmp(studentLastName,"S") == 0)
                invalidFlag = 1;
            
            //check that the name is only English letter not 
            for(int j = 0; j < MAXBUFFER && studentFirstName[j] != '\0'; j++){
                if(!((studentFirstName[j] >= 'a' && studentFirstName[j] <= 'z') ||(studentFirstName[j] >= 'A' && studentFirstName[j] <= 'Z'))){
                    invalidFlag = 1;
                    break;
                }
            }
            for(int j = 0; j < MAXBUFFER && studentLastName[j] != '\0'; j++){
                if(!((studentLastName[j] >= 'a' && studentLastName[j] <= 'z') ||(studentLastName[j] >= 'A' && studentLastName[j] <= 'Z'))){
                    invalidFlag = 1;
                    break;
                }
            }
            
            while(1){
                courses[numCourses_loc] = (char*) calloc(1, MAXBUFFER*sizeof(char));
                state = readToken(courses[numCourses_loc],inputFileptr);

                //Check that this course exists in the list of courses
                int found = 0;
                for(int j = 0; j < numCourses; j++){
                    if(strcmp(coursesList[j].name, courses[numCourses_loc]) == 0){
                        found = 1;
                        break;
                    }
                }
                if(found != 1){
                    invalidFlag = 1;
                }

                //check that the name is only English letter not 
                for(int j = 0; j < MAXBUFFER && courses[numCourses_loc][j] != '\0'; j++){
                    if(!((courses[numCourses_loc][j] >= 'a' && courses[numCourses_loc][j] <= 'z') ||(courses[numCourses_loc][j] >= 'A' && courses[numCourses_loc][j] <= 'Z'))){
                        
                        invalidFlag = 1;
                        break;
                    }
                }
                courses = (char**) realloc(courses, ((++numCourses_loc)+1)*sizeof(char*));
                //New line or end of file
                if(state == 2 || state == 3)
                    break;

            }
            //if there is no given courses for him
            if(numCourses_loc == 0)
                invalidFlag = 1;

            setStudent((studentsList+numStudents),studentFirstName, studentLastName, studentID, courses, numCourses_loc);
            studentsList = (Student*) realloc(studentsList, ((++numStudents)+1)*sizeof(Student));

        
        }

        //check uniqness of Course's name
        int uniqnessFlag = 0;
        for(int j = 0; j < numCourses; j++){
            for(int u = j+1; u < numCourses; u++){
                if(strcmp(coursesList[j].name,coursesList[u].name)==0){
                    uniqnessFlag = 1;
                    invalidFlag = 1;
                }
            }
            if(uniqnessFlag)
                break;
        }

        //check uniqness of Professor's name
        uniqnessFlag = 0;
        for(int j = 0; j < numProf; j++){
            for(int u = j+1; u < numProf; u++){
                if(strcmp(professorsList[j].firstName,professorsList[u].firstName)==0 && strcmp(professorsList[j].lastName,professorsList[u].lastName)==0){
                    uniqnessFlag = 1;
                    invalidFlag = 1;
                }
            }
            if(uniqnessFlag)
                break;
        }

        //check uniqness of TA's name
        uniqnessFlag = 0;
        for(int j = 0; j < numTA; j++){
            for(int u = j+1; u < numTA; u++){
                if(strcmp(TAsList[j].firstName,TAsList[u].firstName)==0 && strcmp(TAsList[j].lastName,TAsList[u].lastName)==0){
                    uniqnessFlag = 1;
                    invalidFlag = 1;
                }
            }
            if(uniqnessFlag)
                break;
        }



        uniqnessFlag = 0;
        for(int j = 0; j < numStudents; j++){
            for(int u = j+1; u < numStudents; u++){
                if(strcmp(studentsList[j].id,studentsList[u].id)==0){
                    uniqnessFlag = 1;
                    invalidFlag = 1;
                }
            }
            if(uniqnessFlag)
                break;
        }


        //--------------------------------------------------------------------------
        if(invalidFlag == 1){
            fprintf(outputFileptr,"%s","Invalid input.");
            if(coursesList != NULL)
                free(coursesList);
            if(professorsList != NULL)
                free(professorsList);
            if(TAsList != NULL)
                free(TAsList);
            if(studentsList != NULL)
                free(studentsList);
            fclose(inputFileptr);
            fclose(outputFileptr);
            continue;
        }
        //Allocate memory for schedule and badness to be passed to solve
        Schedule *schedule = (Schedule*) calloc(1, sizeof(Schedule));
        Badness *badness = (Badness*) calloc(BADNESSSIZE,sizeof(Badness));
        schedule->courses = (Course**) calloc((numCourses+1),sizeof(Course*));
        //initialize the schedule
        schedule->numCourses = 0;
        schedule->totalPoints = 0;

        //Allocate memory for bestschedule and bestbadness in order to be pplicable to be used in storing the optimal
        bestSchedule = (BestSchedule*) calloc(1, sizeof(BestSchedule));
        bestSchedule->courses = (BestCourse*) calloc((numCourses+1),sizeof(BestCourse));
        for(int x = 0; x < numCourses; x++){
            bestSchedule->courses[x].TAs = (TA*) calloc((numTA+1),sizeof(TA));  //array of TAs
            bestSchedule->courses[x].students = (Student*) calloc((numStudents+1),sizeof(Student));   //array of students
        }
        bestBadness = (BestBadness*) calloc(BADNESSSIZE,sizeof(Badness));

        //Call the function to start get all the possible combinations and compare it
        solve(coursesList,professorsList,TAsList,studentsList,schedule,badness);
        //ouptut the optimal solution
        formatOutput(bestSchedule,bestBadness,outputFileptr);

        //--------------------------------------------------------------------------
        //Free memory and close
        free(coursesList);
        free(professorsList);
        free(TAsList);
        free(studentsList);
        free(badness);
        free(bestSchedule);
        free(bestBadness);
        fclose(inputFileptr);
        fclose(outputFileptr);
    }
    //output the last file
    FILE *endFileptr = fopen("HanyHamedEmail.txt","w");
    fprintf(endFileptr,"h.hamed@innopolis.university\n");
    fclose(endFileptr);
}
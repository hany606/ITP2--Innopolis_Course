#ifndef SPEED_H_INCLUDED
#define SPEED_H_INCLUDED 1

//Usage of the static keyword???
//Why it is not working as variables
//Declare varibles inside header files

// int minSpeed = 1;
// int maxSpeed = 10;

#define minSpeed 1
#define maxSpeed 10

typedef struct speed
{
    int val;
}Speed;


int start(Speed);

#endif
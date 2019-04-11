#include "speed.h"

int start(Speed s){
    return s.val;
}

int getMaxSpeed(){
    return maxSpeed;
}

int isAuthorizedSpeed(Speed s){
    if(s.val >= minSpeed && s.val <= maxSpeed)
        return 1;
    return 0;
}

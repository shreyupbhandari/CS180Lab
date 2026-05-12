#ifndef PROJECT_H
#define PROJECT_H

#include <pthread.h> //To initialize a mutex.

//System State machine with the keyword enum, treats the states as integers.
typedef enum
{
    STATE_DISARMED=0,
    STATE_ARMED,
    STATE_TRIGGERED
} SystemState;

//The volatile keyword helps the compiler know that the the code necessary need not change the state. 
//Implemented to avoid compiler bugs.
extern volatile SystemState g_state;
extern volatile int g_alarm_silence;
extern pthread_mutex_t g_lock;

void take_picture();
#endif 
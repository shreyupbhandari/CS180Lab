#include <wiringPi.h>
#include <stdio.h>
#include "reed.h"

//Setup wrapper.
void reed_setup()
{
	pinMode(ReedPin, INPUT);
}	

//If both values are high, then the reed is open.
int reed_is_open()
{
    if(1 == digitalRead(ReedPin))
    {
        delay(10);
        if(1 == digitalRead(ReedPin))
        {
            return 1;
        }
    }
    return 0;
}


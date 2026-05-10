#include <wiringPi.h>
#include <stdio.h>
#include "reed.h"


void reed_setup()
{
	pinMode(ReedPin, INPUT);
}	

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


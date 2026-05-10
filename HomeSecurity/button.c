
#include <wiringPi.h>
#include "button.h"

void button_setup()
{
	pinMode(BtnPin, INPUT);
}
	

int button_is_pressed()
{
    if(0 == digitalRead(BtnPin))
    {
        delay(10);
        if(0 == digitalRead(BtnPin))
        {
            return 1;	
        }
    }
    return 0;   
}

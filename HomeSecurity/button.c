
#include <wiringPi.h>
#include "button.h"

void button_setup() //Setup wrapper
{
	pinMode(BtnPin, INPUT);
}
	

int button_is_pressed() //If both conditions are low then the button is pressed, very similar to SunFounder example.
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

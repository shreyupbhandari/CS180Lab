#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include "buzzer.h"
#include "project.h"


int song_1[] = {CM3,CM5,CM6,CM3,CM2,CM3,CM5,CM6,CH1,CM6,CM5,CM1,CM3,CM2,
				CM2,CM3,CM5,CM2,CM3,CM3,CL6,CL6,CL6,CM1,CM2,CM3,CM2,CL7,
				CL6,CM1,CL5};

int beat_1[] = {1,1,3,1,1,3,1,1,1,1,1,1,1,1,3,1,1,3,1,1,1,1,1,1,1,2,1,1,
				1,1,1,1,1,1,3};

int song_2[] = {CM1,CM1,CM1,CL5,CM3,CM3,CM3,CM1,CM1,CM3,CM5,CM5,CM4,CM3,CM2,
				CM2,CM3,CM4,CM4,CM3,CM2,CM3,CM1,CM1,CM3,CM2,CL5,CL7,CM2,CM1
				};

int beat_2[] = {1,1,1,3,1,1,1,3,1,1,1,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,3,3,2,3};

int buzzer_setup()
{
	if(softToneCreate(BuzPin) == -1){
		fprintf(stderr, "(BUZ) setup softToneCreate failed!");
		return -1; 
	}
    return 0;
}
void buzzer_alarm()
{
    int i;
	
    for(i=0;i<sizeof(song_1)/4;i++)
    {
        if (g_alarm_silence)
        {
            softToneWrite(BuzPin,0);
            return;
        }
        softToneWrite(BuzPin, song_1[i]);	
        delay(beat_1[i] * 500);
    }

    for(i=0;i<sizeof(song_2)/4;i++)
    {
        if (g_alarm_silence)
        {
            softToneWrite(BuzPin,0);
            return 0;
        }
        softToneWrite(BuzPin, song_2[i]);	
        delay(beat_2[i] * 500);
    }	
}

void buzzer_off()
{   
    
    softToneStop(BuzPin, 0);
}

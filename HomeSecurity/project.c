#include "threads.h"
#include "project.h"
#include "lcd.h"
#include "led.h"
#include "buzzer.h"
#include "reed.h"
#include "button.h"
#include "ir.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include <unistd.h>
#include <sys/wait.h>

volatile SystemState g_state = STATE_DISARMED;
volatile int g_alarm_silence = 0;
pthread_mutex_t g_lock;

void take_picture()
{
    printf("(CAM) Capturing image.");
    int pid = fork();
    if (pid==0)
    {
        char *args[]={"rpicam-still","-o","/home/piisnotinfinite/recent.jpg",NULL};
        execvp(args[0],args);
        perror("(CAM) Exec failed!");
    }
    else
    {
        wait(NULL);
        printf("Camera image saved!\n");
    }
}

int main()
{
    printf("=================================\n");
    printf("Shreyup's Home Security System \n");
    printf("=================================\n");


    //Setting wiringPi up.
    if (wiringPiSetup()==-1)
    {
        fprintf(stderr, "(MAIN) wiringPiSetup() failed.\n");
        return 1;
    }

    led_setup();
    led_off();

    if (buzzer_setup() == -1) 
    {
        fprintf(stderr, "(MAIN) Buzzer setup failed, continuing without it.\n");
    }

    if (lcd_setup() == -1) 
    {
        fprintf(stderr, "(MAIN) LCD setup failed, display disabled.\n");
    } 
    else 
    {
        lcd_write(0, 0, "Security System ");
        lcd_write(0, 1, "Initializing... ");
        sleep(1);
    }
    reed_setup();
    button_setup();
    ir_setup();

    pthread_t t_lcd, t_ir, t_reed, t_buzzer, t_button;

    //Could have created a helper function to make the error handling a little less redundant but the args would be too complicated.
    //Sth like void* (*fn) (void *) which translates to a pointer to a function that takes a void pointer as an argument and returns a void pointer? OMG
    if (pthread_create(&t_lcd, NULL, thread_lcd, NULL)!=0)
    {
        fprintf(stderr,"(MAIN) Failed to create thread for LCD.\n");
        exit(1);
    }
    
    if (pthread_create(&t_ir, NULL, thread_ir, NULL)!=0)
    {
        fprintf(stderr,"(MAIN) Failed to create thread for IR.\n");
        exit(1);
    }
    printf("(MAIN) Started thread: LCD\n");

    if (pthread_create(&t_reed, NULL, thread_reed, NULL)!=0)
    {
        fprintf(stderr,"(MAIN) Failed to create thread for Reed Switch.\n");
        exit(1);
    }
    printf("(MAIN) Started thread: Reed Switch\n");

    if (pthread_create(&t_buzzer, NULL, thread_buzzer, NULL)!=0)
    {
        fprintf(stderr,"(MAIN) Failed to create thread for Buzzer.\n");
        exit(1);
    }
    printf("(MAIN) Started thread: Buzzer\n");

    if (pthread_create(&t_button, NULL, thread_button, NULL)!=0)
    {
        fprintf(stderr,"(MAIN) Failed to create thread for button.\n");
        exit(1);
    }
    printf("(MAIN) Started thread: Button\n");

    printf("(MAIN) All threads are running.\n");
    printf("(MAIN) Press any button on the remote to ARM the system.\n");
    printf("(MAIN) Press Ctrl-C to shut down.\n\n");

    pthread_join(t_lcd,   NULL);
    pthread_join(t_ir,    NULL);
    pthread_join(t_reed,  NULL);
    pthread_join(t_buzzer, NULL);
    pthread_join(t_button,  NULL);

    //Executed only when the threads returns, which shouldn't happen!
    lcd_clear();
    led_off();
    buzzer_off();

    return 0;
}

#include "threads.h"
#include "project.h"
#include "lcd.h"
#include "led.h"
#include "buzzer.h"
#include "reed.h"
#include "button.h"
#include "ir.h"

#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void *thread_lcd(void *arg)
{
    char row0[17]; 
    char row1[17];
    time_t     now;
    struct tm *tm_info;

    while (1) {
        
        pthread_mutex_lock(&g_lock);
        SystemState s = g_state;
        pthread_mutex_unlock(&g_lock);

        switch (s) {
            case STATE_DISARMED:
                strncpy(row0, "Status:DISARMED ", 17); 
                break;
            case STATE_ARMED:
                strncpy(row0, "Status:ARMED    ", 17);
                 break;
            case STATE_TRIGGERED:
                strncpy(row0, "Status:TRIGGERED", 17); 
                break;
        }
        row0[16] = '\0';

       
        time(&now);
        tm_info = localtime(&now);
        strftime(row1, 17, "%m/%d %H:%M:%S", tm_info);

        lcd_clear();
        lcd_write(0, 0, row0);
        lcd_write(0, 1, row1);

        sleep(1);
    }
    return NULL;
}

void *thread_ir(void *arg)
{
    printf("(IR)Thread started. Waiting for remote button press.\n");

    while (1)
    {
        if (g_ir_triggered)
        {
            g_ir_triggered = 0;

            pthread_mutex_lock(&g_lock);
            switch (g_state)
            {
            case STATE_DISARMED:
                g_state = STATE_ARMED;
                led_green();
                printf("(IR): ARMED\n");
                break;

            case STATE_ARMED:
                g_state = STATE_DISARMED;
                led_off();
                printf("(IR): DISARMED\n");
                break;

            case STATE_TRIGGERED:
                g_alarm_silence = 1;
                g_state = STATE_DISARMED;
                led_off();
                printf("(IR): SILENCED and DISARMED\n");
                break;
            }
            pthread_mutex_unlock(&g_lock);
        }
        delay(50);
    }
    return NULL;
}


void *thread_reed(void *arg)
{
    int prev_open = 0;  

    printf("(REED) Thread started.\n");

    while (1) {
        int cur_open = reed_is_open();

        pthread_mutex_lock(&g_lock);
        SystemState s = g_state;
        pthread_mutex_unlock(&g_lock);

        if (cur_open && !prev_open && s == STATE_ARMED) {
            printf("(REED) Triggered while ARMED, triggering alarm!\n");

            pthread_mutex_lock(&g_lock);
            g_state = STATE_TRIGGERED;
            g_alarm_silence = 0;
            pthread_mutex_unlock(&g_lock);

            take_picture();
        }

        prev_open = cur_open;
        delay(50); 
    }
    return NULL;
}


void *thread_buzzer(void *arg)
{
    int was_triggered = 0;

    printf("(BUZZER) Thread started.\n");

    while (1) {
        pthread_mutex_lock(&g_lock);
        SystemState s   = g_state;
        int         sil = g_alarm_silence;
        pthread_mutex_unlock(&g_lock);

        if (s == STATE_TRIGGERED) 
        {
            pthread_mutex_lock(&g_lock);
            g_alarm_silence=0;
            pthread_mutex_unlock(&g_lock);
            led_red();
            buzzer_alarm(); 
            led_off();
            delay(200);
        } 
        else 
        {
            buzzer_off();
            delay(100);
        }
    }
    return NULL;
}

void *thread_button(void *arg)
{
    int prev_pressed = 0;
    printf("(BELL) Thread started.\n");

    while (1) 
    {
        int cur_pressed = button_is_pressed();

        if (cur_pressed && !prev_pressed) {
            printf("(BELL) Doorbell button pressed, taking photo.\n");

            
            led_green();
            delay(300);

            pthread_mutex_lock(&g_lock);
            SystemState s = g_state;
            pthread_mutex_unlock(&g_lock);

            if (s == STATE_ARMED)
            {
                led_green();
            }
            else if (s == STATE_TRIGGERED)
            {
                 led_red();
            }
            else
            {
                led_off();
            }

            take_picture();
        }

        prev_pressed = cur_pressed;
        delay(50);
    }
    return NULL;
}

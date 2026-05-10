#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include "led.h"

void ledInit(void)
{
	softPwmCreate(LedPinRed,  0, 100);
	softPwmCreate(LedPinGreen,0, 100);
}

void ledColorSet(uchar r_val, uchar g_val)
{
	softPwmWrite(LedPinRed,   r_val);
	softPwmWrite(LedPinGreen, g_val);
}

void led_setup(void)
{
    ledInit();
}

void led_red(void)
{
    ledColorSet(100, 0);
}

void led_green(void)
{
    ledColorSet(0, 100);
}

void led_off(void)
{
    ledColorSet(0, 0);
}

void led_set(uchar r_val, uchar g_val)
{
    ledColorSet(r_val, g_val);
}


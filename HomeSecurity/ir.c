#include <wiringPi.h>
#include <stdio.h>
#include "ir.h"

volatile int g_ir_triggered = 0;

void myISR(void)
{
    g_ir_triggered = 1;
}

void ir_setup(void)
{
    if (wiringPiISR(IR, INT_EDGE_FALLING, &myISR) == -1)
    {
        printf("setup ISR failed!\n");
    }
}

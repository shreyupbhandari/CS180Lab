#ifndef LED_H
#define LED_H

#define uchar unsigned char
#define LedPinRed    2
#define LedPinGreen  3

void led_setup(void);


void led_red(void);
void led_green(void);
void led_off(void);
void led_set(uchar r_val, uchar g_val);

#endif

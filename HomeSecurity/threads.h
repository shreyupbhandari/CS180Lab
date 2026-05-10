
#ifndef THREADS_H
#define THREADS_H

void *thread_lcd(void *arg);
void *thread_ir(void *arg);
void *thread_reed(void *arg);
void *thread_buzzer(void *arg);
void *thread_button(void *arg);

#endif 

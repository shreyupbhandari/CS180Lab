#ifndef IR_H
#define IR_H

#define IR 1

extern volatile int g_ir_triggered;

void myISR(void);
void ir_setup(void);

#endif
#ifndef IR_H
#define IR_H

#define IR 1 //GPIO for IR.

extern volatile int g_ir_triggered; //external volatile to combat compiler optimizations.

void myISR(void);
void ir_setup(void);

#endif
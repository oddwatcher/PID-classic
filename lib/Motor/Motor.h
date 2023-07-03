#ifndef MOTOR_T
#define MOTOR_T
#endif
struct motor
{
    int mass;
    int maxa;
    int v;
    int maxv;
    int pos;
} motor;


void motoract(int f, struct motor *mot, int tick);

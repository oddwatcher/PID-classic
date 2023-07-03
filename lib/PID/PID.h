#ifndef PID_C
#define PID_C
#endif
//anti overflow interation

typedef struct PID
{
    int p;
    int i;
    int d;
    int I;
    int last;
    int setpoint;
    int current;
    int output;
    int *outaddr;
    int *inaddr;
    int *setaddr;
} PID;
PID *PIDinit(int *inaddr, int *outaddr, int *setaddr, int p, int i, int d);
void PIDupdate(PID *p, int tick);
void inter(int d, int *I, int tickms); 


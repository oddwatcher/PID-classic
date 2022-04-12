/* This is a C99 style PID program using  */

/*for differrnt system change data type*/
#ifndef PID
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
#endif
#include<stdlib.h>

void inter(int d, int *I, int tickms) //anti overflow interation
{
    int i = *I;
    *I = (*I + d * tickms);
    if (d < 0)
    {
        if (*I > i)
        {
            *I = -(__INT32_MAX__);
        }
    }
    else if (*I < i)
    {
        *I = __INT32_MAX__;
    }
    if(*I>__INT32_MAX__){
        
    }
}

PID *PIDinit(int *inaddr, int *outaddr, int *setaddr, int p, int i, int d)
{
    PID *temp = (PID *)malloc(sizeof(PID));
    temp->p = p;
    temp->i = i;
    temp->d = d;
    temp->inaddr = inaddr;
    temp->outaddr = outaddr;
    temp->setaddr = setaddr;
    temp->I = 0;
    temp->last = 0;
    return temp;
}

void PID_IO(PID *p)
{
    p->current = *(p->inaddr);
    *(p->outaddr) = p->output;
    p->setpoint = *(p->setaddr);
}

void PIDupdate(PID *p, int tick)
{
    PID_IO(p);
    int Err = p->setpoint - p->current;
    int D = ((Err - p->last) / tick);
    p->last = Err;
    inter(Err, &(p->I), tick);
    p->output = ((p->p) * (Err*1000 + (p->i) * (p->I/10000) + (p->d) * D)) / 1000;
}
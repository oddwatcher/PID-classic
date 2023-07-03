#include"PID.h"

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
    int D = ((Err - p->last)*100 / tick);
    p->last = Err;
    inter(Err, &(p->I), tick);
    p->output = ((p->p) * (Err + (p->i) * (p->I)/10000 + ((p->d) * D)/100)) / 1000;
    printf("Err=%d,I=%d,D=%d\n",Err,(p->i) * (p->I)/10000,(p->d) * D/100);
}

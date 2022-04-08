#include <stdio.h>
#include <stdlib.h>
int inter(int d, int tickms)
{
    static int I = 0;
    int i = I;
    I = I + d * tickms;
    if (d < 0)
    {
        if (I > i)
        {
            I = -(__INT32_MAX__);
        }
    }
    else if (I < i)
    {
        I = __INT32_MAX__;
    }
    return I;
}

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
} PID;

PID *PIDinit(int *inaddr, int *outaddr, int p, int i, int d)
{
    PID *temp = (PID *)malloc(sizeof(PID));
    temp->p = p;
    temp->i = i;
    temp->d = d;
    temp->inaddr = inaddr;
    temp->outaddr = outaddr;
    temp->I = 0;
    temp->last = 0;
    return temp;
}

void PID_IO(PID *p)
{
    p->current = *(p->inaddr);
    *(p->outaddr) = p->output;
}

void PIDupdate(PID *p, int tick)
{
    
    int Err = p->setpoint - p->current;
    int D = (Err - p->last) / tick;
    p->last = Err;
    p->I = inter(Err, tick);
    p->output = (p->p * (Err + (p->i) * (p->I) + (p->d) * D)) / 1000;
    PID_IO(p);
}



int main()
{
    int din;
    int dout;
    int tick;
    FILE *Din;
    freopen("./Din.txt", "r", Din);
    FILE *Dout;
    freopen("./Dout.txt", "w", Dout);
    PID *pid1 = PIDinit(&din, &dout, 100, 10, 10);
    while (feof(Din))
    {
        fscanf(Din, "%d,%d", &din, &tick);
        pid1->current = din;
        PIDupdate(pid1, tick);
        dout = pid1->output;
        fprintf(Dout, "%d", dout);
    }
    fclose(Din);
    fclose(Dout);
}

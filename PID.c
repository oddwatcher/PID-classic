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
    int *setaddr;
} PID;

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

    int Err = p->setpoint - p->current;
    int D = (Err - p->last) / tick;
    p->last = Err;
    p->I = inter(Err, tick);
    p->output = (p->p * (Err + (p->i) * (p->I) + (p->d) * D)) / 1000;
    PID_IO(p);
}

struct motor
{
    int mass;
    int maxa;
    int v;
    int maxv;
    int pos;
} mot1;

void motoract(int f, struct motor *mot, int tick)
{
    int a = f / (mot->mass);
    if ((a > 0 ? a : -a) > (mot->maxa))
    {
        a = a > 0 ? mot->maxa : -(mot->maxa);
    }
    mot->pos = mot->pos + mot->v * tick + (int)(0.5 * a * tick * tick);
    mot->v = mot->v + a * tick;
    if ((mot->v > 0 ? mot->v : -(mot->v)) > (mot->maxv))
    {
        mot->v = mot->v > 0 ? mot->maxv : -(mot->maxv);
    }
}

int main()
{
    int din;
    int dout;
    int tick;
    int motor;
    mot1.mass=1000;
    mot1.maxa=1000;
    mot1.maxv=3000;
    mot1.pos=0;
    mot1.v=0;
    FILE *Din;
    freopen("Din.txt", "r", Din);
    FILE *Dout;
    freopen("Dout.txt", "w", Dout);
    PID *pid1 = PIDinit(&(mot1.pos),&motor,&din, 100, 10, 10);
    while (feof(Din))
    {
        fscanf(Din, "%d,%d", &din, &tick);
        PIDupdate(pid1, tick);
        motoract(motor,&mot1,tick);
        fprintf(Dout, "%d,%d\n", mot1.pos,mot1.v);
    }
    fclose(Din);
    fclose(Dout);
}

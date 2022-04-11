#include <stdio.h>
#include <stdlib.h>
void inter(int d, int *I, int tickms)
{
    int i = *I;
    *I = (*I + d * tickms / 1000);
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
    inter(Err, &(p->I), tick);
    p->output = ((p->p) * (Err + (p->i) * (p->I) + (p->d) * D)) / 10;
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
    mot->pos = mot->pos + (mot->v * tick + (int)(0.5 * a * tick * tick)) / 1000;
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
    int motor = 0;
    mot1.mass = 10;
    mot1.maxa = 100;
    mot1.maxv = 30;
    mot1.pos = 0;
    mot1.v = 0;
    FILE *Din;
    Din = fopen("Din.txt", "r");
    FILE *Dout;
    Dout = fopen("Dout.txt", "w");
    PID *pid1 = PIDinit(&(mot1.pos), &motor, &din, 80, 1000, 0);
    while (!feof(Din))
    {
        fscanf(Din, "%d,%d", &din, &tick);
        PIDupdate(pid1, tick);
        motoract(motor, &mot1, tick);
        fprintf(Dout, "%d,%d,%d\n", mot1.pos, mot1.v, motor);
        printf("%d,%d,%d,%d\n", din, mot1.pos, mot1.v, motor);
    }
    fclose(Din);
    fclose(Dout);
}

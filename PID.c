#include <stdio.h>
#include <stdlib.h>
//PID service use PIDinit to establish a new memory space for it ,and this function returns an address of the pid data, init requires the address of setpoint,input and output
//PID update use address stored in pidstruct so no need to specify the inouts. 
//PID IO processes the inout progress and may add custom prog to it such as structured input
//the data feed in at the specifyed address should be int and mutiplyed 1000(if on 8 bit shoud reduce to 10 or 0 ) the program run in strict int for imbedding
//
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
    PID_IO(p);
    int Err = p->setpoint - p->current;
    int D = ((Err - p->last) / tick);
    p->last = Err;
    inter(Err, &(p->I), tick);
    p->output = (((p->p)/1000) * (Err + (p->i) * (p->I/1000) + (p->d) * D)) / 1000;
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
    mot->pos = mot->pos + (mot->v * tick + (int)(a * tick * tick)/2000);
    mot->v = mot->v + (a * tick/1000);
    if ((mot->v > 0 ? mot->v : -(mot->v)) > (mot->maxv))
    {
        mot->v = mot->v > 0 ? mot->maxv : -(mot->maxv);
    }
    printf("force:%d,acc:%d,speed:%d\n",f,a,mot->v);
}

int main()
{
    int din;
    int dout;
    int P,I,D;
    int tick;
    int motor = 0;
    mot1.mass = 10;
    mot1.maxa = 100;
    mot1.maxv = 30;
    mot1.pos = 0;
    mot1.v = 0;
    scanf("%d,%d,%d",&P,&I,&D);
    FILE *Din;
    Din = fopen("Din.txt", "r");
    FILE *Dout;
    Dout = fopen("Dout.txt", "w");
    PID *pid1 = PIDinit(&(mot1.pos), &motor, &din, P, I, D);
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

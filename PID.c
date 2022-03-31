#include <stdio.h>
#include <stdlib.h>
int inter(int *d, int tickms)
{
    static int I = 0;
    int i = I;
    I = I + *d * tickms;
    if(*d<0){
        if(I>i){
            I = -(__INT32_MAX__);
        }
    }else if(I<i){
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
    int output;
    int *outaddr;
    int *inaddr;
}PID;

PID*PIDinit(int*inaddr,int*outaddr,int p ,int i,int d){
    PID*temp = (PID*)malloc(sizeof(PID));
    temp->p = p;
    temp->i = i;
    temp->d = d;
    temp->inaddr = inaddr;
    temp->outaddr = outaddr;
    temp->I=0;
    temp->last=0;
    return temp;
}

void PIDupdate(PID*p,int tick){
    int Err = p->setpoint - *(p->inaddr);
    p->I=inter(&Err,tick);
    p->output =(p->p)*(Err+(p->i)*(p->I)+(p->d)*(Err-(p->last))/tick);
    p->last=Err;
    *(p->outaddr) = p->output;
}

int main()
{
    int i;
    while (1)
    {
        scanf("%d", &i);
        i = inter(i, 20);
        printf("%d\n", i);
    }
    return 0;
}

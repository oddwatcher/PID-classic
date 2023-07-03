#include "Motor.h"


void motoract(int f, struct motor *mot, int tick){
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
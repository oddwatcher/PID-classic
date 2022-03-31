/* This is a C99 style PID program using  */

/*for differrnt system change data type*/
#define PID_d_type int
typedef struct PID
{
    PID_d_type p;
    PID_d_type i;
    PID_d_type d;
    PID_d_type I;
    PID_d_type exit;
    PID_d_type setpoint;
    PID_d_type output;
}PID;

void updatePID(PID* p,PID_d_type Din,,PID_d_type Exit){
    PID_d_type D = Din - p->setpoint;
    Din = p->setpoint - Din;
    p->I = p->I+Din-p->exit;
    p->exit = Exit;
    p->output = p->p*(Din + p->i*p->I+p->d*(Din-p->setpoint));
}
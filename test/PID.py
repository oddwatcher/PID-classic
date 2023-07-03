from urllib.parse import MAX_CACHE_SIZE
from numpy import *
import matplotlib.pyplot as plt
class PID:
    p=0
    i=0
    d=0
    I=0
    last=0
    setpoint=0
    def inter(self,din,tick):
        self.I=self.I+din*tick
    def __init__(s,p,i,d):
        s.p=p
        s.d=d
        s.i=i
    def pidupdate(s,tick,setpoint,current):
        err = setpoint-current
        D = int((err-s.last)/tick)
        s.last=err
        s.inter(err,tick)
        return (int(s.p*(err+s.i*s.I+s.d*D)/1000))
class motor:
    mass=1000
    maxa=1000
    maxv=1000
    pos=0
    v=0
    def act(s,f,tick):
        a = int(f/s.mass)
        if(abs(a)>s.maxa):
            if(a>0):
                a=s.maxa
            else:
                a=-s.maxa
        s.pos = s.pos + 1/2

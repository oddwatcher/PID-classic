from numpy import *
x = arange(0,10,0.01)
y = 1000*sin(x)
din = open("./Dout.txt","w")
for i in y:
    a = int(i)
    b = str(a)
    din.write(b)
    din.write(",")
    din.write("12\n")
din.close


import numpy as np
import matplotlib.pyplot as plt
Din = open("Dout.txt",'r')
din = Din.read()
data=din.splitlines(False)
pic = np.zeros((2,1001))
n=0
for i in data:
    print(i)
    p=i.split(',')
    pic[0,n]=int(p[0])
    pic[1,n]=int(p[1])
    n = n+1

plt.plot(np.arange(0,1001),pic[0])
x = np.arange(0,10.01,0.01)
y = 1000*np.sin(x)
plt.plot(np.arange(0,1001),y)
plt.show()

import serial
import time
import sys
data=""
ser=serial.Serial("/dev/ttyUSB0",9600)
ser.flushInput()
ser.flushOutput() 
time.sleep(2)
print sys.argv[1]
ser.write(sys.argv[1])

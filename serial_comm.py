#! python3

from time import sleep
from datetime import datetime
import sys
import glob
import serial
from serial import SerialException
#import numpy as np
from time import time
import time
from datetime import datetime

def node_reset(ser1):
	command=1
	ser1.write(str.encode('1'))
	while True:
		if ser1.in_waiting>0:
			inputvalue=ser1.readline()
			print(inputvalue)
			if inputvalue in return_list:
				print(inputvalue)
				break
		
		
def node_start(ser1,filename):
	command=2
	ser1.write(str.encode('2'))
	while True:
		name=datadir+filename+'.csv'
		f1=open(name,'a')
		if ser1.in_waiting>0:
			inputvalue=ser1.readline()
			if inputvalue in return_list:
				print(inputvalue)
				f1.close()
				break
			else:
				f1.write(str(inputvalue))
				f1.write('\n')
				#print(inputvalue)
			
		

portenv='/dev/ttyUSB1'
serenv=serial.Serial(portenv, 38400)
time.sleep(2)
print("initialize")
serenv.flushInput()
return_list=[b'command 1 complete\r\n', b'command 2 complete\r\n']	#this list is to determine when to return from the while true reading loop
datadir='/home/pi/dual_cs_control/data/'		#data stroage dir

if __name__=="__main__":
	while True:
		command=input("Type in a command, 1 for reset, 2 for start: ")
		print(command)
		if command=='1':
			node_reset(serenv)	#command 1
		elif command=='2':
			filename=input("Type in filename: ")
			node_start(serenv, filename)	#command 2
		else: 
			print('wrong command')

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
import threading
from threading import Thread


def node1_initial():
	ser1.write(str.encode('1'))
	while True:
		if ser1.in_waiting>0:
			inputvalue=ser1.readline()
			if inputvalue in return_list:
				print(inputvalue)
				break
			else:
				print(inputvalue)


def node2_initial():
	ser2.write(str.encode('1'))
	while True:
		if ser2.in_waiting>0:
			inputvalue=ser2.readline()
			if inputvalue in return_list:
				print(inputvalue)
				break
			else:
				print(inputvalue)


def node1_start(filename):
	ser1.write(str.encode('2'))
	while True:
		name=datadir+filename+'_env.csv'
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


def node2_start(filename):
	ser2.write(str.encode('2'))
	while True:
		name=datadir+filename+'_occ.csv'
		f2=open(name,'a')
		if ser2.in_waiting>0:
			inputvalue=ser2.readline()
			if inputvalue in return_list:
				print(inputvalue)
				f2.close()
				break
			else:
				f2.write(str(inputvalue))
				f2.write('\n')

def node1_reset():
	ser1.write(str.encode('3'))
	
def node2_reset():
	ser2.write(str.encode('3'))


def node_reset(ser1,ser2):
	command=1
	ser1.write(str.encode('1'))
	ser2.write(str.encode('1'))
	while True:
		if ser1.in_waiting>0 or ser2.in_waiting>0:
			ser1value=ser1.readline()
			ser2value=ser2.readline()
			print('ser1'+ser1value)
			print('ser2'+ser2value)
			if ser1value in return_list:
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
			
		

portocc='/dev/ttyUSB0'
#serenv=serial.Serial(portenv, 38400)
serocc=serial.Serial(portocc, 38400)
time.sleep(2)
print("initialize")
#serenv.flushInput()
serocc.flushInput()
return_list=[b'command 1 complete\r\n', b'command 2 complete\r\n']	#this list is to determine when to return from the while true reading loop
datadir='/home/pi/dual_cs_data/data0314/'		#data stroage dir

if __name__=="__main__":
	#ser1=serenv
	ser2=serocc
	while True:
		command=input("Type in a command, 1 for initial, 2 for start, 3 for reset, 4 for close ports: ")
		print(command)
		if command=='1':
			#n1_initial=Thread(target=node1_initial)
			n2_initial=Thread(target=node2_initial)
			#n1_initial.start()
			n2_initial.start()
		elif command=='2':
			filename=input("Type in filename: ")
			#n1_start=Thread(target=node1_start, args=(filename,))
			n2_start=Thread(target=node2_start, args=(filename,))
			
			#n1_start.start()
			n2_start.start()
		elif command=='3':
			print('We will reset both arduinos')
			#n1_reset=Thread(target=node1_reset)
			n2_reset=Thread(target=node2_reset)
			#n1_reset.start()
			n2_reset.start()
		elif command=='4':
			print('Ports will be closed, please use CTRL+C to exit the process')
			#serenv.close()
			serocc.close()
		else: 
			print('wrong command')

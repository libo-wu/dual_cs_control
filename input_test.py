#! python3

if __name__=="__main__":
	while True:
		command=input("Type in a command, 1 for reset, 2 for start: ")
		if command=='1':
			node_reset(serenv,serocc)
		elif command=='2':
			filename=input("Type in filename: ")
			node_start(serenv,serocc, filename)
		else: 
			print('wrong command')

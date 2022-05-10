from socket import *
import threading

def fun():
	with socket(AF_INET, SOCK_STREAM) as s:
		f = open('log.txt', 'w')
		s.connect(("172.20.10.7", 8086))
		print("dddd")
		while True:
			recvdata = s.recv(1024).decode("utf-8")
			if len(recvdata) != 0:
				print(recvdata)
				f.write(recvdata)

t = threading.Thread(target=fun)
t.start()




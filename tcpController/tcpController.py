import socket
import time

hostname = '192.168.43.54'

def main():
	print 'begin to connect...'
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
	s.connect((hostname, 7000))
	print 'connected.'
	s.send('forward')
	response = s.recv(1024)
	print str(response)
	print 'forward sent.'

	time.sleep(10)

	s.send('backward')
	response = s.recv(1024)
	print str(response)
	print 'backward sent.'
	time.sleep(10)

	s.send('turnLeft')
	response = s.recv(1024)
	print str(response)
	print 'left sent.'
	time.sleep(10)

	s.send('turnRight')
	response = s.recv(1024)
	print str(response)
	print 'right sent.'
	time.sleep(10)

	s.close()

if __name__ == "__main__":
    main()
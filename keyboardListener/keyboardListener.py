import msvcrt
import time
import serial
import logging
import socket

FORMAT = '%(asctime)-15s %(message)s'
logging.basicConfig(format=FORMAT)

logger = logging.getLogger('keyboard.listener')
logger.setLevel(logging.INFO)

def kbfunc():
    #this is boolean for whether the keyboard has bene hit
    x = msvcrt.kbhit()
    if x:
        #getch acquires the character encoded in binary ASCII
        ret = msvcrt.getch()
        print ("key: " + str(ret.decode()) + " pressed.")
    else:
        ret = False
    return ret


hostname = '192.168.43.54'

def main():
    # ser = serial.Serial('COM9', 9600, timeout=5)
    # logger.info(ser.name)

    print 'begin to connect...'
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
    s.connect((hostname, 7000))
    print 'connected.'

    #infinite loop
    while True:

        #acquire the keyboard hit if exists
        x = kbfunc()

        if x == False:
            time.sleep(0.5)
        elif x.decode() == 'w':
            print ("forward.")
            s.send('forward')
            response = s.recv(1024)
            print str(response)
        elif x.decode() == 's':
            print ("backward.")
            s.send('backward')
            response = s.recv(1024)
            print str(response)
        elif x.decode() == 'a':
            print ("turnLeft.")
            s.send('turnLeft')
            response = s.recv(1024)
            print str(response)
        elif x.decode() == 'd':
            print ("turnRight.")
            s.send('turnRight')
            response = s.recv(1024)
            print str(response)
        elif x.decode() == 'q':
            s.send('stop')
            print ("stop.")
        else:
            time.sleep(0.5)


if __name__ == "__main__":
    main()
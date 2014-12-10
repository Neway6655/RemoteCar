import msvcrt
import time
import serial
import logging

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

def main():
    ser = serial.Serial('COM9', 9600, timeout=5)
    logger.info(ser.name)

    #infinite loop
    while True:

        #acquire the keyboard hit if exists
        x = kbfunc()

        if x == False:
            time.sleep(0.5)
        elif x.decode() == 'w':
            print ("Forward.")
            ser.write("1")
        elif x.decode() == 's':
            print ("Backward.")
            ser.write("2")
        elif x.decode() == 'a':
            print ("Turn left.")
            ser.write("3")
        elif x.decode() == 'd':
            print ("Turn right.")
            ser.write("4")
        elif x.decode() == 'q':
            print ("Quit.")
            break;
        else:
            time.sleep(0.5)

    ser.close


if __name__ == "__main__":
    main()
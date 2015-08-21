import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
from lib_nrf24 import NRF24
import time
import spidev



pipes = [[0xe7, 0xe7, 0xe7, 0xe7, 0xe7], [0xc2, 0xc2, 0xc2, 0xc2, 0xc2]]

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 17)
time.sleep(1)
radio.setRetries(15,15)
radio.setPayloadSize(32)
radio.setChannel(0x60)

radio.setDataRate(NRF24.BR_2MBPS)
radio.setPALevel(NRF24.PA_MIN)
radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()


radio.openWritingPipe(pipes[1])
radio.openReadingPipe(1, pipes[0])
radio.printDetails()


c=1
x=0
while True:
    print "use 'w' 's' 'd' 'a' to steer the car "
    ster = raw_input("write command   ")
    buf = [ster,c]
    #c = (c + 1) & 255
    # send a packet to receiver
    x=0
    while x<10:
        radio.write(buf)
        print ("Sent:"),
        print (buf)
        # did it return with a payload?
        if radio.isAckPayloadAvailable():
            pl_buffer=[]
            radio.read(pl_buffer, radio.getDynamicPayloadSize())
            print ("Received back:"),
            print (pl_buffer)
        else:
            print ("Received: Ack only, no payload")
        x=x+1


    #include <SPI.h>
    #include "nRF24L01.h"
    #include "RF24.h"
    #include "printf.h"
    //  #include "elapsedMillis.h"
    #include "Arduino.h"
    #include<stdio.h>
    #include<stddef.h>
    #include<limits.h>
    //
    // Hardware configuration
    //sudo python example.nrf24-send-rpi.py

    RF24 radio2(7,10);     // pong-back

    const uint64_t pipe =  0xC2C2C2C2C2LL;

    char w = 'w'; 

    void setup(void)
    {
      pinMode(2,OUTPUT);
      pinMode(3,1);//OUTPUT
      pinMode(4,1);
      pinMode(5,1);

      Serial.begin(57600);
      printf_begin();
      printf("\n\rRF24 rx\n\r");    ///////////////////////
      radio2.begin();
      radio2.setDataRate(RF24_2MBPS);
      radio2.setPALevel(RF24_PA_MAX);
      radio2.setAutoAck(true);
      radio2.setChannel(0x60);
      radio2.enableAckPayload();
      radio2.openReadingPipe(1,pipe);
      radio2.startListening();
      radio2.printDetails();    ///////////////////////
    }

    struct txpanel {

        char a = 5;
        char c[10];
        char b = 7;

    } txp;

    char rxbuf[32] ;
    char rx_size =0;
    bool _rx_available = false;

    bool rxAvailable()
    {
        // not secure, but adequate?
        bool result = _rx_available;
        _rx_available = false;
        return result;
    }

    void Radio2()
    {
        static char _rxbuf2[32];
        //delay(700);

        if ( radio2.available() )
        {
          bool done = false;
          while (!done)
          {
            rx_size = radio2.getDynamicPayloadSize();
            done = radio2.read( _rxbuf2 ,rx_size);

            if (_rxbuf2[0] != 0)
            // RULE: all incoming radio payloads should have non-zero first char to be a valid info packet.
            // RULE: no assumption re null terminators is made. Binary treatment.
            {
                _rx_available = true;
                memcpy(rxbuf, _rxbuf2, rx_size);
            }
          }

          radio2.writeAckPayload( 1, &txp, sizeof(txpanel) );

        }
    }


    void loop(void)
    {
        Radio2();
       if (rxAvailable())
       {
         // printf("Rx <%s>\n",rxbuf);
         Serial.println(rxbuf);
         char data[32] = {rxbuf[0]};
       
       }
       if (rxbuf[0] == "w"[0] )
       {
        Serial.println("go w");
        digitalWrite(2,1);
        digitalWrite(3,0);
        digitalWrite(4,1);
        digitalWrite(5,0);
       }
       else if (rxbuf[0] == 's')
       {
        Serial.println("go s");
        digitalWrite(2,0);
        digitalWrite(3,1);
        digitalWrite(4,0);
        digitalWrite(5,1);
       }
       else if (rxbuf[0] == 'a')
       {
        Serial.println("go a");
        digitalWrite(2,0);
        digitalWrite(3,1);
        digitalWrite(4,1);
        digitalWrite(5,0);
       }
       else if (rxbuf[0] == 'd')
       {
        Serial.println("go d");
        digitalWrite(2,1);
        digitalWrite(3,0);
        digitalWrite(4,0);
        digitalWrite(5,1);
       }
       else if (rxbuf[0] == 'f')
       {
        Serial.println("all stop");
        digitalWrite(2,0);
        digitalWrite(3,0);
        digitalWrite(4,0);
        digitalWrite(5,0);
       }
    }

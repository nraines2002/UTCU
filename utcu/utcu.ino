// arduino .ino file for implementing UTCU

// library includes
#include <Wire.h> // wire for serial

#define dt 1000; // throttled delay

// *** "pr/message" branch ***
// any necessary defines for message branch done here



// *** "pr/timing-send" branch ***
// any necessary defines for timing-send branch done here

void setup() 
{
    // *** "pr/message" branch ***
    // any necessary setup for message branch done here



    // *** "pr/timing-send" branch ***
    // any necessary setup for timing-send branch done here

    // set the baud rate -- 115200 
    Serial.begin(115200);

}

void loop() 
{
    // *** "pr/message" branch ***
    // message that is sent over serial, with a delay defined by dt between
    // each index that is sent
    // [0] - enable pin
    // [1] - dpsk
    // [2] - to_fro
    // [3] - sb_start
    // [4] - data1
    // [5] - data2
    // [6] - data3
    // [7] - ant_sel

    // code to define each message bit goes here
    // each signal can be HIGH - 1, LOW - 0, DON'T CARE - -1
    int enable_pin = 0;
    int dpsk = -1;
    int to_fro = -1;
    int sb_start = -1;
    int data1 = -1;
    int data2 = -1;
    int data3 = -1;
    int ant_sel = -1;


    // *** "pr/timing-send" branch ***
    // code to handle timing and message sends below

    int message[] = {   
                        enable_pin,
                        dpsk, 
                        to_fro, 
                        sb_start, 
                        data1, 
                        data2, 
                        data3, 
                        ant_sel
                    };

    // send the message over serial one index at time with delay between each send
    for(int i = 0; i < 7; i++) {
        Serial.print(message[i]);
        delay(dt);
    }

    delay(dt);
}
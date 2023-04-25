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
    // [4] - antsel1
    // [5] - antsel2
    // [6] - antsel3
    // [7] - antsel_rd

    // code to define each message bit goes here
    // each signal can be HIGH - 1, LOW - 0, DON'T CARE - -1
    int enable_pin = 0;
    int dpsk = -1; 
    int to_fro = -1; // bit to determine
    int sb_start = -1;
    int antsel1 = -1; // antenna select has 3 bits to determine the antenna selected
    int antsel2 = -1;
    int antsel3 = -1;
    int antsel_rd = -1; // antenna select read to read the antenna bits once the antenna is to be selected

    // for antenna select:
    // 0 - indent/data - all data transmissions, DPSK, preambles for MLS
    // 1 - left OCI
    // 2 - rear/upper OCI
    // 3 - Right OCI
    // 4 - scanning beam - AZ, BAZ, EL
    // 5 - Off - default
    // 6 and 7 are unused for this project

    enum scanlimit 
    {
        AZ = [-62, 62],
        EL = [-1.5, 29.5],
        BAZ = [-42, 42],
    };


    // *** "pr/timing-send" branch ***
    // code to handle timing and message sends below

    // sending message by lsb to msb

    // What we are sending:
    // enable pin and antsel_rd are in parallel
    // rest of the data including antsel1-3 are sent serially over one wire


    int message[] = {   
                        enable_pin,
                        dpsk, 
                        to_fro, 
                        sb_start, 
                        antsel1, 
                        antsel2, 
                        antsel3, 
                        antsel_rd
                    };

    // send enable and antenna select in parallel
    // send the message over serial one index at time with delay between each send
    for(int i = 0; i < 7; i++) {
        Serial.print(message[i]);
        delay(dt);
    }

    delay(dt);
}

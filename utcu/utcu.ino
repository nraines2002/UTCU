// arduino .ino file for implementing UTCU

// library includes
#include <Wire.h> // wire for serial

#define dt 1; // throttled delay
#define ds 20*dt; // scaled delay
#define AZ "A";
#define EL "E";
#define BDW1 "B";
#define BDW2 "C";
#define BDW3 "D";
#define BDW4 "F";
#define BDW5 "G";
#define BDW6 "H";
#define ADWA1 "I";

// *** "pr/message" branch ***
// any necessary defines for message branch done here



// *** "pr/timing-send" branch ***
// any necessary defines for timing-send branch done here

int shift(int dpsk, int data)
{
    return dpsk ^ data;
}

void setup() 
{
    // HARD CODED FUNCTION
    char function = AZ;
    
    // parallel signals
    bool message_sent = false;
    int barker = [1, 1, 1, 0, 1];

    // function constants
    int az = [0, 0, 1, 1, 0, 0, 1];
    int el = [1, 1, 0, 0, 0, 0, 1];
    int bdw1 = [0, 1, 0, 1, 0, 0, 0];
    int bdw2 = [0, 1, 1, 1, 1, 0, 0];
    int bdw3 = [1, 0, 1, 0, 0, 0, 0];
    int bdw4 = [1, 0, 0, 0, 1, 0, 0];
    int bdw5 = [1, 1, 0, 1, 1, 0, 0];
    int bdw6 = [0, 0, 0, 1, 1, 0, 1];
    int adwa1 = [1, 1, 1, 0, 0, 1, 0];

    // signals
    int tx_en = 0;
    int dpsk = 0;
    int to_fro = -1;    // bit to determine
    int sb_start = 0;
    int antsel1 = 0;   // antenna select has 3 bits to determine the antenna selected
    int antsel2 = 0;
    int antsel3 = 0;
    int antsel_rd = 0; // antenna select read to read the antenna bits once the antenna is to be selected
    
    // TODO: ask laberge about this
    int basic_data_word [20];
    int aux_data_word [64];

    enum scanlimit 
    {
        AZ = [-62, 62],
        EL = [-1.5, 29.5],
        BAZ = [-42, 42],
    };

    // *** "pr/timing-send" branch ***
    // any necessary setup for timing-send branch done here

    // set the baud rate -- 115200 
    Serial.begin(115200);

}

void loop() 
{
    if (!message_sent)
    {
        // for loop that sends pre-data 0's
        for (int i = 0; i < 13;i++) 
        {
            Serial.print(0); // double check this data type
            delay(dt);
        }

        // for loop to shift and send barker code
        for (int i = 0;i < 5;i++)
        {
            dpsk = shift(dpsk, barker[i])
            Serial.print(dpsk);
            delay(dt);
        }

        // for loop to send function then data word
        if (function == AZ)
        {
            for (int i = 0; i < 7;i++)
            {
                dpsk = shift(dpsk, az[i]);
                Serial.print(dpsk);
                delay(dt);
            }
            for (int i = 0;i < 20;i++)
            {
                dpsk = shift(dpsk, basic_data_word[i]);
                Serial.print(dpsk);
                delay(dt);
            }
        } 
        else if (function == EL)
        {
            for (int i = 0; i < 7;i++)
            {
                dpsk = shift(dpsk, el[i]);
                Serial.print(dpsk);
                delay(dt);
            }
            for (int i = 0;i < 20;i++)
            {
                dpsk = shift(dpsk, basic_data_word[i]);
                Serial.print(dpsk);
                delay(dt);
            }
        } 
        else if (function == BDW1)
        {
            for (int i = 0; i < 7;i++)
            {
                dpsk = shift(dpsk, bdw1[i]);
                Serial.print(dpsk);
                delay(dt);
            }
            for (int i = 0;i < 20;i++)
            {
                dpsk = shift(dpsk, basic_data_word[i]);
                Serial.print(dpsk);
                delay(dt);
            }
        }
        else if (function == BDW2)
        {
            for (int i = 0; i < 7;i++)
            {
                dpsk = shift(dpsk, bdw2[i]);
                Serial.print(dpsk);
                delay(dt);
            }
            for (int i = 0;i < 20;i++)
            {
                dpsk = shift(dpsk, basic_data_word[i]);
                Serial.print(dpsk);
                delay(dt);
            }
        }        
        else if (function == BDW3)
        {
            for (int i = 0; i < 7;i++)
            {
                dpsk = shift(dpsk, bdw3[i]);
                Serial.print(dpsk);
                delay(dt);
            }
            for (int i = 0;i < 20;i++)
            {
                dpsk = shift(dpsk, basic_data_word[i]);
                Serial.print(dpsk);
                delay(dt);
            }
        }        
        else if (function == BDW4)
        {
            for (int i = 0; i < 7;i++)
            {
                dpsk = shift(dpsk, bdw4[i]);
                Serial.print(dpsk);
                delay(dt);
            }
            for (int i = 0;i < 20;i++)
            {
                dpsk = shift(dpsk, basic_data_word[i]);
                Serial.print(dpsk);
                delay(dt);
            }
        }        
        else if (function == BDW5)
        {
            for (int i = 0; i < 7;i++)
            {
                dpsk = shift(dpsk, bdw5[i]);
                Serial.print(dpsk);
                delay(dt);
            }
            for (int i = 0;i < 20;i++)
            {
                dpsk = shift(dpsk, basic_data_word[i]);
                Serial.print(dpsk);
                delay(dt);
            }
        }        
        else if (function == BDW6)
        {
            for (int i = 0; i < 7;i++)
            {
                dpsk = shift(dpsk, bdw6[i]);
                Serial.print(dpsk);
                delay(dt);
            }
            for (int i = 0;i < 20;i++)
            {
                dpsk = shift(dpsk, basic_data_word[i]);
                Serial.print(dpsk);
                delay(dt);
            }
        }
        else if (function == ADWA1)
        {
            for (int i = 0; i < 7;i++)
            {
                dpsk = shift(dpsk, adwa1[i]);
                Serial.print(dpsk);
                delay(dt);
            }
            for (int i = 0;i < 64;i++)
            {
                dpsk = shift(dpsk, aux_data_word[i]);
                Serial.print(dpsk);
                delay(dt);
            }
        }

        // for antenna select:
        // 0 - indent/data - all data transmissions, DPSK, preambles for MLS
        // 1 - left OCI
        // 2 - rear/upper OCI
        // 3 - Right OCI
        // 4 - scanning beam - AZ, BAZ, EL
        // 5 - Off - default
        // 6 and 7 are unused for this project
    }

    message_sent = true;
    delay(dt);
}

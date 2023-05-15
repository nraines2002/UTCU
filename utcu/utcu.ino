// arduino .ino file for implementing UTCU

// library includes
#include <Wire.h> // wire for serial

#define dt .064; // throttled delay (64ns)
#define ds 20*dt; // scaled delay
#define AZ "A";
#define BAZ "J";
#define EL "E";
#define BDW1 "B";
#define BDW2 "C";
#define BDW3 "D";
#define BDW4 "F";
#define BDW5 "G";
#define BDW6 "H";
#define ADWA1 "I";

int shift(int dpsk, int data)
{
    return dpsk ^ data;
}

// signals (pins)
int tx_en = 15;
int to_fro = 16;    // bit to determine
int sb_start = 17;
int antsel1 = 0;    // antenna select has 3 bits to determine the antenna selected
int antsel2 = 0;
int antsel3 = 0;
int antsel_rd = 18; // antenna select read to read the antenna bits once the antenna is to be selected
int dpsk = 0;       // dpsk bit to send serially

void setup() 
{
    // HARD CODED FUNCTION
    char function = AZ;
    
    // parallel signals
    bool message_sent = false;
    int barker = [1, 1, 1, 0, 1];

    // function constants
    int az = [0, 0, 1, 1, 0, 0, 1];
    int baz = [1, 0, 0, 1, 0, 0, 1];
    int el = [1, 1, 0, 0, 0, 0, 1];
    int bdw1 = [0, 1, 0, 1, 0, 0, 0];
    int bdw2 = [0, 1, 1, 1, 1, 0, 0];
    int bdw3 = [1, 0, 1, 0, 0, 0, 0];
    int bdw4 = [1, 0, 0, 0, 1, 0, 0];
    int bdw5 = [1, 1, 0, 1, 1, 0, 0];
    int bdw6 = [0, 0, 0, 1, 1, 0, 1];
    int adwa1 = [1, 1, 1, 0, 0, 1, 0];

    // signals (pins)
    pinMode(tx_en, OUTPUT);
    pinMode(to_fro, OUTPUT);
    pinMode(sb_start, OUTPUT);
    pinMode(antsel_rd, OUTPUT);

    enum scanlimit 
    {
        AZ = [-62, 62], 
        EL = [-1.5, 29.5],
        BAZ = [-42, 42],
    };

    // set the baud rate -- 115200 
    Serial.begin(115200);

}

// function to transmit az
void transmit_az(char function)
{
    if (function == AZ)
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
            dpsk = shift(dpsk, barker[i]);
            Serial.print(dpsk);
            delay(dt);
        }
        // sends function for AZ
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, az[i]);
            Serial.print(dpsk);
            delay(dt);
        }
        // after az send, antenna select and start scanning beam
        // LOCI
        antsel1 = 1; // TODO: check with supergroup to see if we need to send
        antsel2 = 0;
        antsel3 = 0;
        delay(dt); // delay for clock strobe
        // ROCI
        antsel1 = 0;
        antsel2 = 1;
        antsel3 = 0;
        delay(dt);
        // BOCI
        antsel1 = 1;
        antsel2 = 1;
        antsel3 = 0;
        delay(dt);
        // off
        antsel1 = 0;
        antsel2 = 0;
        antsel3 = 0;
        delay(dt);
        // whatever this is
        antsel1 = 1;
        antsel2 = 0;
        antsel3 = 1;
        // start scanning beam
        analogWrite(sb_start, HIGH);
        analogWrite(to_fro, HIGH);
        delay(); // TODO: delay for to scan
        analogWrite(tx_en, LOW);
        analogWrite(sb_start, LOW);
        delay(.01); // 10ns pause
        analogWrite(sb_start, HIGH);
        analogWrite(tx_en, HIGH);
        analogWrite(to_fro, LOW);
        delay(); // TODO: delay for the fro scan
        antsel1 = 0;
        antsel2 = 0;
        antsel3 = 0;
        analogWrite(tx_en, LOW);
    }
    else
    {
        // just transmit 0s for AZ send
        for (int i = 0; i < 74;i++) 
        {
            Serial.print(0);
            delay(dt);
        }
    }
}

// function to transmit baz
void transmit_baz(char function)
{
    if (function == BAZ)
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
            dpsk = shift(dpsk, barker[i]);
            Serial.print(dpsk);
            delay(dt);
        }
        // sends function for BAZ
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, baz[i]);
            Serial.print(dpsk);
            delay(dt);
        }
        // TODO: "to be provided" - doesn't provide (what data word are we transmitting?)
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, bdw1[i]);
            Serial.print(dpsk);
            delay(dt);
        }
        antsel1 = 1;
        antsel2 = 0;
        antsel3 = 0;
        delay(dt); // delay for clock strobe
        // ROCI
        antsel1 = 0;
        antsel2 = 1;
        antsel3 = 0;
        delay(dt);
        // BOCI
        antsel1 = 1;
        antsel2 = 1;
        antsel3 = 0;
        delay(dt);
        // off
        antsel1 = 0;
        antsel2 = 0;
        antsel3 = 0;
        delay(dt);
        // whatever this is
        antsel1 = 1;
        antsel2 = 0;
        antsel3 = 1;
        // start scanning beam
        analogWrite(sb_start, HIGH);
        analogWrite(to_fro, HIGH);
        delay(); // TODO: delay for to scan
        analogWrite(tx_en, LOW);
        analogWrite(sb_start, LOW);
        delay(.01); // 10ns pause
        analogWrite(sb_start, HIGH);
        analogWrite(tx_en, HIGH);
        analogWrite(to_fro, LOW);
        delay(); // TODO: delay for the fro scan
        antsel1 = 0;
        antsel2 = 0;
        antsel3 = 0;
        analogWrite(tx_en, LOW);
    }
    else
    {
        // just transmit 0s for BAZ send
        for (int i = 0; i < 32;i++) 
        {
            Serial.print(0);
            delay(dt);
        }
    }
}

// function to transmit el
void transmit_el(char function)
{
    if (function == EL)
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
            dpsk = shift(dpsk, barker[i]);
            Serial.print(dpsk);
            delay(dt);
        }
        // sends function for EL
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, el[i]);
            Serial.print(dpsk);
            delay(dt);
        }
        antsel1 = 0;
        antsel2 = 0;
        antsel3 = 0;
        delay(dt); // delay for clock strobe
        // ROCI
        antsel1 = 0;
        antsel2 = 1;
        antsel3 = 0;
        delay(dt);
        // BOCI
        antsel1 = 1;
        antsel2 = 1;
        antsel3 = 0;
        delay(dt);
        // off
        antsel1 = 0;
        antsel2 = 0;
        antsel3 = 0;
        delay(dt);
        // whatever this is
        antsel1 = 1;
        antsel2 = 0;
        antsel3 = 1;
        // start scanning beam
        analogWrite(sb_start, HIGH);
        analogWrite(to_fro, HIGH);
        delay(); // TODO: delay for to scan
        analogWrite(tx_en, LOW);
        analogWrite(sb_start, LOW);
        delay(.01); // 10ns pause
        analogWrite(sb_start, HIGH);
        analogWrite(tx_en, HIGH);
        analogWrite(to_fro, LOW);
        delay(); // TODO: delay for the fro scan
        antsel1 = 0;
        antsel2 = 0;
        antsel3 = 0;
        analogWrite(tx_en, LOW);
    }
    else
    {
        // just transmit 0s for EL send
        for (int i = 0; i < 25;i++) 
        {
            Serial.print(0);
            delay(dt);
        }
    }
}

// SEQ1 send
void SEQ1(char function)
{
    tx_en = 1;
    transmit_el(funtion);
    
    // send bdw1
    for (int i = 0; i < 7;i++)
    {
        dpsk = shift(dpsk, bdw1[i]);
        Serial.print(dpsk);
        delay(dt);
    }

    transmit_az(function);
    
    for (int i = 0; i < 7;i++)
    {
        dpsk = shift(dpsk, bdw2[i]);
        Serial.print(dpsk);
        delay(dt);
    }

    transmit_el(function);

    transmit_baz(function);

    // note 2 (ignoring for now)

    transmit_el(function);

    for (int i = 0; i < 7;i++)
    {
        dpsk = shift(dpsk, bdw3[i]);
        Serial.print(dpsk);
        delay(dt);
    }
}

// SEQ2 send
void SEQ2(char function)
{
    tx_en = 1;
    transmit_el(funtion);
    
    for (int i = 0; i < 7;i++)
    {
        dpsk = shift(dpsk, bdw4[i]);
        Serial.print(dpsk);
        delay(dt);
    }

    transmit_az(function);
    
    for (int i = 0; i < 7;i++)
    {
        dpsk = shift(dpsk, bdw5[i]);
        Serial.print(dpsk);
        delay(dt);
    }

    transmit_el(function);

    for (int i = 0; i < 7;i++)
    {
        dpsk = shift(dpsk, adw1[i]);
        Serial.print(dpsk);
        delay(dt);
    }

    transmit_el(function);

    for (int i = 0; i < 7;i++)
    {
        dpsk = shift(dpsk, bdw6[i]);
        Serial.print(dpsk);
        delay(dt);
    }
}

void loop() 
{
    if (!message_sent)
    {
        // full cycle
        SEQ1(function);
        delay(1); // double check that this is ms
        SEQ2(function);
        delay(13);
        SEQ1(function);
        delay(19);
        SEQ2(function);
        delay(2);
        SEQ1(function);
        delay(20);
        SEQ2(function);
        delay(6);
        SEQ1(function);
        SEQ2(function);
        delay(18);
    }
    message_sent = true;
    delay(dt);
}

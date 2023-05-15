// arduino .ino file for implementing UTCU

// library includes
#include "Wire.h" // wire for serial

#define AZ 'A';
#define BAZ 'J';
#define EL 'E';
#define BDW1 'B';
#define BDW2 'C';
#define BDW3 'D';
#define BDW4 'F';
#define BDW5 'G';
#define BDW6 'H';
#define ADWA1 'I';

int shift(int dpsk, int data)
{
    return dpsk ^ data;
}

// 50 ms/deg
enum scanlimit 
{
    AZ_limit = 124, 
    EL_limit = 31,
    BAZ_limit = 84,
};

int az_delay = (scanlimit::AZ_limit*50)/1000;
int baz_delay = (scanlimit::BAZ_limit*50)/1000;
int el_delay = (scanlimit::EL_limit*50)/1000;

// signals (pins)
int tx_en = 13;
int to_fro = 12;    // bit to determine
int sb_start = 11;
int antsel1 = 10;    // antenna select has 3 bits to determine the antenna selected
int antsel2 = 9;
int antsel3 = 7;
int antsel_rd = 5; // antenna select read to read the antenna bits once the antenna is to be selected
int dpsk = 0;       // dpsk bit to send serially
int dt = .064; // throttled delay (64ns)
int ds = 20*dt; // scaled delay

// HARD CODED FUNCTION
char function = AZ;

// parallel signals
bool message_sent = false;
int barker [5] = {1, 1, 1, 0, 1};

// function constants
int az [7] = {0, 0, 1, 1, 0, 0, 1};
int baz [7] = {1, 0, 0, 1, 0, 0, 1};
int el [7] = {1, 1, 0, 0, 0, 0, 1};
int bdw1[7] = {0, 1, 0, 1, 0, 0, 0};
int bdw2 [7] = {0, 1, 1, 1, 1, 0, 0};
int bdw3 [7] = {1, 0, 1, 0, 0, 0, 0};
int bdw4 [7] = {1, 0, 0, 0, 1, 0, 0};
int bdw5 [7] = {1, 1, 0, 1, 1, 0, 0};
int bdw6 [7] = {0, 0, 0, 1, 1, 0, 1};
int adwa1 [7] = {1, 1, 1, 0, 0, 1, 0};

void setup() 
{
    // signals (pins)
    pinMode(tx_en, OUTPUT);
    pinMode(to_fro, OUTPUT);
    pinMode(sb_start, OUTPUT);
    pinMode(antsel1, OUTPUT);
    pinMode(antsel2, OUTPUT);
    pinMode(antsel3, OUTPUT);
    pinMode(antsel_rd, OUTPUT);

    // set the baud rate -- 115200 
    Serial.begin(115200);
}

// pause time in annex 10
// function to transmit az
void transmit_az(char function)
{
    if (function == 'A')
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
        delay(az_delay);
        // after az send, antenna select and start scanning beam
        // LOCI
        digitalWrite(antsel1, HIGH);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, LOW);
        delay(dt); // delay for clock strobe
        // ROCI
        digitalWrite(antsel1, LOW);
        digitalWrite(antsel2, HIGH);
        digitalWrite(antsel3, LOW);
        delay(dt);
        // BOCI
        digitalWrite(antsel1, HIGH);
        digitalWrite(antsel2, HIGH);
        digitalWrite(antsel3, LOW);
        delay(dt);
        // off
        digitalWrite(antsel1, LOW);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, LOW);
        delay(dt);
        // whatever this is
        digitalWrite(antsel1, HIGH);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, HIGH);

        // start scanning beam
        digitalWrite(sb_start, HIGH);
        digitalWrite(to_fro, HIGH);
        delay(az_delay); 
        digitalWrite(tx_en, LOW);
        digitalWrite(sb_start, LOW);
        delay(.6); // 600 us pause
        digitalWrite(sb_start, HIGH);
        digitalWrite(tx_en, HIGH);
        digitalWrite(to_fro, LOW);
        delay(az_delay); 
        digitalWrite(antsel1, LOW);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, LOW);
        digitalWrite(tx_en, LOW);
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
    if (function == 'J')
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

        // ignoring this basic data word send
        /*for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, bdw1[i]);
            Serial.print(dpsk);
            delay(dt);
        }*/

        // after az send, antenna select and start scanning beam
        // LOCI
        digitalWrite(antsel1, HIGH);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, LOW);
        delay(dt); // delay for clock strobe
        // ROCI
        digitalWrite(antsel1, LOW);
        digitalWrite(antsel2, HIGH);
        digitalWrite(antsel3, LOW);
        delay(dt);
        // BOCI
        digitalWrite(antsel1, HIGH);
        digitalWrite(antsel2, HIGH);
        digitalWrite(antsel3, LOW);
        delay(dt);
        // off
        digitalWrite(antsel1, LOW);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, LOW);
        delay(dt);
        // whatever this is
        digitalWrite(antsel1, HIGH);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, HIGH);
        // start scanning beam
        digitalWrite(sb_start, HIGH);
        digitalWrite(to_fro, HIGH);
        delay(baz_delay); 
        digitalWrite(tx_en, LOW);
        digitalWrite(sb_start, LOW);
        delay(.6); // 600us pause
        digitalWrite(sb_start, HIGH);
        digitalWrite(tx_en, HIGH);
        digitalWrite(to_fro, LOW);
        delay(baz_delay); 
        digitalWrite(antsel1, LOW);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, LOW);
        digitalWrite(tx_en, LOW);
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
    if (function == 'E')
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
        // LOCI - off for EL
        digitalWrite(antsel1, LOW);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, LOW);
        delay(dt); // delay for clock strobe
        // ROCI
        digitalWrite(antsel1, LOW);
        digitalWrite(antsel2, HIGH);
        digitalWrite(antsel3, LOW);
        delay(dt);
        // BOCI
        digitalWrite(antsel1, HIGH);
        digitalWrite(antsel2, HIGH);
        digitalWrite(antsel3, LOW);
        delay(dt);
        // off
        digitalWrite(antsel1, LOW);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, LOW);
        delay(dt);
        // whatever this is
        digitalWrite(antsel1, HIGH);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, HIGH);
        // start scanning beam
        digitalWrite(sb_start, HIGH);
        digitalWrite(to_fro, HIGH);
        delay(el_delay); 
        digitalWrite(tx_en, LOW);
        digitalWrite(sb_start, LOW);
        delay(.4); // 400 us pause
        digitalWrite(sb_start, HIGH);
        digitalWrite(tx_en, HIGH);
        digitalWrite(to_fro, LOW);
        delay(el_delay); 
        digitalWrite(antsel1, LOW);
        digitalWrite(antsel2, LOW);
        digitalWrite(antsel3, LOW);
        digitalWrite(tx_en, LOW);
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
    transmit_el(function);
    
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
    transmit_el(function);
    
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
        dpsk = shift(dpsk, adwa1[i]);
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
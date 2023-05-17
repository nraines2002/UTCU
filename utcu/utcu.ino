// arduino .ino file for implementing UTCU

// library includes
#include "Wire.h"

#define AZ 'A';
#define BAZ 'B';
#define EL 'E';

// 50 us/deg
enum scanlimit 
{
    AZ_limit = 124, 
    EL_limit = 31,
    BAZ_limit = 84,
};

// signals (pins)
int tx_en = 13;
int to_fro = 12;    // bit to determine
int sb_start = 11;
// antsel3 is MSB
int antsel1 = 10;    // antenna select has 3 bits to determine the antenna selected
int antsel2 = 9;
int antsel3 = 7;
int antsel_rd = 5; // antenna select read to read the antenna bits once the antenna is to be selected
int dpsk = 0;       // dpsk bit to send serially
int dpsk_pin = 2;
int scale = 18;
float dt = .064; // throttled delay (64us)
float ds = scale*dt; // scaled delay
int az_delay = ((scanlimit::AZ_limit*50)/1000)*scale;
int baz_delay = ((scanlimit::BAZ_limit*50)/1000)*scale;
int el_delay = ((scanlimit::EL_limit*50)/1000)*scale;
unsigned long t_o = 0;
unsigned long t_d = 0;

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
    pinMode(dpsk_pin, OUTPUT);
    Serial.begin(115200);
}

void loci()
{
    // left oci - 001 (1)
    digitalWrite(antsel1, HIGH);
    digitalWrite(antsel2, LOW);
    digitalWrite(antsel3, LOW);
}

void boci()
{
    // read(back) oci - 010 (2)
    digitalWrite(antsel1, LOW);
    digitalWrite(antsel2, HIGH);
    digitalWrite(antsel3, LOW);
}

void roci()
{
    // right oci - 011 (3)
    digitalWrite(antsel1, HIGH);
    digitalWrite(antsel2, HIGH);
    digitalWrite(antsel3, LOW);
}

void sboci()
{
    // sb oci - 100 (4)
    digitalWrite(antsel1, LOW);
    digitalWrite(antsel2, LOW);
    digitalWrite(antsel3, HIGH);
}

void offoci()
{
    // turns oci off - 101 (5)
    digitalWrite(antsel1, HIGH);
    digitalWrite(antsel2, LOW);
    digitalWrite(antsel3, HIGH);
}

int shift(int dpsk, int data)
{
    int new_dpsk = dpsk ^ data;
    Serial.print(new_dpsk);
    if (new_dpsk == 1)
    {
        digitalWrite(dpsk_pin, HIGH);
    } else
    {
        digitalWrite(dpsk_pin, LOW);
    }
    return new_dpsk; // return dpsk bit for logic
}

// pause time in annex 10
// function to transmit az
void transmit_az(char function)
{
    Serial.print("\nAz began\n");
    digitalWrite(tx_en, HIGH);
    t_o = millis();
    //delay(ds);
    //Serial.print("Tx enabled");
    if (function == 'A')
    {
        Serial.print("printing 0's\n");
        // for loop that sends pre-data 0's
        for (int i = 0; i < 13;i++) 
        {
            dpsk = shift(0, 0); 
            delay(ds);
            t_d = millis() - t_o; 
            Serial.print("\nbit sent:%d", t_d);
        }
        t_d = millis() - t_o;
        Serial.print("\npre-data 0's done:%d", t_d);
        Serial.print("printing barker\n");
        // for loop to shift and send barker code
        for (int i = 0;i < 5;i++)
        {
            dpsk = shift(dpsk, barker[i]);
            delay(ds);
            t_d = millis() - t_o;
            Serial.print("\nbit sent:%d", t_d);
        }
        Serial.print("\nbarker done:%d", t_d);
        Serial.print("sending function\n");
        // sends function for AZ
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, az[i]);
            delay(ds);
            t_d = millis() - t_o;
            Serial.print("\nbit sent:%d", t_d);
        }
        Serial.print("delaying\n");
        delay(az_delay);

        // after az send, antenna select and start scanning beam
        Serial.print("loci\n");
        loci();
        delay(ds); // delay for clock strobe
        Serial.print("boci\n");
        boci();
        delay(ds);
        Serial.print("roci\n");
        roci();
        delay(ds);
        Serial.print("offoci\n");
        // off
        offoci();
        delay(ds);

        // start scanning beam
        Serial.print("setting sb high");
        digitalWrite(sb_start, HIGH);
        digitalWrite(to_fro, HIGH);
        delay(az_delay); 
        digitalWrite(tx_en, LOW);
        digitalWrite(sb_start, LOW);
        Serial.print("setting sb low");
        delay(.6*scale); // 600 us pause
        digitalWrite(sb_start, HIGH);
        Serial.print("setting sb high");
        digitalWrite(tx_en, HIGH);
        Serial.print("Tx enabled");
        digitalWrite(to_fro, LOW);
        delay(az_delay); 
        digitalWrite(sb_start, LOW);
        Serial.print("setting sb low");
        offoci();
        digitalWrite(tx_en, LOW);
    }
    else
    {
        // just transmit 0s for AZ send
        for (int i = 0; i < 74;i++) 
        {
            dpsk = shift(0, 0);
            delay(ds);
        }
    }
    digitalWrite(tx_en, LOW);
    //delay(ds);
}

// function to transmit baz
void transmit_baz(char function)
{
    digitalWrite(tx_en, HIGH);
    //delay(ds);
    Serial.print("Tx enabled");
    if (function == 'B')
    {
        // for loop that sends pre-data 0's
        for (int i = 0; i < 13;i++) 
        {
            dpsk = shift(0, 0); // double check this data type
            delay(ds);
        }
        // for loop to shift and send barker code
        for (int i = 0;i < 5;i++)
        {
            dpsk = shift(dpsk, barker[i]);
            delay(ds);
        }
        // sends function for BAZ
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, baz[i]);
            delay(ds);
        }
        delay(baz_delay);

        // ignoring this basic data word send
        /*for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, bdw1[i]);
            tx.print(dpsk);
            delay(ds);
        }*/

        // after az send, antenna select and start scanning beam
        // LOCI
        loci();
        delay(ds); // delay for clock strobe

        roci();
        delay(ds);
        
        boci();
        delay(ds);
        
        // off
        offoci();
        delay(ds);
        // start scanning beam
        digitalWrite(sb_start, HIGH);
        digitalWrite(to_fro, HIGH);
        delay(baz_delay); 
        digitalWrite(tx_en, LOW);
        digitalWrite(sb_start, LOW);
        delay(.6*scale); // 600us pause
        digitalWrite(sb_start, HIGH);
        digitalWrite(tx_en, HIGH);
        Serial.print("Tx enabled");
        digitalWrite(to_fro, LOW);
        delay(baz_delay); 
        digitalWrite(sb_start, LOW);
        offoci();
        digitalWrite(tx_en, LOW);
    }
    else
    {
        // just transmit 0s for BAZ send
        for (int i = 0; i < 32;i++) 
        {
            dpsk = shift(0, 0);
            delay(ds);
        }
    }
    digitalWrite(tx_en, LOW);
    //delay(ds);
}

// function to transmit el
void transmit_el(char function)
{
    digitalWrite(tx_en, HIGH);
    //delay(ds);
    Serial.print("Tx enabled");
    if (function == 'E')
    {
        // for loop that sends pre-data 0's
        for (int i = 0; i < 13;i++) 
        {
            dpsk = shift(0, 0); // double check this data type
            delay(ds);
        }
        // for loop to shift and send barker code
        for (int i = 0;i < 5;i++)
        {
            dpsk = shift(dpsk, barker[i]);
            delay(ds);
        }
        // sends function for EL
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, el[i]);
            delay(ds);
        }
        delay(el_delay);

        // rear oci
        boci();
        delay(ds);
        
        // off
        offoci();
        delay(ds);

        // start scanning beam
        Serial.print("setting sb high");
        digitalWrite(sb_start, HIGH);
        digitalWrite(to_fro, HIGH);
        delay(el_delay); 
        digitalWrite(tx_en, LOW);
        digitalWrite(sb_start, LOW);
        Serial.print("setting sb low");
        delay(.4*scale); // 600 us pause
        digitalWrite(sb_start, HIGH);
        Serial.print("setting sb high");
        digitalWrite(tx_en, HIGH);
        Serial.print("Tx enabled");
        digitalWrite(to_fro, LOW);
        delay(el_delay); 
        digitalWrite(sb_start, LOW);
        Serial.print("setting sb low");
        offoci();
        digitalWrite(tx_en, LOW);
    }
    else
    {
        // just transmit 0s for EL send
        for (int i = 0; i < 25;i++) 
        {
            dpsk = shift(0, 0);
            delay(ds);
        }
    }
    digitalWrite(tx_en, LOW);
    //delay(ds);
}

// SEQ1 send
void SEQ1(char function)
{
    Serial.print("\nSending el\n");
    transmit_el(function);
    
    Serial.print("\nSEQ1: sending bdw1\n");
    // send bdw1
    digitalWrite(tx_en, HIGH);
    if (function != "E")
    {
        //delay(ds);
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, bdw1[i]);
            delay(ds);
        }
    } else { // dont transmit bdw1 during el 
        //delay(ds);
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, 0);
            delay(ds);
        }
    }
    digitalWrite(tx_en, LOW);
    //delay(ds);

    Serial.print("\nSending azimuth\n");
    transmit_az(function);
    
    Serial.print("\nSending bdw2\n");
    digitalWrite(tx_en, HIGH);
    if (function != 'E')
    {    
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, bdw2[i]);
            delay(ds);
        }
    } else {
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, 0);
            delay(ds);
        }
    }
    digitalWrite(tx_en, LOW);
    //delay(ds);

    Serial.print("\nSending el\n");
    transmit_el(function);

    Serial.print("\nSending BAZ\n");
    transmit_baz(function);

    // note 2 (ignoring for now)
    Serial.print("\nSending EL\n");
    transmit_el(function);

    Serial.print("\nSending bdw3\n");
    digitalWrite(tx_en, HIGH);
    if (function != 'E')
    {
        //delay(ds);
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, bdw3[i]);
            delay(ds);
        }
    } else {
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, 0);
            delay(ds);
        }
    }
    digitalWrite(tx_en, LOW);
    //delay(ds);
}

// SEQ2 send
void SEQ2(char function)
{
    transmit_el(function);
    
    digitalWrite(tx_en, HIGH);
    if (function != 'E')
    {
    //delay(ds);
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, bdw4[i]);
            delay(ds);
        }
    } else {
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, 0);
            delay(ds);
        }
    }
    digitalWrite(tx_en, LOW);
    //delay(ds);

    transmit_az(function);
    
    digitalWrite(tx_en, HIGH);
    //delay(ds);
    if (function != 'E')
    {
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, bdw5[i]);
            delay(ds);
        }
    } else {
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, 0);
            delay(ds);
        }
    }
    digitalWrite(tx_en, LOW);
    //delay(ds);

    transmit_el(function);

    digitalWrite(tx_en, HIGH);
    //delay(ds);
    if (function == 'A')
    {
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, adwa1[i]);
            delay(ds);
        }
    } else if {
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, 0);
            delay(ds);
        }
    }
    digitalWrite(tx_en, LOW);
    //delay(ds);

    transmit_el(function);

    digitalWrite(tx_en, HIGH);
    //delay(ds);
    if (function != 'E')
    {
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, bdw6[i]);
            delay(ds);
        }
    } else if {
        for (int i = 0; i < 7;i++)
        {
            dpsk = shift(dpsk, 0);
            delay(ds);
        }
    }
    digitalWrite(tx_en, LOW);
    //delay(ds);
}

void loop() 
{
    if (!message_sent)
    {
        // full cycle
        SEQ1(function);
        delay(1*scale); // double check that this is ms
        SEQ2(function);
        delay(13*scale);
        SEQ1(function);
        delay(19*scale);
        SEQ2(function);
        delay(2*scale);
        SEQ1(function);
        delay(20*scale);
        SEQ2(function);
        delay(6*scale);
        SEQ1(function);
        SEQ2(function);
        delay(18*scale);
    }
    message_sent = false;
    Serial.print("\nFinished run!!!\n");
}
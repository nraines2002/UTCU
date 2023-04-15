// arduino .ino file for implementing UTCU

#define dt 1000; // 

void setup() 
{
    // set the baud rate -- 115200 
    Serial.begin(115200);

}

void loop() 
{
    // message that is sent over serial, with a delay defined by dt between
    // each index that is sent
    // [0] - 
    // [1] - enable pin
    // [2] - 
    // [3] - 
    // [4] - 
    // [5] - 
    // [6] - 
    // [7] - 

    int message[] = {1, 1, 1, -1, -1, 1, -1};

    // send the message over serial
    for(int i = 0; i < 7; i++) {
        Serial.print(message[i]);
        delay(dt);
    }
    sprintf(buf, "%03d:%03d:%03d:%03d:%03d:%03d:%03d:%03d", alt, hdg, ow, volt1, volt2, hdp, vdp, vers);

    Serial.print("$");
    Serial.print(takeoffButtonPressed);
    Serial.print(":");
    Serial.print(landButtonPressed);
    Serial.print(":");
    Serial.print(confirmButtonPressed);
    Serial.print(":");
    Serial.print(holdButtonPressed);
    Serial.print(":");
    Serial.println(buf);

    // Wait for a moment before sending the next code
    delay(dt);
}
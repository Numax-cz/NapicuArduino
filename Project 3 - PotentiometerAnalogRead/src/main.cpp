#include <Arduino.h>

const int ANALOG_PIN = A1;

void setup(){
    Serial.begin(9600);
}

void loop(){
    int value = analogRead(ANALOG_PIN);

    Serial.print("Value: ");
    Serial.println(value);
    
    delay(100);
}
//
// Created by fadhil on 8/31/25.
//

#include <Arduino.h>
#include <stepper/stepper.h>

int stepPin[4] = {D0, D1, D2, D8};
int dirPin[4] = {D3, D4, D5, D8};
unsigned long lastTime = 0;
unsigned long lastTimeStop = 0;
float speed = 0;
ShakerStepper stp(stepPin, dirPin);
void setup() {
    Serial.begin(115200);
    stp.init();
    stp.setSpeed(200.0);
    stp.setAcceleration(20.0);
    stp.setState(StepperState::RUN);
    pinMode(D6, OUTPUT);
    digitalWrite(D6, LOW);
    Serial.println("HelloWorld");
}

void loop() {
    const unsigned long now = millis();
    stp.run();
    if (now -lastTime > 100) {
        speed = stp.getSpeed();
        Serial.println(speed);
        lastTime = now;
    }

    if ( now - lastTimeStop > 15000 ) {
        stp.setState(StepperState::STOP);
    }
}
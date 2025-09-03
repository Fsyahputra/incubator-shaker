//
// Created by fadhil on 8/31/25.
//

#include <Arduino.h>
#include <stepper/stepper.h>

int stepPin[4] = {18, 5, 17, 16};
int dirPin[4] = {10, 10, 10, 10};
unsigned long lastTime = 0;
float speed = 0;
ShakerStepper stp(stepPin, dirPin);
void setup() {
    Serial.begin(115200);
    stp.init();
    stp.setSpeed(500);
    stp.setAcceleration(10.0);
    stp.setState(StepperState::RUN);
}

void loop() {
    const unsigned long now = millis();
    stp.run();
    if (now -lastTime > 100) {
        speed = stp.getSpeed();
        Serial.println(speed);
        lastTime = now;
    }
    // if (speed > 500) {
    //     stp.setState(StepperState::STOP);
    // }

    // if (speed == 0) {
    //     stp.setState(StepperState::RUN);
    // }

}
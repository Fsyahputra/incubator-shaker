//
// Created by fadhil on 9/2/25.
//

#include <Arduino.h>
#include <countdown/countdown.h>

unsigned long lastTime = 0;
bool firstTime = true;

ShakerCountdown cd;

void secondTestSetup() {
    cd.setTime(0, 0, 10);
    cd.setState(CountDownState::COUNTING);
}


void minuteTestSetup() {
    cd.setTime(0, 1, 0);
    cd.setState(CountDownState::COUNTING);
}

void minuteTestLoop() {
    const unsigned long now = millis();
    char timeString[32];
    cd.getTimeString(timeString, sizeof(timeString));
    if (now - lastTime > 1000 || firstTime) {
        Serial.println(timeString);
        lastTime = now;
        firstTime = false;
    }

    if (cd.getState() == CountDownState::DONE) {
        Serial.println("Done");
        cd.resetVariables();
        minuteTestSetup();
        delay(2000);
    }
}

void hourTestSetup() {
    cd.setTime(2, 0, 0);
    cd.setState(CountDownState::COUNTING);
}

void hourTestLoop() {
    const unsigned long now = millis();
    char timeString[32];
    cd.getTimeString(timeString, sizeof(timeString));
    if (now - lastTime > 10 || firstTime) {
        Serial.println(timeString);
        lastTime = now;
        firstTime = false;
    }

    if (cd.getState() == CountDownState::DONE) {
        Serial.println("Done");
        cd.resetVariables();
        hourTestSetup();
        delay(2000);
    }
}


void secondTestLoop() {
    const unsigned long now = millis();
    char timeString[32];
    cd.getTimeString(timeString, sizeof(timeString));
    if (now - lastTime > 1000 || firstTime) {
        Serial.println(timeString);
        lastTime = now;
        firstTime = false;
    }

    if (cd.getState() == CountDownState::DONE) {
        Serial.println("Done");
        cd.resetVariables();
        cd.setTime(0,0, 10);
        cd.setState(CountDownState::COUNTING);
        delay(2000);
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Countdown Test");
    hourTestSetup();
}


void loop() {
    cd.run();
    hourTestLoop();
}
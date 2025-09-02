//
// Created by fadhil on 8/31/25.
//

#include <Arduino.h>
#include <rotary/rotary.h>
#define PIN_A 19
#define PIN_B 25
#define BUTTON_PIN 26

RotaryEncoder ro(PIN_A, PIN_B, BUTTON_PIN);
unsigned long lastTime = 0;
unsigned long interval = 1000;

void setup() {
    Serial.begin(115200);
    ro.initializePins();
}


void rotaryStateTest() {
    const RotaryState state = ro.getState();
    const unsigned long now = millis();
    if (state == RotaryState::LEFT) {
        Serial.println("Left");
    }
    else if (state == RotaryState::RIGHT) {
        Serial.println("Right");
    } else {
        if ( now - lastTime > interval) {
            Serial.println("Idle");
            lastTime = now;
        }
    }
}

void buttonStateTest() {
    const RotaryButtonState state = ro.getButtonState();
    const unsigned long now = millis();
    if (now - lastTime > interval) {
        if (state == RotaryButtonState::PRESSED) {
            Serial.println("Pressed");
        } else if (state == RotaryButtonState::RELEASED) {
            Serial.println("Released");
        }
        lastTime = now;
    }

}

void loop() {
    ro.run();
    buttonStateTest();
    rotaryStateTest();
}



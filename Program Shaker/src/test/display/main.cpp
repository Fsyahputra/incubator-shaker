//
// Created by fadhil on 8/31/25.
//

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <display/display.h>
#include <random>
#define LCD_SDA 21
#define LCD_SCL 22

char configTitles[4][20] = {
    "Configuring",
    "Set Rpm",
    "Set Temp",
    "Set Time",
};

char labels[4][20] = {
    "welcome",
    "RPM",
    "C",
    "Sec"
};

float values[4] = {
    10.0,
    20.0,
    30.0,
    40.0,
};

float rpm = 20.0;
float times = 20.0;
float temperature = 30.0;

unsigned long lastTimeChange = 0;
unsigned long interval = 500;
int state = 0;
int state2 = 0;
bool outerState = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);
ShakerDisplay sd(lcd, LCD_SDA, LCD_SCL);

void setup() {
    Serial.begin(115200);
    Serial.println("hello world");
    Wire.begin(LCD_SDA,LCD_SCL);
    sd.initDisplay();
    sd.setState(displayState::CONFIGURING);

}

void configuringTest() {
    const unsigned long now = millis();
    if (now - lastTimeChange > interval) {
        sd.setConfigTitle(configTitles[state]);
        sd.setConfigValue(labels[state], values[state]);
        lastTimeChange = now;
        state++;
    }
    if (state >= 4 ) {
        state = 0;
        outerState = true;
        sd.setState(displayState::READY);
    }
}

void readyTest() {
    const unsigned long now = millis();
    if (now - lastTimeChange > interval) {
        rpm = random(100, 200);
        times = random(100, 200);
        temperature = random(25, 50);
        lastTimeChange = now;
        state2++;
    }
    sd.setRpm(rpm);
    sd.setTime(times);
    sd.setTemperature(temperature);
    if (state2 >= 10) {
        state2 = 0;
        state = 0;
        outerState = false;
        sd.setState(displayState::CONFIGURING);
    }
}

void integrate() {
    if (!outerState) {
        configuringTest();
    }
    if (outerState) {
        readyTest();
    }
}



void loop() {
    sd.run();
    integrate();
}

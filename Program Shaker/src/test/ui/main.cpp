//
// Created by fadhil on 9/3/25.
//
#include <Arduino.h>
#include <rotary/rotary.h>
#include <display/display.h>
#include <LiquidCrystal_I2C.h>
#include <countdown/countdown.h>
#include <UI/ui.h>
#include <random>
#define PIN_A 32
#define PIN_B 33
#define BUTTON_PIN 25
#define LCD_SDA 21
#define LCD_SCL 22


unsigned long lastTimes = 0;
unsigned long interval = 500;
float rpm = 20.0;
float temperature = 30.0;

RotaryEncoder re(PIN_A, PIN_B, BUTTON_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
ShakerDisplay sd(lcd, LCD_SDA, LCD_SCL);
ShakerCountdown counter;
ShakerUI ui(re, sd, counter);


void setup() {
    Serial.begin(115200);
    Serial.println("Hello World!");
    ui.init();
}

void loop() {
    ui.run();
    ui.setCurrentTemperature(temperature);
    ui.setCurrentRpm(rpm);
    const unsigned long now = millis();
    if (now - lastTimes > interval) {
        rpm = random(0, 300);
        temperature = random(20, 40);
        lastTimes = now;
    }

}
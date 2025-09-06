//
// Created by fadhil on 9/6/25.
//

#include "tempControl/fan.h"
#include <Arduino.h>

void ShakerFan::init() const {
    pinMode(this->IN1Pin, OUTPUT);
    pinMode(this->IN2Pin, OUTPUT);
    pinMode(this->ENPin, OUTPUT);
    this->turnOff();
}


void ShakerFan::setTargetTemperature(float newTargetTemperature) {
    if (this->targetTemperature == newTargetTemperature) {
        return;
    }
    this->targetTemperature = newTargetTemperature;
}


void ShakerFan::run(float temperature, float targetTemperature) {
    const unsigned long now  = millis();
    this->setTargetTemperature(targetTemperature);
    if (now - this->lastTimeChange >= this->speedChangeInterval) {
        this->lastTimeChange = now;
        this->computeNewSpeed();
    }
}


void ShakerFan::setCcw() const {
    digitalWrite(this->IN1Pin, LOW);
    digitalWrite(this->IN2Pin, HIGH);
}

void ShakerFan::setCw() const {
    digitalWrite(this->IN1Pin, HIGH);
    digitalWrite(this->IN2Pin, LOW);
}

void ShakerFan::changeDirection(const FanDirection direction) {
    this->currentDirection = direction;
    switch (this->currentDirection) {
        case FanDirection::CW:
            this->setCw();
            break;
        case FanDirection::CCW:
            this->setCw();
            break;
    }
}

void ShakerFan::turnOn(const FanDirection direction) {
    this->changeDirection(direction);
    analogWrite(this->ENPin, this->currentPwmValue);
}

void ShakerFan::turnOff() const {
    analogWrite(this->ENPin, 0);
}


void ShakerFan::convertSpeedToPwm() {
    this->currentPwmValue = static_cast<int>( (this->currentSpeed / 100.0 ) * this->pwmMaxVal);
    this->currentPwmValue = constrain(this->currentPwmValue, 0, this->pwmMaxVal);
}


void ShakerFan::computeNewSpeed() {
    if (this->currentDirection == FanDirection::CCW) {
        this->currentSpeed = 100.0;
        return;
    }
    this->currentSpeed = this->gainConstant * abs(this->currentTemperature - this->targetTemperature);
    this->currentSpeed = constrain(this->currentSpeed, 0.0, 100.0);
    this->convertSpeedToPwm();
}


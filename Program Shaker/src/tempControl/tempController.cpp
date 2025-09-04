//
// Created by fadhil on 9/4/25.
//

#include <Arduino.h>
#include <tempControl/tempController.h>


ShakerTempController::ShakerTempController(tempIF &sensor, const int peltierPin, const int fanPin) : sensor(sensor),
    peltierPin(peltierPin), fanPin(fanPin), targetTemperature(0), currentTemperature(0) {
    this->peltierPin = peltierPin;
    this->fanPin = fanPin;
    this->sensor = sensor;
}

void ShakerTempController::init() {
    this->sensor.init();
    pinMode(this->peltierPin, OUTPUT);
    pinMode(this->fanPin, OUTPUT);
    this->turnOffPeltier();
    this->turnOffFan();
    this->setState(TempControllerState::OFF);
}

float ShakerTempController::getCurrentTemperature() const {
    return this->currentTemperature;
}

void ShakerTempController::readWithInterval() {
    if (const unsigned long now = millis(); now - this->lastRead > this->readInterval) {
        this->currentTemperature = this->sensor.readTemperatureCelsius();
        this->lastRead = now;
    }
}

void ShakerTempController::turnOnPeltier() const {
    digitalWrite(this->peltierPin, HIGH);
}

void ShakerTempController::turnOffPeltier() const {
    digitalWrite(this->peltierPin, LOW);
}

void ShakerTempController::turnOffFan() const {
    digitalWrite(this->fanPin, LOW);
}

void ShakerTempController::turnOnFan() const {
    digitalWrite(this->fanPin, HIGH);
}

void ShakerTempController::setTargetTemperature(const float targetTemp) {
    this->targetTemperature = targetTemp;
}




TempControllerState ShakerTempController::getState() const {
    return this->currentState;
}




void ShakerTempController::handleOnState() {
    const unsigned long now = millis();
    if (this->heating && (now - this->lastHeatingTime >= this->heatingInterval)) {
        heating = false;
        this->lastHeatingTime = now;
    }
    if (!this->heating && (now - this->lastHeatingTime >= this->coolingInterval)) {
        this->heating = true;
        this->lastHeatingTime = now;
    }
    if (this->heating) {
        if (this->currentTemperature > this->targetTemperature + this->hysterisis) {
            this->turnOffPeltier();
        }
        if (this->currentTemperature < this->targetTemperature - this->hysterisis) {
            this->turnOnPeltier();
        }
        this->turnOnFan();
        return;
    }
    this->handleOffState();
}

void ShakerTempController::handleOffState() const {
    turnOffFan();
    turnOffPeltier();
}



void ShakerTempController::setState(const TempControllerState newState) {
    this->currentState = newState;
}

void ShakerTempController::run() {
    this->readWithInterval();
    switch (this->getState()) {
        case TempControllerState::OFF:
            this->handleOffState();
            break;
        case TempControllerState::ON:
            this->handleOnState();
            break;
    }
}

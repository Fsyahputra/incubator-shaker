//
// Created by fadhil on 9/6/25.
//

#include <tempControl/peltier.h>


void ShakerPeltier::init() const {
    pinMode(this->peltierPin, OUTPUT);
    this->sensor.init();
    this->turnOff();
}

void ShakerPeltier::readTempWithInterval() {
    if (const unsigned long now = millis(); now - this->lastRead > this->readInterval) {
        this->lastRead = now;
        this->currentTemperature = this->sensor.readTemperatureCelsius();
    }
}

ShakerPeltier::ShakerPeltier(const int pin, tempIF &sensor) : peltierPin(pin), maxTemperature(70.0), sensor(sensor) {
    this->peltierPin = pin;
    this->sensor = sensor;
}


void ShakerPeltier::turnOn() const {
    if (this->currentTemperature > this->maxTemperature) {
        this->turnOff();
        return;
    }
    digitalWrite(this->peltierPin, HIGH);
}

void ShakerPeltier::turnOff() const {
    digitalWrite(this->peltierPin, LOW);
}



void ShakerPeltier::run() {
    this->readTempWithInterval();
}

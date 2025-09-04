//
// Created by fadhil on 9/4/25.
//

#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include "tempControl/tempSensorIF.h"

enum class TempControllerState {
    ON,
    OFF,
};


class ShakerTempController {


private:
    tempIF &sensor;
    int peltierPin;
    int fanPin;
    const float hysterisis = 5.0;

    const unsigned long readInterval = 5000;
    unsigned long lastRead = 0;

    bool heating = false;
    const unsigned long heatingInterval = 60000;
    const unsigned long coolingInterval = 30000;
    unsigned long lastHeatingTime = 0;


    TempControllerState currentState = TempControllerState::OFF;
    float targetTemperature;
    float currentTemperature;


    void readWithInterval();

    void turnOnPeltier() const;
    void turnOffPeltier() const;
    void turnOnFan() const;
    void turnOffFan() const;

    void handleOnState();
    void handleOffState() const;
    TempControllerState getState() const;
public:
    void init();
    void run();
    ShakerTempController(tempIF &sensor, const int peltierPin, const int fanPin);
    float getCurrentTemperature() const;
    void setTargetTemperature(float targetTemp);
    void setState(TempControllerState newState);
};

#endif //TEMPCONTROLLER_H
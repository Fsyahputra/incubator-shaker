//
// Created by fadhil on 9/6/25.
//

#include "tempControl/tempSensorIF.h"

void Mcp9808Impl::init() {
    this->sensor.begin(this->i2cAddress);
    this->sensor.init();
    this->sensor.setResolution(this->resolution);
}

Mcp9808Impl::Mcp9808Impl(const Adafruit_MCP9808 &sensor, const uint8_t i2cAddress) : sensor(sensor), i2cAddress(i2cAddress) {
    this->sensor = sensor;
    this->i2cAddress = i2cAddress;
}


float Mcp9808Impl::readTemperatureCelsius() {
    return this->sensor.readTempC();
}


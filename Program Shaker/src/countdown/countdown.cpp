//
// Created by fadhil on 9/2/25.
//

#include <esp32-hal.h>
#include <countdown/countdown.h>
#include "UI/ui.h"




int ShakerCountdown::convertToSeconds() const {
    const int secondsFromHours = this->hours * 3600;
    const int secondsFromMinutes = this->minutes * 60;
    const int secondsFromSeconds = this->seconds;
    return secondsFromHours + secondsFromMinutes + secondsFromSeconds;
}


HMS ShakerCountdown::convertSecondToHMS() const {
    HMS result{};
    result.hours = this->_totalSeconds / 3600;
    result.minutes = (static_cast<int>(this->_totalSeconds) % 3600) / 60;
    result.seconds = static_cast<int>(this->_totalSeconds) % 60;
    return result;
}

void ShakerCountdown::setState(CountDownState state) {
    this->state = state;
}

void ShakerCountdown::getTimeString(char *buffer, size_t bufferSize) const {
    const HMS hms = this->convertSecondToHMS();
    snprintf(buffer, bufferSize, "%02d:%02d:%02d", hms.hours, hms.minutes, hms.seconds);
};


ShakerCountdown::ShakerCountdown() = default;

CountDownState ShakerCountdown::getState() const {
    return this->state;
}

void ShakerCountdown::setTime(float hour, float minute, float second) {
    this->hours = hour;
    this->minutes = minute;
    this->seconds = second;
    this->_totalSeconds = this->convertToSeconds();
    this->_lastTotalSeconds = this->_totalSeconds;
    this->_lastHours = this->hours;
    this->_lastMinutes = this->minutes;
    this->_lastSeconds = this->seconds;
}


void ShakerCountdown::resetVariables() {
    this->hours = 0;
    this->minutes = 0;
    this->seconds = 0;

    this->_lastHours = 0;
    this->_lastMinutes = 0;
    this->_lastSeconds = 0;

    this->_totalSeconds = 0;
    this->_lastTotalSeconds = 0;
    this->setState(CountDownState::STOPPED);
}



void ShakerCountdown::countDown() {
    this->setState(CountDownState::COUNTING);
    const unsigned long now = millis();
    if (now - this->lastTime >= 10) {
        this->lastTime = now;
        this->_totalSeconds -= 1;
        if (this->_totalSeconds <= 0) {
            this->resetVariables();
            this->setState(CountDownState::DONE);
        }
    }
}


void ShakerCountdown::run() {
    const CountDownState currentState = this->getState();
    if (currentState== CountDownState::STOPPED || currentState == CountDownState::DONE) {
        return;
    }
    countDown();
}

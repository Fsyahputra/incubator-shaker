//
// Created by fadhil on 9/2/25.
//

#ifndef PROGRAM_SHAKER_COUNTDOWN_H
#define PROGRAM_SHAKER_COUNTDOWN_H


enum class CountDownState {
    COUNTING,
    STOPPED,
    DONE,
};


struct HMS {
    int hours;
    int minutes;
    int seconds;
};

class ShakerCountdown {


private:
    CountDownState state = CountDownState::STOPPED;
    unsigned long lastTime = 0;

    float hours = 0;
    float minutes = 0;
    float seconds = 0;

    float _lastHours = 0;
    float _lastMinutes = 0;
    float _lastSeconds = 0;

    float _totalSeconds = 0;
    float _lastTotalSeconds = 0;

    int convertToSeconds() const;


    HMS convertSecondToHMS() const;
    void countDown();


public:
    ShakerCountdown();
    CountDownState getState() const;
    void resetVariables();
    void setState(CountDownState state);
    void setTime(float hour, float minute, float second);
    void run();
    void getTimeString(char *buffer, size_t bufferSize) const;
};

#endif //PROGRAM_SHAKER_COUNTDOWN_H

#ifndef STEPPER_H
#define STEPPER_H

#include <constant/hardware.h>

enum class StepperState
{
  RUN,
  STOP,
};

enum class InternalStepperState
{
  RUNNING_AT_SPEED,
  ACCELERATING,
  DECELERATING,
  STOPPED,
  RESETTING,
};

class ShakerStepper
{
private:
  StepperState state = StepperState::STOP;
  InternalStepperState internalState = InternalStepperState::STOPPED;
  long STEP_PER_REV = Hardware::Stepper::STEP_PER_REV;
  const unsigned long baseIntervalStep = 625;
  unsigned long currentInterval = this->baseIntervalStep;
  float speed = 0;
  float acceleration = 0;
  int stepPins[4];
  int dirPins[4];
  unsigned long lastStepTime = 0;
  InternalStepperState getInternalState();
  void initializePins();
  void setInternalState(InternalStepperState newState);
  float convertRpmToStepPerMicros(float rpm);
  int convertStepToInterval();
  void handleAcceleration();
  void handleDeceleration();
  void handleRunningAtSpeed();
  float convertRpmToStepPerMicros2(float accRpm);

  void sendHighPulse();
  void sendLowPulse();

public:
  ShakerStepper(int stepPins[], int dirPins[]);
  void setSpeed(float rpm);
  void setAcceleration(float rpm);
  void run();
  void setState(StepperState newState);
  StepperState getState();
  float getSpeed();
  float getAcceleration();
};

#endif
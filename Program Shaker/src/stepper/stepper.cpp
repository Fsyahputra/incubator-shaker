#include <stepper/stepper.h>
#include <Arduino.h>

void ShakerStepper::init()
{
  for (int i = 0; i < sizeof(stepPins) / sizeof(stepPins[0]); i++)
  {
    pinMode(stepPins[i], OUTPUT);
    pinMode(dirPins[i], OUTPUT);
    digitalWrite(dirPins[i], LOW);
    digitalWrite(stepPins[i], LOW);
  }
  this->state = StepperState::STOP;
  this->internalState = InternalStepperState::STOPPED;
}

ShakerStepper::ShakerStepper(int stepPins[], int dirPins[])
{
  for (int i = 0; i < 4; i++)
  {
    this->stepPins[i] = stepPins[i];
    this->dirPins[i] = dirPins[i];
  }
}

void ShakerStepper::sendHighPulse()
{
  for (int i = 0; i < sizeof(stepPins) / sizeof(stepPins[0]); i++)
  {
    digitalWrite(stepPins[i], HIGH);
  }
}

void ShakerStepper::sendLowPulse()
{
  for (int i = 0; i < sizeof(stepPins) / sizeof(stepPins[0]); i++)
  {
    digitalWrite(stepPins[i], LOW);
  }
}

float ShakerStepper::convertRpmToStepPerMicros(float rpm)
{
  return (rpm * STEP_PER_REV) / (60.0f * 1000000.0f);
}

InternalStepperState ShakerStepper::getInternalState()
{
  return this->internalState;
}

void ShakerStepper::setInternalState(InternalStepperState newState)
{
  this->internalState = newState;
}

void ShakerStepper::setSpeed(float rpm)
{
  this->speed = rpm;
}

void ShakerStepper::setAcceleration(float rpm)
{
  this->acceleration = rpm;
}

float ShakerStepper::convertRpmToStepPerMicros2(float accRpm)
{
  float stepsPerSec2 = (accRpm * STEP_PER_REV) / 60.0f;
  return stepsPerSec2 / 1e12f;
}

void ShakerStepper::handleAcceleration()
{
  unsigned long currentTime = micros();

  float stepInterval = 1.0f / this->convertRpmToStepPerMicros(this->speed); // µs per step target
  float a = this->convertRpmToStepPerMicros2(this->acceleration);

  if ((float)(currentTime - this->lastStepTime) >= this->currentInterval)
  {
    this->lastStepTime = currentTime;

    this->sendHighPulse();
    delayMicroseconds(2);
    this->sendLowPulse();

    if (this->currentInterval > stepInterval)
    {
      this->currentInterval = this->currentInterval / (1.0f + a * this->currentInterval * this->currentInterval);
      if (this->currentInterval < stepInterval)
        this->currentInterval = stepInterval;
      this->setInternalState(InternalStepperState::ACCELERATING);
    }
    else
    {
      this->setInternalState(InternalStepperState::RUNNING_AT_SPEED);
    }
  }
}

void ShakerStepper::handleRunningAtSpeed()
{
  unsigned long currentTime = micros();
  if ((float)(currentTime - this->lastStepTime) >= this->currentInterval)
  {
    this->lastStepTime = currentTime;
    this->sendHighPulse();
    delayMicroseconds(2);
    this->sendLowPulse();
  }
}

float ShakerStepper::convertStepPerMicrosToRpm(float stepPerMicros)
{
  return (  60.0f * 1000000.0f) / (STEP_PER_REV * stepPerMicros);
}

float ShakerStepper::getSpeed()
{
  if (this->getInternalState() == InternalStepperState::STOPPED) return  0.0;
  return this->convertStepPerMicrosToRpm(this->currentInterval);
}

void ShakerStepper::handleDeceleration()
{
  unsigned long currentTime = micros();

  if ((float)(currentTime - lastStepTime) >= currentInterval)
  {
    lastStepTime = currentTime;

    this->sendHighPulse();
    delayMicroseconds(2);
    this->sendLowPulse();
    float currentIntervalF = (float)this->currentInterval;
    currentIntervalF += 1.0f;
    this->currentInterval = (unsigned long)currentIntervalF;
    this->setInternalState(InternalStepperState::DECELERATING);
    if (this->getSpeed() < 4.0f)
    {
      currentInterval = this->baseIntervalStep;
      setInternalState(InternalStepperState::STOPPED);
    }
  }
}

void ShakerStepper::setState(StepperState newState)
{
  this->state = newState;
}

StepperState ShakerStepper::getState()
{
  return this->state;
}

void ShakerStepper::run()
{
  StepperState currentState = this->getState();
  InternalStepperState currentInternalState = this->getInternalState();
  switch (currentState)
  {

  case StepperState::RUN:
    switch (currentInternalState)
    {
    case InternalStepperState::STOPPED:
      this->handleAcceleration();
      break;
    case InternalStepperState::RUNNING_AT_SPEED:
      this->handleRunningAtSpeed();
      break;
    default:
        this->handleAcceleration();
    }
    break;

  case StepperState::STOP:
    switch (currentInternalState)
    {
    case InternalStepperState::STOPPED:
      this->sendLowPulse();
      break;
    default:
        this->handleDeceleration();
        break;
    }
    break;
  }
}
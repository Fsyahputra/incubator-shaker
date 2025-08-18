#include <stepper/stepper.h>
#include <Arduino.h>
#include <AccelStepper.h>
#include <constant/hardware.h>

long ShakerStepper::STEP_PER_REV = Hardware::Stepper::STEP_PER_REV;

void ShakerStepper::initSteppers()
{
  Astepper = AccelStepper(AccelStepper::DRIVER, Hardware::Stepper::Step::A, Hardware::Stepper::Dir::A);
  Xstepper = AccelStepper(AccelStepper::DRIVER, Hardware::Stepper::Step::X, Hardware::Stepper::Dir::X);
  Ystepper = AccelStepper(AccelStepper::DRIVER, Hardware::Stepper::Step::Y, Hardware::Stepper::Dir::Y);
  Zstepper = AccelStepper(AccelStepper::DRIVER, Hardware::Stepper::Step::Z, Hardware::Stepper::Dir::Z);

  this->resetSpeed();
  this->resetAcceleration();
  this->resetPosition();!
}

ShakerStepper::ShakerStepper()
{
  initSteppers();
  setSpeed(0);
  setAcceleration(0);
}

void ShakerStepper::resetSpeed()
{
  Astepper.setSpeed(0);
  Xstepper.setSpeed(0);
  Ystepper.setSpeed(0);
  Zstepper.setSpeed(0);
}

void ShakerStepper::resetAcceleration()
{
  Astepper.setAcceleration(0);
  Xstepper.setAcceleration(0);
  Ystepper.setAcceleration(0);
  Zstepper.setAcceleration(0);
}

void ShakerStepper::resetPosition()
{
  Astepper.setCurrentPosition(0);
  Xstepper.setCurrentPosition(0);
  Ystepper.setCurrentPosition(0);
  Zstepper.setCurrentPosition(0);
}

float ShakerStepper::convertRpmToStep(float param)
{
  float stepPerSec = (param * STEP_PER_REV) / 60.0;
  return stepPerSec;
}

void ShakerStepper::setSpeed(float rpm)
{
  this->speed = rpm;
}

void ShakerStepper::setAcceleration(float rpm)
{
  this->acceleration = rpm;
}

void ShakerStepper::setState(ShakerStepper::StepperState state)
{
  this->state = state;
}

ShakerStepper::StepperState ShakerStepper::getState()
{
  return state;
}

void ShakerStepper::stop()
{
  this->state = STOPPED;
  Astepper.stop();
  Xstepper.stop();
  Ystepper.stop();
  Zstepper.stop();
  Astepper.setSpeed(0);
  Xstepper.setSpeed(0);
  Ystepper.setSpeed(0);
  Zstepper.setSpeed(0);
}

void ShakerStepper::run()
{
  if (this->state == CONFIGURING)
  {
    this->deaccelerate();
    return;
  }

  if (this->state == STOPPED)
  {
    this->stop();
    return;
  }
}

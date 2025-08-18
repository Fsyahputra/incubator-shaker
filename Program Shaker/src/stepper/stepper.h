
#ifndef STEPPER_H
#define STEPPER_H

#include <AccelStepper.h>
#include <constant/hardware.h>

class ShakerStepper
{
private:
  enum StepperState
  {
    RUNNING,
    STOPPED,
    RESETTING,
    CONFIGURING,
  };
  AccelStepper Xstepper;
  AccelStepper Ystepper;
  AccelStepper Zstepper;
  AccelStepper Astepper;

  StepperState state = STOPPED;
  static long STEP_PER_REV;
  float speed = 0;
  float acceleration = 0;
  float convertRpmToStep(float param);
  void resetSpeed();
  void resetAcceleration();
  void resetPosition();
  void deaccelerate();
  void initSteppers();
  void updateSteppers();

public:
  ShakerStepper();
  void setSpeed(float rpm);
  void setAcceleration(float rpm);
  void run();
  void stop();
  void reset();
  float getSpeed();
  float getAcceleration();
  StepperState getState();
  void setState(StepperState state);
};

#endif
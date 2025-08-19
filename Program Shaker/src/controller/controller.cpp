#include <controller/controller.h>
#include <stepper/stepper.h>
#include <UI/ui.h>

ShakerController::ShakerController(ShakerStepper &stepper, ShakerUI &ui)
    : stepper(stepper), ui(ui)
{
}

void ShakerController::init()
{
  this->ui.init();
  this->stepper.init();
}

void ShakerController::run()
{
  ui.run();
  stepper.run();
  UIState currentUiState = ui.getState();
  InternalStepperState currentInternalState = stepper.getInternalState();

  if (currentUiState != UIState::READY)
  {
    stepper.setState(StepperState::STOP);
    return;
  }

  if (currentInternalState == InternalStepperState::DECELERATING)
  {
    stepper.setState(StepperState::STOP);
    return;
  }

  float rpm = ui.getRpm();
  float time = ui.getTime();
  float temperature = ui.getTemperature();

  if (rpm < 0 || time < 0 || temperature < 0)
  {
    stepper.setState(StepperState::STOP);
    return;
  }
  stepper.setSpeed(rpm);
  stepper.setAcceleration(rpm);
  stepper.setState(StepperState::RUN);
}

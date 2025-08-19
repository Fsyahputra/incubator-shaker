#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <stepper/stepper.h>
#include <UI/ui.h>

class ShakerController
{

private:
  ShakerStepper &stepper;
  ShakerUI &ui;

public:
  ShakerController(ShakerStepper &stepper, ShakerUI &ui);
  void init();
  void run();
};

#endif
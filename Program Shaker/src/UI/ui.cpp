#include <UI/ui.h>

void ShakerUI::setState(UIState newState)
{
  state = newState;
}

internalUiState ShakerUI::getInternalState()
{
  return this->internalState;
}

void ShakerUI::init()
{
  this->rotaryEncoder.initializePins();
  this->lcdDisplay.initDisplay();
  this->setState(UIState::CONFIGURING);
  this->setInternalState(internalUiState::NOT_CONFIGURED);
  this->lcdDisplay.setConfigTitle("Configuring");
  this->lcdDisplay.setConfigValue("...", 0.0);
}

ShakerUI::ShakerUI(RotaryEncoder &rotaryEncoder, ShakerDisplay &lcdDisplay)
    : rotaryEncoder(rotaryEncoder), lcdDisplay(lcdDisplay)
{
}

void ShakerUI::next()
{
  internalUiState currentState = this->getInternalState();
  switch (currentState)
  {
  case internalUiState::NOT_CONFIGURED:
    this->setInternalState(internalUiState::CONFIGURE_RPM);
    break;
  case internalUiState::CONFIGURE_RPM:
    this->setInternalState(internalUiState::CONFIGURE_TIME);
    break;
  case internalUiState::CONFIGURE_TIME:
    this->setInternalState(internalUiState::CONFIGURE_TEMPERATURE);
    break;
  case internalUiState::CONFIGURE_TEMPERATURE:
    this->setInternalState(internalUiState::CONFIGURED);
    break;
  case internalUiState::CONFIGURED:
    this->setInternalState(internalUiState::NOT_CONFIGURED);
    break;
  }
}

UIState ShakerUI::getState()
{
  return state;
}

void ShakerUI::setButtonPress()
{
  this->nthButtonPress++;
}

void ShakerUI::setInternalState(internalUiState newState)
{
  this->internalState = newState;
}

void ShakerUI::setMinMaxParam(float &param, float min, float max)
{
  if (param < min)
  {
    param = min;
  }
  else if (param > max)
  {
    param = max;
  }
}

void ShakerUI::configureParam(float &param, RotaryState rotaryState, int increment)
{
  if (rotaryState == RotaryState::RIGHT)
  {
    param += increment;
  }
  else if (rotaryState == RotaryState::LEFT)
  {
    param -= increment;
  }
}

void ShakerUI::handleConfigureRPM(RotaryState rotaryState, RotaryButtonState buttonState)
{
  this->setMinMaxParam(this->rpm, 0, 300);
  this->configureParam(this->rpm, rotaryState, 10);
  this->lcdDisplay.setConfigTitle("Configuring RPM");
  this->lcdDisplay.setConfigValue("RPM", this->rpm);
  if (buttonState == RotaryButtonState::PRESSED)
  {
    this->lcdDisplay.setState(displayState::CONFIGURING);
    this->next();
  }
}

void ShakerUI::handleConfigureTime(RotaryState rotaryState, RotaryButtonState buttonState)
{
  this->setMinMaxParam(this->time, 0, 3600);
  this->configureParam(this->time, rotaryState, 1);
  this->lcdDisplay.setConfigTitle("Configuring Time");
  this->lcdDisplay.setConfigValue("Time", this->time);
  if (buttonState == RotaryButtonState::PRESSED)
  {
    this->lcdDisplay.setState(displayState::CONFIGURING);
    this->next();
  }
}

void ShakerUI::handleConfigureTemperature(RotaryState rotaryState, RotaryButtonState buttonState)
{
  this->setMinMaxParam(this->temperature, 0, 60);
  this->configureParam(this->temperature, rotaryState, 1);
  this->lcdDisplay.setConfigTitle("Configuring Temperature");
  this->lcdDisplay.setConfigValue("Temperature", this->temperature);
  if (buttonState == RotaryButtonState::PRESSED)
  {
    this->lcdDisplay.setState(displayState::READY);
    this->next();
  }
}

void ShakerUI::reset()
{
  this->rpm = 0;
  this->time = 0;
  this->temperature = 0;
  this->nthButtonPress = 0;
  this->lcdDisplay.setState(displayState::CONFIGURING);
}

void ShakerUI::handleMultipleButtonPress(int pressCount, internalUiState nextState)
{
  if (this->nthButtonPress >= pressCount)
  {
    this->setInternalState(nextState);
    this->nthButtonPress = 0;
  }

  this->setButtonPress();
}

float ShakerUI::getRpm()
{
  UIState currentState = this->getState();
  if (currentState != UIState::READY)
  {
    return -1.0f;
  }
  return this->rpm;
}

float ShakerUI::getTime()
{
  UIState currentState = this->getState();
  if (currentState != UIState::READY)
  {
    return -1.0f;
  }
  return this->time;
}

float ShakerUI::getTemperature()
{
  UIState currentState = this->getState();
  if (currentState != UIState::READY)
  {
    return -1.0f;
  }
  return this->temperature;
}

void ShakerUI::run()
{
  this->rotaryEncoder.run();
  this->lcdDisplay.run();
  RotaryState rotaryState = this->rotaryEncoder.getState();
  RotaryButtonState buttonState = this->rotaryEncoder.getButtonState();

  internalUiState internalState = this->getInternalState();

  if (internalState == internalUiState::NOT_CONFIGURED)
  {
    this->setState(UIState::CONFIGURING);
    this->lcdDisplay.setState(displayState::CONFIGURING);
    this->lcdDisplay.setConfigTitle("NOT CONFIGURED");
    this->lcdDisplay.setConfigValue("...", 0.0);
    if (buttonState == RotaryButtonState::PRESSED)
    {
      this->handleMultipleButtonPress(3, internalUiState::CONFIGURE_RPM);
    }
    return;
  }

  if (internalState == internalUiState::CONFIGURED)
  {
    this->setState(UIState::READY);
    if (buttonState == RotaryButtonState::PRESSED)
    {
      this->handleMultipleButtonPress(3, internalUiState::NOT_CONFIGURED);
      if (this->nthButtonPress == 3) {
        this->reset();
      }
    }
    return;
  }

  switch (internalState)
  {
  case internalUiState::CONFIGURE_RPM:
    this->handleConfigureRPM(rotaryState, buttonState);
    break;
  case internalUiState::CONFIGURE_TIME:
    this->handleConfigureTime(rotaryState, buttonState);
    break;
  case internalUiState::CONFIGURE_TEMPERATURE:
    this->handleConfigureTemperature(rotaryState, buttonState);
    break;
  }
}
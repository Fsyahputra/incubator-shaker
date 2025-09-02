#include <UI/ui.h>
#include <countdown/countdown.h>

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

ShakerUI::ShakerUI(RotaryEncoder &rotaryEncoder, ShakerDisplay &lcdDisplay, ShakerCountdown &counter)
    : rotaryEncoder(rotaryEncoder), lcdDisplay(lcdDisplay), counter(counter)
{
}

void ShakerUI::next()
{
  internalUiState currentState = this->getInternalState();
  switch (currentState) {
    case internalUiState::NOT_CONFIGURED:
      this->setInternalState(internalUiState::CONFIGURE_RPM);
      break;

    case internalUiState::CONFIGURE_RPM:
      this->setInternalState(internalUiState::CONFIGURE_HOURS);
      break;

    case internalUiState::CONFIGURE_HOURS:
      this->setInternalState(internalUiState::CONFIGURE_MINUTES);
      break;

    case internalUiState::CONFIGURE_MINUTES:
      this->setInternalState(internalUiState::CONFIGURE_SECONDS);
      break;

    case internalUiState::CONFIGURE_SECONDS:
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

void ShakerUI::handleConfigureHours(RotaryState rotaryState, RotaryButtonState buttonState) {
  ShakerUI::setMinMaxParam(this->hours, 0, 24);
  this->configureParam(this->hours, rotaryState, 1);
  this->lcdDisplay.setConfigTitle("Configuring Hours");
  this->lcdDisplay.setConfigValue("Hours", this->hours);
  if (buttonState == RotaryButtonState::PRESSED) {
    this->lcdDisplay.setState(displayState::CONFIGURING);
    this->next();
  }
}

void ShakerUI::handleConfigureMinutes(RotaryState rotaryState, RotaryButtonState buttonState) {
  ShakerUI::setMinMaxParam(this->minutes, 0, 59);
  this->configureParam(this->minutes, rotaryState, 1);
  this->lcdDisplay.setConfigTitle("Configuring Minutes");
  this->lcdDisplay.setConfigValue("Minutes", this->minutes);
  if (buttonState == RotaryButtonState::PRESSED) {
    this->lcdDisplay.setState(displayState::CONFIGURING);
    this->next();
  }
}

void ShakerUI::handleConfigureSeconds(RotaryState rotaryState, RotaryButtonState buttonState) {
  ShakerUI::setMinMaxParam(this->seconds, 0, 59);
  this->configureParam(this->seconds, rotaryState, 1);
  this->lcdDisplay.setConfigTitle("Configuring Seconds");
  this->lcdDisplay.setConfigValue("Seconds", this->seconds);
  if (buttonState == RotaryButtonState::PRESSED) {
    this->lcdDisplay.setState(displayState::CONFIGURING);
    this->next();
  }
}




void ShakerUI::handleConfigureTime(RotaryButtonState buttonState)
{
  this->counter.setTime(this->hours, this->minutes, this->seconds);
  char timeStamp[9];
  this->counter.getTimeString(timeStamp, sizeof(timeStamp));
  this->lcdDisplay.setConfigTitle("Configuring Time");
  this->lcdDisplay.setConfigValue(timeStamp, 0.0);
  if (buttonState == RotaryButtonState::PRESSED)
  {
    this->counter.setState(CountDownState::COUNTING);
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
  this->hours = 0;
  this->minutes = 0;
  this->seconds = 0;
  this->counter.resetVariables();
  this->temperature = 0;
  this->nthButtonPress = 0;
  this->lcdDisplay.setState(displayState::CONFIGURING);
  this->setInternalState(internalUiState::NOT_CONFIGURED);
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
  return 0.0;
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
  this->counter.run();
  RotaryState rotaryState = this->rotaryEncoder.getState();
  RotaryButtonState buttonState = this->rotaryEncoder.getButtonState();
  internalUiState internalState = this->getInternalState();
  CountDownState countdownState = this->counter.getState();

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
  case internalUiState::CONFIGURE_HOURS:
    this->handleConfigureHours(rotaryState, buttonState);
    break;
  case internalUiState::CONFIGURE_MINUTES:
    this->handleConfigureMinutes(rotaryState, buttonState);
    break;
  case internalUiState::CONFIGURE_SECONDS:
    this->handleConfigureSeconds(rotaryState, buttonState);
    break;
  case internalUiState::CONFIGURE_TIME:
    this->handleConfigureTime(buttonState);
    break;
  case internalUiState::CONFIGURE_TEMPERATURE:
    this->handleConfigureTemperature(rotaryState, buttonState);
    break;
  }

  if (countdownState == CountDownState::DONE) {
    this->setInternalState(internalUiState::NOT_CONFIGURED);
    this->reset();
  }
}
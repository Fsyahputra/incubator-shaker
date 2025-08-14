#include <AccelStepper.h>
#include <Arduino.h>

long STEP_PER_REV = 3200;

AccelStepper Aaxis(AccelStepper::DRIVER, 17, 16);
AccelStepper Zaxis(AccelStepper::DRIVER, 32, 33);
AccelStepper Yaxis(AccelStepper::DRIVER, 14, 25);
AccelStepper Xaxis(AccelStepper::DRIVER, 12, 26);

float motorSpeedRpm = 1000;
float motorAccelRpmPerSec = 1000;
volatile bool motorRunning = true;

void setMotorParamsRPM(AccelStepper &motor, float rpm, float accelRPMperSec)
{
  float stepsPerSec = (rpm * STEP_PER_REV) / 60.0;
  float stepsPerSec2 = (accelRPMperSec * STEP_PER_REV) / 60.0;

  motor.setMaxSpeed(int(stepsPerSec));
  motor.setAcceleration(int(stepsPerSec2));
}

void stepperTask(void *parameter)
{
  for (;;)
  {
    if (motorRunning)
    {
      Aaxis.run();
      Zaxis.run();
      Yaxis.run();
      Xaxis.run();
      Aaxis.move(100 * STEP_PER_REV);
      Zaxis.move(100 * STEP_PER_REV);
      Yaxis.move(100 * STEP_PER_REV);
      Xaxis.move(100 * STEP_PER_REV);
    }
  }
}

String inputBuffer = "";

void serialTask(void *parameter)
{
  for (;;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      Serial.write(c);
      if (c == 0x08 || c == 0x7F)
      {
        if (inputBuffer.length() > 0)
        {
          inputBuffer.remove(inputBuffer.length() - 1);
          Serial.print("\b \b");
        }
      }
      else if (c == '\n' || c == '\r')
      {
        inputBuffer.trim();
        if (inputBuffer.length() > 0)
        {
          if (inputBuffer.startsWith("SET"))
          {
            int firstSpace = inputBuffer.indexOf(' ');
            int secondSpace = inputBuffer.indexOf(' ', firstSpace + 1);
            int thirdSpace = inputBuffer.indexOf(' ', secondSpace + 1);
            if (firstSpace > 0 && secondSpace > firstSpace && thirdSpace > secondSpace)
            {
              float rpm = inputBuffer.substring(firstSpace + 1, secondSpace).toFloat();
              float accel = inputBuffer.substring(secondSpace + 1).toFloat();
              motorSpeedRpm = rpm;
              motorAccelRpmPerSec = accel;
              String motor = inputBuffer.substring(thirdSpace + 1);

              if (motor.equalsIgnoreCase("A") || motor.equalsIgnoreCase("ALL"))
              {
                setMotorParamsRPM(Aaxis, rpm, accel);
              }
              if (motor.equalsIgnoreCase("Z") || motor.equalsIgnoreCase("ALL"))
              {
                setMotorParamsRPM(Zaxis, rpm, accel);
              }
              if (motor.equalsIgnoreCase("Y") || motor.equalsIgnoreCase("ALL"))
              {
                setMotorParamsRPM(Yaxis, rpm, accel);
              }
              if (motor.equalsIgnoreCase("X") || motor.equalsIgnoreCase("ALL"))
              {
                setMotorParamsRPM(Xaxis, rpm, accel);
              }
              if (motor.equalsIgnoreCase("ALL"))
              {
                setMotorParamsRPM(Aaxis, rpm, accel);
                setMotorParamsRPM(Zaxis, rpm, accel);
                setMotorParamsRPM(Yaxis, rpm, accel);
                setMotorParamsRPM(Xaxis, rpm, accel);
              }

              Serial.print("\nRPM di-set ke: ");
              Serial.println(rpm);
              Serial.print("Akselerasi di-set ke: ");
              Serial.println(accel);
            }
            else
            {
              Serial.println("\nFormat SET salah!");
            }
          }
          else if (inputBuffer.equalsIgnoreCase("START"))
          {
            motorRunning = true;
            Serial.println("\nMotor mulai berjalan...");
          }
          else if (inputBuffer.equalsIgnoreCase("STOP"))
          {
            motorRunning = false;
            Serial.println("\nMotor dihentikan.");
          }
          else
          {
            Serial.println("\nPerintah tidak dikenal.");
          }
        }
        inputBuffer = "";
        Serial.print("> "); // prompt
      }
      else
      {
        inputBuffer += c;
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); // cek setiap 10 ms
  }
}

void setup()
{
  disableCore0WDT();
  disableCore1WDT();

  Serial.begin(115200);

  Aaxis.setCurrentPosition(0);
  Zaxis.setCurrentPosition(0);
  Yaxis.setCurrentPosition(0);
  Xaxis.setCurrentPosition(0);

  setMotorParamsRPM(Aaxis, motorSpeedRpm, motorAccelRpmPerSec);
  setMotorParamsRPM(Zaxis, motorSpeedRpm, motorAccelRpmPerSec);
  setMotorParamsRPM(Yaxis, motorSpeedRpm, motorAccelRpmPerSec);
  setMotorParamsRPM(Xaxis, motorSpeedRpm, motorAccelRpmPerSec);

  // xTaskCreate(stepperTask, "StepperTask", 4096, NULL, 2, NULL);
  xTaskCreate(serialTask, "SerialTask", 4096, NULL, 1, NULL);
}

void loop()
{
  if (motorRunning)
  {
    Aaxis.run();
    Zaxis.run();
    Yaxis.run();
    Xaxis.run();
    Aaxis.move(100 * STEP_PER_REV);
    Zaxis.move(100 * STEP_PER_REV);
    Yaxis.move(100 * STEP_PER_REV);
    Xaxis.move(100 * STEP_PER_REV);
  }
  else
  {

    Aaxis.setCurrentPosition(0);
    Zaxis.setCurrentPosition(0);
    Yaxis.setCurrentPosition(0);
    Xaxis.setCurrentPosition(0);
    Aaxis.move(0);
    Zaxis.move(0);
    Yaxis.move(0);
    Xaxis.move(0);
  }
}

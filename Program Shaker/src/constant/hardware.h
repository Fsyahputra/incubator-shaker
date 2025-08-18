#ifndef HARDWARE_H
#define HARDWARE_H

namespace Hardware
{
  namespace Stepper
  {
    namespace Step
    {
      const int X = 12;
      const int Y = 32;
      const int Z = 14;
      const int A = 12;
    }
    namespace Dir
    {
      const int X = 16;
      const int Y = 33;
      const int Z = 25;
      const int A = 26;
    }
    const long STEP_PER_REV = 3200;
  }

  namespace LcdI2C
  {
    const int SDA = 21;
    const int SCL = 22;
  }

  namespace RotaryEncoder
  {
    const int A = 34;
    const int B = 35;
    const int BUTTON = 36;
  }

  namespace SerialCfg
  {
    const int BAUD_RATE = 115200;
  }
}

#endif
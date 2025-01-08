/*
 * Basic example code on how to control via I2C your geared DC motor with quadrature
 * encoder which is already connected to the Easy PID Motor Controller module and have already
 * succesfully set up their velocity PID control using the epmc_setup_application
 *
 * The code basically sends a low PWM value, waits for some time and then
 * sends a high PWM value. it also prints out the motors' angular positions
 * (in rad) and angular velocities (in rad/s).
 *
 * you can copy the code and use it in your project as you will.
 */

// Easy PID Motor Controller i2c communication library
#include <epmc.h>

int epmcAddress = 1; // set this address to the same address you have during setup via the GUI app
EPMC motor(epmcAddress);

///////// my sepcial delay function ///////////////
void delayMs(int ms)
{
  for (int i = 0; i < ms; i += 1)
  {
    delayMicroseconds(1000);
  }
}
//////////////////////////////////////////////////

bool isSuccessful;
float angPosA, angPosB; // motorA, motorB (in rad)
float angVelA, angVelB; // motorA, motorB (in rad/sec)

int lowPwmVal = 100;
int highPwmVal = 200;
bool sendHigh = true;

long prevTime;
long sampleTime = 100; // millisec

long ctrlPrevTime;
long ctrlSampleTime = 5000; // millisec

void setup()
{
  // start i2c communication
  Wire.begin();

  // setup serial communication to print result on serial minitor
  Serial.begin(115200);

  // wait for the driver module to fully setup (5 to 10 sec)
  for (int i = 0; i <= 6; i += 1)
  {
    delayMs(1000);
    Serial.println(i);
  }
  motor.sendPwm(0, 0); // pwmA, pwmB

  // int cmd_vel_timeout = 2000;           // 0 to deactivate.
  // motor.setCmdTimeout(cmd_vel_timeout); // set motor command velocity timeout
  // motor.getCmdTimeout(cmd_vel_timeout); // get the stored command velocity timeout
  // Serial.print("motor command vel timeout in ms: ");
  // Serial.println(cmd_vel_timeout);

  motor.sendPwm(lowPwmVal, lowPwmVal); // pwmA, pwmB
  sendHigh = true;

  prevTime = millis();
  ctrlPrevTime = millis();
}

void loop()
{
  if ((millis() - ctrlPrevTime) >= ctrlSampleTime)
  {
    if (sendHigh)
    {
      motor.sendPwm(highPwmVal, highPwmVal); // pwmA, pwmB
      sendHigh = false;
    }
    else
    {
      motor.sendPwm(lowPwmVal, lowPwmVal); // pwmA, pwmB
      sendHigh = true;
    }
    ctrlPrevTime = millis();
  }

  if ((millis() - prevTime) >= sampleTime)
  {
    /* CODE SHOULD GO IN HERE*/

    motor.getMotorsPos(angPosA, angPosB); // gets angPosA, angPosB
    motor.getMotorsVel(angVelA, angVelB); // gets angVelA, angVelB

    Serial.print(angPosA, 3);
    Serial.print(", ");
    Serial.println(angVelA, 3);

    Serial.print(angPosB, 3);
    Serial.print(", ");
    Serial.println(angVelB, 3);

    Serial.println();

    prevTime = millis();
  }
}
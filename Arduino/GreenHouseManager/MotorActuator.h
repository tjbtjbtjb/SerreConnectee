/**
 * @file      MotorActuator.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      February, 2018
 * @brief     Simple Motor actuator based on I2C Grove system
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _MOTOR_ACTUATOR_H_
#define _MOTOR_ACTUATOR_H_

#include <Grove_I2C_Motor_Driver.h>
#include "Actuator.h"

class MotorActuator : public Actuator {
  public:
    MotorActuator(String s,unsigned char addr) : Actuator(s), m_addr(addr) { 
      //Wire.begin();
      Motor.begin(m_addr);
      delay(500);
    } ;
    virtual ~MotorActuator() {} ;
    void setValue(int step) { 
      Motor.StepperRun(step);
     } ;
     void setValue(float step) { setValue((int)round(step)); };
  private:
    unsigned char m_addr;
};

#endif

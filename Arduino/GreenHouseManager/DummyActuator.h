
/**
 * @file      DummyActuator.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Dummy prototype for an actuator
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _DUMMY_ACTUATOR_H_
#define _DUMMY_ACTUATOR_H_

#include "Actuator.h"

class DummyActuator : public Actuator {
  public:
    DummyActuator(String s) : Actuator(s) {} ;
    virtual ~DummyActuator() {} ;
    void setValue(int) const {} ;
    void setValue(float) const {} ;
};

#endif

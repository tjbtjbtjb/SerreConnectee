/**
 * @file      Device.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Header file for the Device master class, used by Sensor and Actuator
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_

class Device {
  public:
    Device(String s) : m_ID(s) {} ;
    String getID() const { return m_ID; };
    virtual ~Device() {} ;
  private:
    String m_ID;
};

#endif

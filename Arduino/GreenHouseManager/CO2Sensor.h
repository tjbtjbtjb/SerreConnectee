/**
 * @file      CO2Sensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     CO2 sensor class
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _CO2_SENSOR_H_
#define _CO2_SENSOR_H_

#include "Arduino.h"         //needed for delay()
#include "SoftwareSerial.h"
#include "Sensor.h"

class CO2Sensor : public Sensor {
  public:
    CO2Sensor(String, int, int) ; // name, rx, tx
    virtual  ~CO2Sensor() {} ;
    float    getValue() const { return readData()?(float)getRawCO2PPM():-1. ; };
    float    getTemperature() { return readData()?(float)getRawTemperature():-300. ; };
    int      getRawCO2PPM() { return (int)m_data[2] * 256 + (int)m_data[3] ; } ;
    int      getRawTemperature() { return (int)m_data[4] - 40 ; } ;

  public:
    SoftwareSerial* getSerialSensor() const {return m_pSerialSensor;};
  private:
    bool readData() ;
  private:
    int                    m_tx, m_rx;
    SoftwareSerial        *m_pSerialSensor;
    unsigned char          m_data[9];
    const unsigned char    m_cmdGetSensor[9] = {
      0xff, 0x01, 0x86, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x79 
    };
};

#endif

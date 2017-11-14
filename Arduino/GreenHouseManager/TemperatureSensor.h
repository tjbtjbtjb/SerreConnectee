/**
 * @file      TemperatureSensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Temperature sensor class based on Humidity/Temp Groove module
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _TEMPERATURE_SENSOR_H_
#define _TEMPERATURE_SENSOR_H_

#include "Sensor.h"

#include "DHT.h"

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

class TemperatureSensor : public Sensor {
  public:
    TemperatureSensor(String s, int p) : Sensor(s), m_pin(p)  { m_pDht = new DHT(m_pin, DHTTYPE);} ;
    virtual ~TemperatureSensor() {} ;
    float getValue() {return m_pDht->readTemperature(); } ;
  public:
    DHT* getDht() const {return m_pDht;};
  private:
    int m_pin;
    DHT *m_pDht;
};

#endif

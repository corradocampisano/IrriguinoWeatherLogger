/*
  Measurements.cpp - Library for handling measurements for Irriguino sensors
*/



#include "Arduino.h"
#include "Measurements.h"

//#include "/home/corrado/sketchbook/IrriguinoWeather0006/Libs_DHT_11/dht11.h"
#include <dht11.h>

dht11 dhtInterface;

#define MEASURES_DELAY_BEFORE_MILLISECS 1000



Measurements::Measurements(
  MonitorLed* monitorLed, SerialLogger* serialLogger, 
  int dhtPin, int ambLightPin,
  int soilMoistPin1, int soilMoistPin2,
  int soilMoistPin3, int soilMoistPin4 )
{
  _monitorLed=monitorLed;
  _serialLogger=serialLogger;
  
  // copy passed pin values to internal values
  _dhtPin=dhtPin;
  _ambLightPin=ambLightPin;
  
  _soilMoistPin1=soilMoistPin1;
  _soilMoistPin2=soilMoistPin2;
  _soilMoistPin3=soilMoistPin3;
  _soilMoistPin4=soilMoistPin4;
}



void Measurements::init()
{
  
  // should initialize some of the pins ?!?  
  // not for output!
  // maybe just the WindStation needs something?
  
  
  _serialLogger->logFF(F("\nDHT-11 library version: "), F(DHT11LIB_VERSION));
}



//---------------------------------------------------------------------
// MEASURES
//---------------------------------------------------------------------


int Measurements::measure()
{
  // should do real measurements !!!
  
  measureAirTempAndMoistFromDht();
  
  measureSoilMoists();
  
  _pressure = 1.023;
  
  measureAmbLight();
  
  _windDir = 82.19;
  _windSpeed = 4.45;
  _windGusts = 7.65;
  
  _waterFlow = 0.21;
  
  // no errors
  return 0;
}



void Measurements::measureAirTempAndMoistFromDht()
{
  _serialLogger->logF(F("Measuring AirTemp and AirMoist from DHT-11..."));
  
  // try read data
  delay(MEASURES_DELAY_BEFORE_MILLISECS);
  int chk=dhtInterface.read(_dhtPin);
  
  // 
  switch (chk){
    
    case DHTLIB_OK:
                _serialLogger->logF(F("\t [ Digital Read ok ]")); 
                break;
                
    case DHTLIB_ERROR_CHECKSUM:
                _serialLogger->logF(F("\t Checksum error"));
                break;
                
    case DHTLIB_ERROR_TIMEOUT:
                _serialLogger->logF(F("\t Time out error"));
                break;
                
    default:
                _serialLogger->logF(F("\t Unknown error"));
                break;
  }
  
  // store and log data
  
  _airTemp = dhtInterface.temperature;
  _serialLogger->logFL(F("\t AirTemp: "), _airTemp);
  _airMoist = dhtInterface.humidity;
  _serialLogger->logFL(F("\t AirMoist: "), _airMoist);
  
}



void Measurements::measureSoilMoists()
{
  _serialLogger->logF(F("Measuring SoilMoists from analog sensors..."));
  
  /*
  the sensor value description
   - 0  ~300     dry soil
   - 300~700     humid soil
   - 700~950     in water
  */
  
  
  delay(MEASURES_DELAY_BEFORE_MILLISECS);
  _soilMoist1 = analogRead(_soilMoistPin1);
  _serialLogger->logFL(F("\t soilMoistPin1: "), _soilMoistPin1);
  _serialLogger->logFL(F("\t SoilMoist1: "), _soilMoist1);
  
  
  delay(MEASURES_DELAY_BEFORE_MILLISECS);
  _soilMoist2 = analogRead(_soilMoistPin2);
  _serialLogger->logFL(F("\t soilMoistPin2: "), _soilMoistPin2);
  _serialLogger->logFL(F("\t SoilMoist2: "), _soilMoist2);
  
  
  delay(MEASURES_DELAY_BEFORE_MILLISECS);
  _soilMoist3 = analogRead(_soilMoistPin3);
  _serialLogger->logFL(F("\t soilMoistPin3: "), _soilMoistPin3);
  _serialLogger->logFL(F("\t SoilMoist3: "), _soilMoist3);
  
  
  delay(MEASURES_DELAY_BEFORE_MILLISECS);
  _soilMoist4 = analogRead(_soilMoistPin4);
  _serialLogger->logFL(F("\t soilMoistPin4: "), _soilMoistPin4);
  _serialLogger->logFL(F("\t SoilMoist4: "), _soilMoist4);
  
}



void Measurements::measureAmbLight()
{
  _serialLogger->logF(F("Measuring AmbLight from analog sensor..."));
  
  _ambLight = analogRead(_ambLightPin);  
  _serialLogger->logFL(F("\t AmbientLight: "), _ambLight);
  
}





//---------------------------------------------------------------------
// GETTERS
//---------------------------------------------------------------------


float Measurements::getAirTemp()
{
  return _airTemp;
}
float Measurements::getAirMoist()
{
  return _airMoist;
}


float Measurements::getSoilMoist1()
{
  return _soilMoist1;
}
float Measurements::getSoilMoist2()
{
  return _soilMoist2;
}
float Measurements::getSoilMoist3()
{
  return _soilMoist3;
}
float Measurements::getSoilMoist4()
{
  return _soilMoist4;
}


float Measurements::getPressure()
{
  return _pressure;
}


float Measurements::getAmbLight()
{
  return _ambLight;
}



float Measurements::getWindDir()
{
  return _windDir;
}
float Measurements::getWindSpeed()
{
  return _windSpeed;
}
float Measurements::getWindGusts()
{
  return _windGusts;
}


float Measurements::getWaterFlow()
{
  return _waterFlow;
}


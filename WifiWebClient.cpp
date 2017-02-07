/*
  WifiWebClient.cpp - Library for Wifi Web Client
*/



#include "Arduino.h"

#include "WifiWebClient.h"

#include <WiFi.h>
#include <WiFiClient.h>



#define WIFI_WEB_CLIENT_MASKED_PASS "(MASKED_PASS)"


// Initialize the Wifi client library
WiFiClient client;


// server address:
char server[] = "192.168.1.75";



WifiWebClient::WifiWebClient(MonitorLed* monitorLed, SerialLogger* serialLogger, Measurements* measurements, Actuators* actuators)
{
  _monitorLed=monitorLed;
  _serialLogger=serialLogger;
  _measurements=measurements;
  _actuators=actuators;
}



void WifiWebClient::init(const char * ssid, const char * password)
{
  
  printWifiConst();

    
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) 
  {
    _serialLogger->logFF(F("\n\nSEVERE: WiFi shield not present. "), F("Will not continue, stopping here.")); 
    
    // blinkError() and don't continue (it's IOT !!!):
    while(true)
    {
      _monitorLed->blinkError();
    }
  }
  
  _serialLogger->logFS(F("\nWiFi Firmware Version : "), WiFi.firmwareVersion());
  
  _ssid = ssid;  
  _password = password;
  
  this->checkAndReconnect();
}



int WifiWebClient::checkAndReconnect()
{
  // assuming idle status WL_IDLE_STATUS
  int status=WiFi.status();
  
  // if not connected, attempt to connect to Wifi network:
  if (status != WL_CONNECTED) 
  {
    _serialLogger->logFS(F("\nNot (yet) connected to (or disconnected from) SSID: "), _ssid);
    
    _serialLogger->logFS(F("\nAttempting to connect to SSID: "), _ssid);
    _serialLogger->logFS(F("\t Connecting using password: "), WIFI_WEB_CLIENT_MASKED_PASS);
    
    // Connect to WPA/WPA2 network
    _monitorLed->on();
    status = WiFi.begin(const_cast<char*>(_ssid), _password);
    _monitorLed->off();
    
    _serialLogger->logFI(F("\t WiFi.begin() returned: "), status);

    // wait 1 second for connection:
    delay(1000);
       
    // you're connected now, so print out the status:
    this->printWifiStatus();
  }
  else
  {
    _serialLogger->logFS(F("\nAlready connected to SSID: "), _ssid);
  }
  
  return status;
}






void WifiWebClient::sendRq() 
{
  
  _serialLogger->logFL(F("\t sending Request at "), millis());
  
  _serialLogger->logFS(F("\t sending Request to "), server);
      
  // if there's a successful connection:
  if (client.connect(server, 8080)) 
  {
    
    // send the HTTP GET request:
    //client.println(F("GET /IrriguinoWeatherLogger0003/logger HTTP/1.1"));
    client.println(F("GET /IrriguinoWeatherLogger/logger HTTP/1.1"));
    
    // mandatory headers
    
    client.println(F("Host: any\nUser-Agent: any"));
    
    
    // measurements ones
    
    client.print(F("AirTemp: "));
    client.println(_measurements->getAirTemp(),DEC);
    
    client.print(F("AirMoist: "));
    client.println(_measurements->getAirMoist(),DEC);
    
    client.print(F("SoilMoist1: "));
    client.println(_measurements->getSoilMoist1(),DEC);
    
    client.print(F("SoilMoist2: "));
    client.println(_measurements->getSoilMoist2(),DEC);
    
    client.print(F("SoilMoist3: "));
    client.println(_measurements->getSoilMoist3(),DEC);
    
    client.print(F("SoilMoist4: "));
    client.println(_measurements->getSoilMoist4(),DEC);
    
    client.print(F("AmbLight: "));
    client.println(_measurements->getAmbLight(),DEC);
    
    
    // actuators ones
    
    client.print(F("RelayValue1: "));
    client.println(_actuators->getRelayValue1(),DEC);
    
    client.print(F("RelayValue2: "));
    client.println(_actuators->getRelayValue2(),DEC);
    
    client.print(F("RelayValue3: "));
    client.println(_actuators->getRelayValue3(),DEC);
    
    client.print(F("RelayValue4: "));
    client.println(_actuators->getRelayValue4(),DEC);
    
    
    // for closing connection
    
    client.println(F("Connection: close"));
    client.println();
    
  } 
  else 
  {
    
    // if you couldn't make a connection:
        
    _serialLogger->logFL(F("\nERROR : connection failed at "), millis());
    _serialLogger->logF(F("\t Sorry, will disconnect and go on for a new loop..."));
    client.stop();
  }
}



void WifiWebClient::readRs() 
{
  _serialLogger->logFL(F("\t reading Response at "), millis());
  
  char c = '\0';
  
  // finche' c'e' una connessione aperta con il server,
  // e non hai raggiunto il timeout per leggere la RS,
  // visto che e' il server che chiude la connessione quando ha finito,
  // in quanto nella RQ ho chiesto "connection: close" negli header:
  while (client.connected())//&& (millis() - lastConnectionTime > timeoutInterval)) 
  {
    
    // se ci sono dati da leggere sulla connessione, leggili 
    // e, ai fini di debug, stampali su seriale:
    while (client.available()) {
      
      c = client.read();
      
      // volendo qui puoi leggere i caratteri in righe, 
      // come fa(ceva, visto che l'hai buttato) l'evoluzione di questo:
      // https://www.arduino.cc/en/Tutorial/WiFiWebServer
      Serial.write(c);
    }
  }
  
  client.flush();
  client.stop();
}


void WifiWebClient::addHeaderF(String* rqBuffer, const char* name, float value)
{
  
  char numBuff[25] = "\0";
  
  rqBuffer->concat(name);
  
  rqBuffer->concat(": ");
  
  sprintf(numBuff, "%f", value);
  rqBuffer->concat(numBuff);
  
  rqBuffer->concat("\n");
  
  _serialLogger->logFS(F("\t\t added name: "), name);
  _serialLogger->logFS(F("\t\t added value: "), numBuff);
  
}




void WifiWebClient::printWifiStatus() 
{
  
  _serialLogger->logFL(F("\nWireless status at "), millis());
  
  // print the SSID of the network you're attached to:
  _serialLogger->logFI(F("\t WiFi.status(): "), WiFi.status());
  
  // print the SSID of the network you're attached to:
  _serialLogger->logFS(F("\t WiFi.SSID(): "), WiFi.SSID());


  // print your WiFi shield's IP address:
  char ipString[24];
  IPAddress ip = WiFi.localIP();
  sprintf(ipString, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  _serialLogger->logFS(F("\t WiFi.localIP(): "), ipString);

  ip = WiFi.subnetMask();
  sprintf(ipString, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  _serialLogger->logFS(F("\t WiFi.subnetMask(): "), ipString);
  
  ip = WiFi.gatewayIP();
  sprintf(ipString, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  _serialLogger->logFS(F("\t WiFi.gatewayIP(): "), ipString);
  

  // print the received signal strength:
  _serialLogger->logFL(F("\t WiFi.RSSI() (dBm): "), WiFi.RSSI());
}



void WifiWebClient::printWifiConst() {
  _serialLogger->logF(F("\nWIFI STATUSES from WifiWebClient.cpp"));
  
  _serialLogger->logFL(F("\t WL_NO_SHIELD = "), WL_NO_SHIELD);
  _serialLogger->logFL(F("\t WL_IDLE_STATUS = "), WL_IDLE_STATUS);
  _serialLogger->logFL(F("\t WL_NO_SSID_AVAIL = "), WL_NO_SSID_AVAIL);
  _serialLogger->logFL(F("\t WL_SCAN_COMPLETED = "), WL_SCAN_COMPLETED);
  _serialLogger->logFL(F("\t WL_CONNECTED = "), WL_CONNECTED);
  _serialLogger->logFL(F("\t WL_CONNECT_FAILED = "), WL_CONNECT_FAILED);
  _serialLogger->logFL(F("\t WL_CONNECTION_LOST = "), WL_CONNECTION_LOST);
  _serialLogger->logFL(F("\t WL_DISCONNECTED = "), WL_DISCONNECTED);
}


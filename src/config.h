#pragma once

#define null 0
//#define MXTNR_ESP_DEBUG
#include <Arduino.h>

class Config {
public:
  const unsigned int SERIAL_SPEED = 1000000;

  //
  // Wi-Fi, mDNS, OTA
  //

  const String WIFI_HOSTNAME = "esp-flower-keeper";

  const String WIFI_SSID = "grove";
  const String WIFI_PASSW = "0My%Secur";

  //
  // IoT
  //

  // TODO: use EEPROM

  //
  // Pins
  //

//  const unsigned int PIN_PIR = 12;  // IN; movement sensor

};

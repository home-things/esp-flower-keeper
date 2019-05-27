#pragma once

#define null 0
//#define MXTNR_ESP_DEBUG

class Config {
public:
  const unsigned int SERIAL_SPEED = 1000000;

  //
  // Wi-Fi, mDNS, OTA
  //

  const String WIFI_HOSTNAME = "esp-reminder";

  const String WIFI_SSID = "RocketInternet";
  const String WIFI_PASSW = "The_Internet$2k17";

  //
  // IoT
  //

  // TODO: use EEPROM

  //
  // Pins
  //

//  const unsigned int PIN_PIR = 12;  // IN; movement sensor

};

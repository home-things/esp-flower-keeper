#pragma once

/**
   @file OTA-mDNS-SPIFFS.ino
   update On The Air

   @author Pascal Gollor (http://www.pgollor.de/cms/)
   @date 2015-09-18

   changelog:
   2015-10-22:
   - Use new ArduinoOTA library.
   - loadConfig function can handle different line endings
   - remove mDNS studd. ArduinoOTA handle it.

*/

// includes

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// #include <Scheduler.h>

void ota_setup ();
void ota_loop ();

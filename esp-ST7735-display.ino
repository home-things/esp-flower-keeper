/*
 An example analogue clock using a TFT LCD screen to show the time
 use of some of the drawing commands with the ST7735 library.
 For a more accurate clock, it would be better to use the RTClib library.
 But this is just a demo. 
 Uses compile time to set the time so a reset will start with the compile time again
 
 Gilchrist 6/2/2014 1.0
 Updated by Bodmer
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
//#include <WiFi.h>
//#include <NTPClient.h>
#include <time.h>

#include <SPI.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <TFT_Drivers/ST7735_Defines.h>
// TFT_eSPI/User_Setup.h patched!
#include "rtc.h" // rtc for esp -- https://esp8266.ru/forum/threads/esp8266-vremja-ds3231-ds1307-chtenie-ustanovka.695/

#include "consts.h"
#include "config.h"
#include "ntp.h"

Config conf;
WiFiUDP ntpUDP;


// By default 'pool.ntp.org' is used with 7 days update interval and no offset
//NTPClient timeClient(ntpUDP, "pool.ntp.org", 3, 1000 * 3600 * 7);


// Stock font and GFXFF reference handle
#define GFXFF 1
//#define FF18 &FreeSans12pt7b
//#define CF_RT24 &Roboto_Thin_24
#define OVERSCAN_Y -24

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

const unsigned line_h = 24; // height, px
unsigned text_y = OVERSCAN_Y + 12;
//unsigned long startedAt = null; // s since 1970 aka epoch

void center_text (String text, unsigned color) {
  const unsigned font = 4;
  tft.setTextColor(color, TFT_BLACK);
  // landscape mode
  tft.drawCentreString(text, TFT_HEIGHT / 2, text_y += line_h, font);
  tft.setTextColor(TFT_LIGHTGREY, TFT_WHITE);
}

void margin (unsigned margin) {
  text_y += margin;
}


static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}


void setupWifi () {
  WiFi.mode(WIFI_STA);
  WiFi.begin(conf.WIFI_SSID.c_str(), conf.WIFI_PASSW.c_str());
  // Serial.println("");

   Serial.println("Wait for connection");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
   Serial.println("");
   Serial.print("Connected to ");
   Serial.println(conf.WIFI_SSID);
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());

}
//
//// @returns epoch secs
//unsigned long getTimestamp () {
//  return startedAt + millis() / 1000;
//}
//
//unsigned get_hour (unsigned long ts)
//{
//  return (getTimestamp() / 3600) % 86400L;
//}
//unsigned get_min (unsigned long ts)
//{
//  return (getTimestamp() / 60) % 3600;
//}
//unsigned get_sec (unsigned long ts)
//{
//  return (getTimestamp()) % 60;
//}

void setup(void) {
  Serial.begin(115200);
  setupWifi();
  // todo: skip ntp, if no internet or 2 retries are failed
  time_t epoch = setupNtpClient();
  Serial.print("NTP time: "); Serial.println(epoch, DEC);

//  timeClient.begin(); // NTP
//  Serial.print("NTP time: ");
//  Serial.println(timeClient.getFormattedTime());
//  time_t epoch = timeClient.getEpochTime();
  Serial.print("NTP time day: ");
  if (epoch) Serial.println(gmtime(&epoch)->tm_mday);

  rtc_setup();
  tm *t = rtc_time_get();
  // todo: если ntp не сработал, использовать rtc
  // todo: если ntp сработал, сохранить в rtc
  Serial.print("RTC: ");
  Serial.println(rtc_time_str());
//  setTime(t->tm_hour, t->tm_min, t->tm_sec, t->tm_mday, t->tm_mon, t->tm_year);

  // set rtc time from compile time
  // uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time
  // rtc_time_set(hh, mm, ss, 0, 27, 5, 2019);
  if (epoch) {
    rtc_time_set(gmtime(&epoch));
    Serial.print("Corrected RTC: ");
    Serial.println(rtc_time_str());
  }

  tft.init();
  tft.setRotation(45); // landscape mode
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);  // Adding a black background colour erases previous text automatically
  
  // Draw clock face
  //tft.fillCircle(64, 64, 61, TFT_BLUE);
  //tft.fillCircle(64, 64, 57, TFT_BLACK);


  // tft.fillCircle(65, 65, 3, TFT_RED);

  // Draw text at position 64,125 using fonts 4
  // Only font numbers 2,4,6,7 are valid. Font 6 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 : . a p m
  // Font 7 is a 7 segment font and only contains characters [space] 0 1 2 3 4 5 6 7 8 9 : .

//  tft.drawCentreString("SUCULENTA",64,0,GFXFF);
//  tft.setFreeFont(CF_RT24);                 // Select the font

  center_text("SUCULENTA", tft.color565(76, 175, 80)); // light green
  center_text("water!", tft.color565(3, 169, 244)); // light blue

  margin(12);
  center_text("LEMON MINT", tft.color565(76, 175, 80)); // light green
  center_text("ok now", TFT_DARKGREY);

}

void loop() {
//    timeClient.update(); // ntp

  // tft.drawLine(ohx, ohy, 65, 65, TFT_BLACK);
  // tft.fillCircle(65, 65, 3, TFT_RED);
}

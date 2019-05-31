#include <Arduino.h>

//
// LCD
#include <SPI.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <TFT_Drivers/ST7735_Defines.h>
// TFT_eSPI/User_Setup.h patched!

//
// time
#include <time.h>
#include "rtc.h" // rtc for esp -- https://esp8266.ru/forum/threads/esp8266-vremja-ds3231-ds1307-chtenie-ustanovka.695/
#include "ntp.h"

#include "lib.h"
#include "consts.h"
#include "config.h"

#include "ota.h"

Config conf;
unsigned long startedAt = null; // s since 1970 aka epoch
bool isNtp = false;
int prev_day = -1;
// WiFiUDP ntpUDP;


// By default 'pool.ntp.org' is used with 7 days update interval and no offset
//NTPClient timeClient(ntpUDP, "pool.ntp.org", 3, 1000 * 3600 * 7);


// Stock font and GFXFF reference handle
#define GFXFF 1
//#define FF18 &FreeSans12pt7b
//#define CF_RT24 &Roboto_Thin_24

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

const unsigned line_h = 16; // height, px
unsigned text_y;
//unsigned long startedAt = null; // s since 1970 aka epoch

void start_text()
{
  text_y = -16;
}

void center_text (String text, unsigned color) {
  const unsigned font = 2;
  tft.setTextColor(color, TFT_BLACK);
  // landscape mode
  tft.drawCentreString(text, TFT_HEIGHT / 2, text_y += line_h, font);
  tft.setTextColor(TFT_LIGHTGREY, TFT_WHITE);
}

void print_status (const String &text, unsigned color)
{
  tft.setTextColor(color, TFT_BLACK);
  tft.drawRightString(text, TFT_HEIGHT - 2, TFT_WIDTH - 10, 1);
}
void print_status (const String &text)
{
  print_status(text, TFT_BLACK);
}


void margin (unsigned margin) {
  text_y += margin;
}


void water_please ()
{
  center_text("water!", tft.color565(3, 169, 244)); // light blue
}
void okay_now ()
{
  center_text("ok now", TFT_DARKGREY);
}


static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}


void setup_wifi () {
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

// @returns epoch secs
time_t get_timestamp () {
 return startedAt + millis() / 1000;
}

tm *get_time ()
{
  time_t ts = get_timestamp();
  return gmtime(&ts);
}

// Day number since 1970
unsigned get_day (time_t ts)
{
 return get_timestamp() / 86400L;
}
//unsigned get_hour (time_t ts)
//{
//  return (get_timestamp() / 3600) % 86400L;
//}
//unsigned get_min (time_t ts)
//{
//  return (get_timestamp() / 60) % 3600;
//}
//unsigned get_sec (time_t ts)
//{
//  return (get_timestamp()) % 60;
//}

void render ()
{
  unsigned INTERVAL;
  const unsigned day = get_day(get_timestamp());
  if (day == prev_day) return;

  start_text();
  center_text("SUCULENTA", tft.color565(76, 175, 80)); // light green
  const unsigned OFFSET = -3; // 18044th day must be 0 (=water_please)
  INTERVAL = 5;
  (day + OFFSET) % INTERVAL ? okay_now() : water_please();


  margin(8);
  center_text("LEMON MINT", tft.color565(76, 175, 80)); // light green
  INTERVAL = 2;
  (day + OFFSET) % INTERVAL ? okay_now() : water_please();


  margin(8);
  center_text("THUJA", tft.color565(76, 175, 80)); // light green
  INTERVAL = 3;
  (day + OFFSET) % INTERVAL ? okay_now() : water_please();

  prev_day = day;
}

void display_setup ()
{
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

  render();
}

void setup_time_rtc ()
{
  rtc_setup();
  tm *t = rtc_time_get();
  startedAt = mktime(t);
}

void setup_time_ntp ()
{

  Serial.print("RTC: ");
  Serial.println(rtc_time_str());

  time_t epoch = setupNtpClient();

//  timeClient.begin(); // NTP
//  Serial.print("NTP time: ");
//  Serial.println(timeClient.getFormattedTime());
//  time_t epoch = timeClient.getEpochTime();


//  setTime(t->tm_hour, t->tm_min, t->tm_sec, t->tm_mday, t->tm_mon, t->tm_year);

  // set rtc time from compile time
  // uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time
  // rtc_time_set(hh, mm, ss, 0, 27, 5, 2019);

  // если ntp сработал, сохранить в rtc
  if (epoch) {
    isNtp = true;
    Serial.print("NTP time: "); Serial.println(epoch, DEC);

    Serial.print("NTP time day: ");
    Serial.println(gmtime(&epoch)->tm_mday);

    startedAt = epoch;

    rtc_time_set(gmtime(&epoch));
    Serial.print("Corrected RTC: "); Serial.println(rtc_time_str());
  } else {
    isNtp = false;
    Serial.print("From RTC: "); Serial.println(startedAt);
  }

}



// //
// //
// //
// //
// //



void setup(void) {
  Serial.begin(115200);

  setup_time_rtc(); // quick
  print_status("rtc_t: ok", TFT_DARKGREY);

  display_setup();
  print_status("disp: ok", TFT_DARKGREY);

  setup_wifi();
  print_status("wifi: ok", TFT_CYAN);

  ota_setup();
  print_status(" ota: ok", TFT_WHITE);

  setup_time_ntp(); // slow
  print_status("time: ok", TFT_WHITE);

  Serial.println("inited");
}




void loop() {
  render();

  ota_loop();

  tm *t = get_time();

  // right bottom corner clock
  tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
  // print_status(formatDigit(t->tm_hour, 2) + ":" + formatDigit(t->tm_min, 2));
  tft.drawRightString(time_str(t) + " " + (isNtp ? "ntp" : "rtc"), TFT_HEIGHT - 2, TFT_WIDTH - 10, 1);
}

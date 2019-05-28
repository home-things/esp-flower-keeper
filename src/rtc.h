// https://esp8266.ru/forum/threads/esp8266-vremja-ds3231-ds1307-chtenie-ustanovka.695/

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <time.h>
#include <sys/time.h>
#define DS3231_SCL 4
#define DS3231_SDA 5

void rtc_setup();
void rtc_getDateDS3231();
void rtc_setDateDS3231();
void rtc_time_set();
void rtc_time_set(tm * t);
void rtc_time_set(int _hour, int _minute, int _second, int _dayOfWeek, int _dayOfMonth, int _month, int _year);
String rtc_time_str();
tm *rtc_time_get();

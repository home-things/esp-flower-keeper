// https://esp8266.ru/forum/threads/esp8266-vremja-ds3231-ds1307-chtenie-ustanovka.695/
#include "rtc.h"

//byte second, minute, hour, dayOfWeek, dayOfMonth, month, year, temperature;
int temperature;
tm t;

int bcdToDec(int val);
int decToBcd(int val);
String formatDigit(int i, int len);

 /*
struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};
*/
void rtc_setup() {
  Wire.begin(DS3231_SCL, DS3231_SDA);
  rtc_getDateDS3231();
}

//void loop() {
// 
//  if (Serial.available() > 0) {
//    String str = Serial.readStringUntil('\n');
//    if (str == "get") {
//      getDateDS3231(); // update time variables
//      String s = rtc_getTimeStr();    
//      Serial.println("-----------show time-----------");
//      Serial.println(s);
//    }
//    if (str == "set") {
//      time_set();
//    }
//  }
//}

void rtc_getDateDS3231()
{
  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 7);
  t.tm_sec   = bcdToDec(Wire.read() & 0x7f);
  t.tm_min   = bcdToDec(Wire.read());
  t.tm_hour  = bcdToDec(Wire.read() & 0x3f);
  t.tm_wday  = bcdToDec(Wire.read()); // day of week
  t.tm_mday  = bcdToDec(Wire.read()); // day of month
  t.tm_mon   = bcdToDec(Wire.read()); // month
  t.tm_year  = bcdToDec(Wire.read());
  Serial.print("tm_sec: ");
  Serial.println(bcdToDec(Wire.read() & 0x7f));
  Serial.println(t.tm_sec);
 
//--------------------------------------------
    Wire.beginTransmission(0x68);
    Wire.write(0x11);
    Wire.endTransmission();
    Wire.requestFrom(0x68, 2);
    temperature = (Wire.read() & 0b1111111) + ( (Wire.read() >> 6) * 0.25 );
}
void rtc_setDateDS3231()
{
  // Use of (byte) type casting and ascii math to achieve result.
  /*second = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  minute = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  hour  = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  dayOfWeek = (byte) (Serial.read() - 48);
  dayOfMonth = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  month = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  year= (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));*/
  Wire.beginTransmission(0x68);
  Wire.write(int(0x00));
  Wire.write(decToBcd(t.tm_sec));  // 0 to bit 7 starts the clock
  Wire.write(decToBcd(t.tm_min));
  Wire.write(decToBcd(t.tm_hour));    // If you want 12 hour am/pm you need to set
  // bit 6 (also need to change readDateDs1307)
  Wire.write(decToBcd(t.tm_wday));
  Wire.write(decToBcd(t.tm_mday));
  Wire.write(decToBcd(t.tm_mon));
  Wire.write(decToBcd(t.tm_year));
  Wire.endTransmission();
}

// user input
void rtc_time_set()
{
  Serial.println("Enter Year 2 last digits");
  while (Serial.available() <= 0);
  t.tm_year = Serial.parseInt();
  Serial.println(t.tm_year);
  Serial.println("Month");
  while (Serial.available() <= 0);
  t.tm_mon = Serial.parseInt();
  Serial.println(t.tm_mon);
  Serial.println("dayOfMonth");
  while (Serial.available() <= 0);
  t.tm_mday = Serial.parseInt();
  Serial.println(t.tm_mday);
  Serial.println("Day of Week");
  while (Serial.available() <= 0);
  t.tm_wday = Serial.parseInt();
  Serial.println(t.tm_wday);
  Serial.println("Hour");
  while (Serial.available() <= 0);
  t.tm_hour = Serial.parseInt();
  Serial.println(t.tm_hour);
  Serial.println("Minute");
  while (Serial.available() <= 0);
  t.tm_min = Serial.parseInt();
  Serial.println(t.tm_min);
  Serial.println("Second");
  while (Serial.available() <= 0);
  t.tm_sec = Serial.parseInt();
  Serial.println(t.tm_sec);
 
  rtc_setDateDS3231();
  Serial.println("Time recvd OK");
}

void rtc_time_set(int _hour, int _minute, int _second, int _dayOfWeek, int _dayOfMonth, int _month, int _year)
{
  t.tm_hour = _hour, t.tm_min = _minute, t.tm_sec = _second;
  t.tm_wday = _dayOfWeek;
  t.tm_mday = _dayOfMonth, t.tm_mon = _month, t.tm_year = _year;

  rtc_setDateDS3231();
}

tm *rtc_time_get()
{
  return &t; // todo: correct pointer owning
}

void rtc_time_set(tm * _t)
{
  t = *_t; // todo: correct pointer owning
  rtc_setDateDS3231();
}


//
// @private
//

// Convert normal decimal numbers to binary coded decimal
int decToBcd(int val)
{
  return ( (val / 10 * 16) + (val % 10) );
}
// Convert binary coded decimal to normal decimal numbers
int bcdToDec(int val)
{
  return ( (val / 16 * 10) + (val % 16) );
}
String rtc_time_str()
{
  String str = String(t.tm_mday) + "." + formatDigit(t.tm_mon, 2) + "." + formatDigit(t.tm_year, 4) + " (week: " + t.tm_wday + ") " + formatDigit(t.tm_hour, 2) + ":" + formatDigit(t.tm_min, 2) + ":" + formatDigit(t.tm_sec, 2) + ". temperature: " + temperature;
  return str;
}
String formatDigit(int i, int len)
{
  String s = String(i);
  while (s.length() < len) {
    s = "0" + s;
  }
  return (s);
}

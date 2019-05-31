#include <Arduino.h>

String formatDigit(int i, int len)
{
  String s = String(i);
  while (s.length() < len) {
    s = "0" + s;
  }
  return (s);
}
String time_str(tm *t)
{
  return String(t->tm_mday) + "." + formatDigit(t->tm_mon + 1, 2)
    + " " + formatDigit(t->tm_hour, 2) + ":" + formatDigit(t->tm_min, 2);
}

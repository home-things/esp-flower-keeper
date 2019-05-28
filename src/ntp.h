#pragma once

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "consts.h"

// local port to listen for UDP packets
#define ntpLocalPort 2390

#define ntpServerName "time.nist.gov"

// NTP time stamp is in the first 48 bytes of the message
#define NTP_PACKET_SIZE 48

// +3h, in secs
#define corrTZ (3 * 3600)

unsigned long getNtpTime ();

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address);

unsigned long setupNtpClient ();

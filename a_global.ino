/********************* Includes and variabled *******************************/
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

long lastMsg = 0; // Time of last notification
long millis_held; // Time the button was held
long cooldownEndTime; // Time the doorbell sound relay should be turned on
char msg[50];
unsigned long firstTime; // Start time button pressed
byte previousButtonStatus = HIGH; // assume the button isn't pressed during startup
bool telnetClientConnected = false; // assume there is no telnet client during startup
bool bellStatus; // Variable to set the current status of the bell sound relay

int debounceDuration;
int bellOffDuration;
int noNotificationsDuration;

const int setPin = 12;
const int unsetPin = 14;
const int sigPin = 13;
const int onOffLedPin = 2;
const int signalLedPin = 0;

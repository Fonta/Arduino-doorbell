///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// HELPER FUNCTIONS //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void loadTimings()
{
  debounceDuration = string_convert(read_EEPROM(30, 0));
  bellOffDuration = string_convert(read_EEPROM(30, 100));
  noNotificationsDuration = string_convert(read_EEPROM(30, 200));

  //fallback to defaults when value = 0
  if ( isnan(debounceDuration) )
  {
    debounceDuration = 50;
    printMsg("Using default value of 50 for the debounce duration");
  }
  if ( isnan(bellOffDuration) )
  {
    bellOffDuration = 5000;
    printMsg("Using default value of 5000 for the bell off duration");
  }
  // if (!noNotificationsDuration > 0)
  if ( isnan(bellOffDuration) )
  {
    noNotificationsDuration = 10000;
    printMsg("Using default value of 10000 for the no notifications duration");
  }

  char charDebounceDuration[6];
  itoa(debounceDuration, charDebounceDuration, 10);
  mqttClient.publish(dbSetDebounceDuration, charDebounceDuration);

  char charBellOffDuration[6];
  itoa(bellOffDuration, charBellOffDuration, 10);
  mqttClient.publish(dbSetbellOffDuration, charBellOffDuration);

  char charNoNotificationsDuration[6];
  itoa(noNotificationsDuration, charNoNotificationsDuration, 10);
  mqttClient.publish(dbSetnoNotificationsDuration, charNoNotificationsDuration);
}

// Helper function to print a msg in the serial monitor and also to the telnet client if connected.
void printMsg(String theMsg) {
  if (telnetClient && telnetClient.connected()) {  // send data to Client
    telnetClient.println(theMsg);
  }
  Serial.println(theMsg);
}

// Just a simple helper function to blink a led in various patterns
void ledblink(int times, int lengthms, int pinnum) {
  for (int x = 0; x < times; x++) {
    digitalWrite(pinnum, LOW);
    delay (lengthms);
    digitalWrite(pinnum, HIGH);
    delay(lengthms);
  }
}

void turnBellOn() {
  digitalWrite(setPin, HIGH);
  delay(10);
  digitalWrite(setPin, LOW);
  digitalWrite(onOffLedPin, LOW);
  bellStatus = true;
}

void turnBellOff() {
  digitalWrite(unsetPin, HIGH);
  delay(10);
  digitalWrite(unsetPin, LOW);
  digitalWrite(onOffLedPin, HIGH);
  bellStatus = false;
}

//creates int out of a string for your IP address
int string_convert(String convert) {
  int number;
  if (convert.toInt()) {
    number = convert.toInt();
    return number;
  }
}

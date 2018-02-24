/******************************** Set it all up ***************************************/
void setup() {
  Serial.begin(115200);
  delay(10);

  // set the pin modes
  pinMode(setPin, OUTPUT);
  pinMode(unsetPin, OUTPUT);
  pinMode(sigPin, INPUT_PULLUP);
  pinMode(onOffLedPin, OUTPUT);
  pinMode(signalLedPin, OUTPUT);

  // put leds off by default
  digitalWrite(onOffLedPin, HIGH);
  digitalWrite(signalLedPin, HIGH);

  // connect to wifi and mqtt server
  setup_wifi();
  setup_telnetserver();
  setup_mqttclient();
  setup_webserver();

  mqttconnect();

  setup_EEPROM();
  loadTimings();
}

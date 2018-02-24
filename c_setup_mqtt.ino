/************************* MQTT Setup **********************************/
#define MQTT_SERVER                  "192.168.1.19"
#define MQTT_SERVERPORT              1883     // use 8883 for SSL
// #define MQTT_USERNAME                "MQTTUSERNAME"
// #define MQTT_PASSWORD                "MQTTPASSWORD"
#define dbState                      "doorbell/state"
#define dbPressed                    "doorbell/pressed"
#define dbSet                        "doorbell/set"
#define dbSetDebounceDuration        "doorbell/set/debounceDuration"
#define dbSetbellOffDuration         "doorbell/set/bellOffDuration"
#define dbSetnoNotificationsDuration "doorbell/set/noNotificationsDuration"

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup_mqttclient() {
  mqttClient.setServer(MQTT_SERVER, MQTT_SERVERPORT);
  mqttClient.setCallback(callbackmqtt);
}

void mqttconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    printMsg("Attempting MQTT connection... ");

    long randomid = random(5000);
    String randomid_str = String(randomid);
    String clientId = "doorbell-" + randomid_str;

    // Attempt to connect
    if (mqttClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
      printMsg("Connection to MQTT broker successfull!");
      mqttClient.subscribe(dbSet);
      mqttClient.subscribe(dbState);
      mqttClient.subscribe(dbSetDebounceDuration);
      mqttClient.subscribe(dbSetbellOffDuration);
      mqttClient.subscribe(dbSetnoNotificationsDuration);
    } else {
      printMsg("Failed to connect to MQTT broker, rc=" + mqttClient.state());
      printMsg("Will try again in 5 seconds.....");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callbackmqtt(char* topic, byte* payload, unsigned int length) {
  String topicString = topic;
  String payloadmsg = ""; // start with empty payloadmsg

  for (int i = 0; i < length; i++) {
    payloadmsg += (char)payload[i];
  }

  if ( strcmp(topic, dbSet) == 0 ) {
    printMsg("On/Off command received [" + topicString + "] " + payloadmsg);

    // flash the signal led
    ledblink(1, 100, signalLedPin);

    if (payloadmsg == "on") {
      printMsg("Turning doorbell on");

      turnBellOn();

      printMsg("Returning doorbell status to mqtt for confirmation...");
      if (! mqttClient.publish(dbState, "on", true) ) {
        printMsg("FAILED!");
        mqttconnect();
      } else {
        printMsg("OK!");
      }
    } else if (payloadmsg == "off") {
      printMsg("Turning doorbell off");

      turnBellOff();

      printMsg("Returning doorbell status to mqtt for confirmation...");
      if (! mqttClient.publish(dbState, "off", true) ) {
        printMsg("FAILED!");
        mqttconnect();
      } else {
        printMsg("OK!");
      }
    } else {
      printMsg("I have no idea what to do...");
    }
  }

  if ( strcmp(topic, dbSetDebounceDuration) == 0 && debounceDuration != string_convert(payloadmsg) ) {
    debounceDuration = string_convert(payloadmsg);
    payloadmsg += ";";
    write_EEPROM(payloadmsg, 0);
    EEPROM.commit();
    printMsg("Saved new debounce duration value from MQTT!");
  }

  if ( strcmp(topic, dbSetbellOffDuration) == 0 && bellOffDuration != string_convert(payloadmsg) ) {
    bellOffDuration = string_convert(payloadmsg);
    payloadmsg += ";";
    write_EEPROM(payloadmsg, 100);
    EEPROM.commit();
    printMsg("Saved new bell off duration value from MQTT!");
  }

  if ( strcmp(topic, dbSetnoNotificationsDuration) == 0 && noNotificationsDuration != string_convert(payloadmsg) ) {
    noNotificationsDuration = string_convert(payloadmsg);
    payloadmsg += ";";
    write_EEPROM(payloadmsg, 200);
    EEPROM.commit();
    printMsg("Saved new no notification duration value from MQTT!");
  }
}

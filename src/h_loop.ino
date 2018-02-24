/********************************* The loop *****************************************/
void loop() {
  telnetclient_check();

  // check the mqtt connection
  if (!mqttClient.connected()) {
    mqttconnect();
  }
  mqttClient.loop();

  doorbell_check();

  webserver.handleClient();
}

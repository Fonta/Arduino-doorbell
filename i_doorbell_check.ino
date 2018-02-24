/********************************* The doorbell check *************************************/
void doorbell_check() {
  int currentButtonStatus = digitalRead(sigPin);

  if (currentButtonStatus == LOW && previousButtonStatus == HIGH && (millis() - firstTime) > 200 ) {
    // button is pressed - save the time when this was
    firstTime = millis();
  }

  if (currentButtonStatus == HIGH && previousButtonStatus == LOW) {
    millis_held = (millis() - firstTime);

    if (millis_held > debounceDuration) { //debounce time
      //turn the bell off if it's currently on for a nice cooldown
      if (bellStatus) {
        turnBellOff();
        cooldownEndTime = millis() + bellOffDuration; // Set the time the cooldown ends
      }

      long now = millis();

      if (now - lastMsg > noNotificationsDuration) { //only sent a notification every 10 seconds
        // send a message to MQTT
        printMsg("Doorbell pressed for " + String(millis_held) + "ms! Sending signal to MQTT...");
        if (mqttClient.publish(dbPressed, "ding")) {
          printMsg("OK!");
        } else {
          printMsg("Failed!");
          // if it failed try to connect to mqtt
          mqttconnect();
        }

        // flash the signal led
        ledblink(3, 100, signalLedPin);

        lastMsg = now;
      } else {
        printMsg("Button was pressed during cooldown. Not gonna sent a notification.");
      }

    } else {
      printMsg("The received signal was only " + String(millis_held) + "ms. Won't do a thing.");
    }
  }

  if (millis() > cooldownEndTime && cooldownEndTime > 0) {
    turnBellOn();
    cooldownEndTime = 0; //Reset to 0 so it won't be turned on again.
  }
  previousButtonStatus = currentButtonStatus;
}

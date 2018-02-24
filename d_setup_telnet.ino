/************************* Telnet setup ********************************/
// Create a telnet server and spawn a client
WiFiServer telnetServer(23);
WiFiClient telnetClient;

void setup_telnetserver() {
  telnetServer.begin();
  telnetServer.setNoDelay(true);
}

void telnetclient_check() {
  // look for telnet Client connect trial
  if (!telnetClient) {
    telnetClient = telnetServer.available();
  } else {
    if (!telnetClient.connected()) {
      if (telnetClientConnected) {
        closeTelnetConnection();
      }
    } else {
      if (!telnetClientConnected) {
        printMsg("Telnet Client Connected!");
        telnetClient.flush();
        telnetClientConnected = true;
      }
    }
    // Empty the buffer for a gracefull disconnect.
    while (telnetClient.available()) { // get data from Client
      telnetClient.read();
    }
  }
}

void closeTelnetConnection() {
  telnetClientConnected = false;
  telnetClient.stop();
  printMsg("Telnet Client Disconnected!");
}

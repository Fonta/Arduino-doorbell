/*************************** WiFi Setup ***********************************/
#define WLAN_HOSTNAME   "doorbell"
// #define WLAN_SSID       "SSID"
// #define WLAN_PASS       "WIFIPASSWORD"
#define NONE_SLEEP_T    "NONE_SLEEP_T"

void setup_wifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());

  MDNS.begin(WLAN_HOSTNAME);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

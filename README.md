# Arduino-doorbell
After downloading, (uncomment and) change the following:

in b_setup_wifi:
```cpp
#define WLAN_HOSTNAME   "doorbell"
// #define WLAN_SSID      "SSID"
// #define WLAN_PASS      "WIFIPASSWORD"
```
in c_setup_mqtt:
```cpp
#define MQTT_SERVER       "192.168.1.19"
#define MQTT_SERVERPORT   1883     // use 8883 for SSL
// #define MQTT_USERNAME  "MQTTUSERNAME"
// #define MQTT_PASSWORD  "MQTTPASSWORD"
```

in e_stup_webserver:
```cpp
// const char* update_username = "USERNAME";
// const char* update_password = "PASSWORD";
```

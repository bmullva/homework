#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>


// INITIALIZE DEVICE PARTICULAR CONSTANTS & VARIABLES
String type = "mom switch";
String ver = "6.1";

const int d_pin [3] = {14, 12, 13};
const char* d_pin_name [3] = {"p14", "p12", "p13"};
int d_pin_reading [3] = {HIGH, HIGH, HIGH};
int d_pin_n1_reading [3] = {HIGH, HIGH, HIGH};
char* d_pin_reading2state(int n) {
  if(n == 0) {return "on";}
  else {return "off";} 
  }
char* timer [3] = {"off","off","off"};
unsigned long startMillis [3] = {0,0,0};
int clk [3] = {0,0,0};
char* hld [3] = {"off","off","off"};


//***************************
//***    DO NOT TOUCH     ***
//***************************

// INITIALIZE GLOBAL MODULES
WiFiClient espClient;
PubSubClient client(espClient);
AsyncWebServer server(80);

// EEPROM Definitions
const int ip1_addr = 185;
const int ip2_addr = 186;
const int ip3_addr = 187;
const int ip4_addr = 188;
const int network_id_length_addr = 189; // number len of network_id, 2 <= len <= 32
const int network_id_addr = 190; //ASCII 32 byte (190-221) unique ID for network_id 
const int device_id_addr = 222; //ASCII 8 digit (222-229) unique ID for this device
const int setting_addr = 230; //ASCII 'a','n', or 'c'  a-setup, b-get mqtt ip, c-normal
const int password_length_addr = 231; // number len of network password, 8 <= len <= 63
const int password_addr = 240; //ASCII n digit network password (240-(240+len) 8<=len<=63 

//GLOBAL VARIABLES
// ip# are defined below
char mqtt_ip[16] {};
int network_id_length {};
char network_id[33] {};
char device_id[9] {};
char setting {};
int password_length {};
char password[64] {};

//SERVER CONSTANTS & VARIABLES
#ifndef APSSID
#define APSSID "newModule"
#define APPSK  "newModule"
#endif
const char *ssid_s = APSSID;
const char *password_s = APPSK;



void setup() { 
  Serial.begin(115200);
  EEPROM.begin(512);
  delay(1000);

  for(int i=0;i<8;i++) {device_id[i] = char(EEPROM.read(device_id_addr+i));}
  setting = char(EEPROM.read(setting_addr));
  
  if (setting=='s') {
    WiFi.softAP(ssid_s, password_s);
    def_server();
    }

  if (setting=='n') {
    int p1 = EEPROM.read(ip1_addr);
    int p2 = EEPROM.read(ip2_addr);
    int p3 = EEPROM.read(ip3_addr);
    int p4 = EEPROM.read(ip4_addr);
    String ip1 = String(p1);
    String ip2 = String(p2);
    String ip3 = String(p3);
    String ip4 = String(p4);
    String ip = ip1 + "." + ip2 + "." + ip3 + "." + ip4;
    int ip_len = ip.length()+1;
    ip.toCharArray(mqtt_ip, ip_len);  
    network_id_length = EEPROM.read(network_id_length_addr);
    for(int i=0;i<network_id_length;i++) {network_id[i] = char(EEPROM.read(network_id_addr+i));}
    setting = char(EEPROM.read(setting_addr));
    password_length = EEPROM.read(password_length_addr);
    for(int i=0;i<password_length;i++) {password[i] = char(EEPROM.read(password_addr+i));}
    print_vars();
    setup_wifi();
    client.setServer(mqtt_ip, 1883);
    client.setCallback(callback);
    ArduinoOTA.setHostname(device_id);
    ArduinoOTA.begin();
    
    initialize_pins();
  }

}


// DO NOT TOUCH - Ensures ESP is connected & sends messages to topics
void loop() {
  
  if (setting == 'n') {
  ArduinoOTA.handle();
  
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop()) {
    WiFi.mode(WIFI_STA);
    client.connect(device_id);
  }

  send_messages();
  
  delay(50);
  }
} 

#ifndef EZAMA_H
#define EZAMA_H


//Initialize standard imports
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>


//SERVER CONSTANTS & VARIABLES - MAYBE THIS GOES OUTSIDE OF THIS indef?
//#ifndef APSSID
#define APSSID "newModule"
#define APPSK  "newModule"
//#endif
const char *ssid_s = APSSID;
const char *password_s = APPSK;


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
const int setting_addr = 230; //ASCII 'n', or 's' s-setup, n-normal
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



//STANDARD SETUP CODE
void ezama_setup() {
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
}



//STANDARD LOOP CODE
ezama_loop() {
  if (setting == 'n') {
  ArduinoOTA.handle();
  
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop()) {
    WiFi.mode(WIFI_STA);
    client.connect(device_id);
  }
}



//FOR DEBUGGING
void print_vars() {
  Serial.print("mqtt_ip: ");
  Serial.println(mqtt_ip);
  Serial.print("device_id: ");
  Serial.println(device_id);
  Serial.print("setting: ");
  Serial.println(setting);
  Serial.print("password_length: ");
  Serial.println(password_length);
  Serial.print("network_id: ");
  Serial.println(network_id);
  Serial.print("password: ");
  Serial.println(password);
}


// SETUP WIFI
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(network_id);
  WiFi.mode(WIFI_STA);
  WiFi.begin(network_id, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 240) {  //try for 240 half-seconds is two minutes
      EEPROM.write(setting_addr, 's');
      EEPROM.commit();
      ESP.restart();
    }
    i += 1;
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


// CALLBACK Actions 

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // STANDARD PING RETURN
  if((topic=="broadcast") && (setting=='n')) {   // move most of this into the publish state function
    if(messageTemp == "ids") {publish_ids_json();}    
    if(messageTemp == "ping") {publish_reporting_json();}
    if(messageTemp == "restart") {ESP.restart();}
  }  

  if (topic == String(device_id)){
    receive_controls_json(messageTemp);
  }
}


//RECONNECT TO WIFI
void reconnect() {
  // Loop until reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    WiFi.mode(WIFI_STA);
    if (client.connect(device_id)) {
      Serial.print("connected ");
      Serial.print(device_id);  
      Serial.println(".");
      // subscribe to topic
      client.subscribe(device_id);
      client.subscribe("broadcast");
      client.subscribe("mqtt");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 2.0rem; }
    .units { font-size: 1.2rem; }
    .t-label{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8266 Setup</h2>
  <label class="units" id="device_id">Device ID: %DEVICE_ID%</label><br>
  <label class="units" id="type">Part Type: %TYPE%</label>
  <p><form action="/">
    <label class="t-label" for="network_id">Network ID:</label>
    <input class="t-label" type="text" id="network_id" name="input1">
    <br><br>
    <label class="t-label" for="password">Password:</label>
    <input class="t-label" type="text" id="password" name="input2">
    <br><br>
    <label class="t-label" for="mqtt">MQTT Addr:</label>
    <input class="t-label" type="text" id="mqtt_addr" name="input3">
    <br><br>
    <input class="units" type="submit" value="Submit">
  </form></p>
</body>
<script>
</script>
</html>)rawliteral";


// 'processor' replaces placeholder in html with X values
String processor(const String& var){
  //Serial.println(var);
  if(var == "DEVICE_ID"){
    return String(device_id);
  }
  if(var == "TYPE"){
    return String(type);
  }
  return String();
}



void def_server() {
    IPAddress myIP = WiFi.softAPIP();    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("input1") && request->hasParam("input2")) {
        String input1str = request->getParam("input1")->value();
        String input2str = request->getParam("input2")->value();
        String input3str = request->getParam("input3")->value();
        
        int len = input1str.length();
        EEPROM.write(network_id_length_addr, len);
        for(int i=0;i<len;i++) {
          EEPROM.write(network_id_addr+i, input1str[i]);
        }
        
        int len2 = input2str.length();
        EEPROM.write(password_length_addr, len2);
        for(int i=0;i<len2;i++) {
          EEPROM.write(password_addr+i, input2str[i]);
        }
        
        int addr_parts[4];
        int len3 = input3str.length();
        for (int i=0, start_idx=0, end_idx=0; i<4; i++, start_idx=end_idx+1) {
          end_idx = input3str.indexOf('.', start_idx);
          if (end_idx == -1) end_idx = input3str.length();
          addr_parts[i] = input3str.substring(start_idx, end_idx).toInt();
        }          
        EEPROM.write(ip1_addr, addr_parts[0]);
        EEPROM.write(ip2_addr, addr_parts[1]);
        EEPROM.write(ip3_addr, addr_parts[2]);
        EEPROM.write(ip4_addr, addr_parts[3]);
        
        EEPROM.write(setting_addr, 'n');
        
        
        EEPROM.commit();
        delay(1000);
        ESP.restart();
      }
      request->send_P(200, "text/html", index_html, processor);
    });

    // Start server
    server.begin();
}


#endif // MY_HEADER_FILE_H





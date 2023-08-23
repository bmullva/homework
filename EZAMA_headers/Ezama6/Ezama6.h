#ifndef EZAMA_H
#define EZAMA_H

#if defined(ESP8266)
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <PubSubClient.h>
  #include <EEPROM.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <ESP8266mDNS.h>
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
  #include <cstring>
  #include <ArduinoJson.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <PubSubClient.h>
  #include <ESPAsyncWebServer.h>
  #include <EEPROM.h>
  #include <ArduinoOTA.h>
  #include <cstring>
  #include <ArduinoJson.h>
#endif



//declare function prototypes and external variables
void receive_controls_json(String messageTemp);
void publish_ids_json();
void publish_reporting_json();
extern String type;



//SERVER CONSTANTS, VARIABLES, & EEPROM
const char *APSSID = "newModule";
const char *APPSK = "newModule";
const char *ssid_s = APSSID;
const char *password_s = APPSK;
const int ip1_addr = 185;
const int ip2_addr = 186;
const int ip3_addr = 187;
const int ip4_addr = 188;
const int network_id_length_addr = 189;
const int network_id_addr = 190;
const int device_id_addr = 222;
const int setting_addr = 230;
const int password_length_addr = 231;
const int password_addr = 240;
const int ezama_serial_num_addr = 303;
const int descript_length_addr = 311;
const int descript_addr = 312;



// INITIALIZE GLOBAL MODULES
WiFiClient espClient;
PubSubClient client(espClient);
AsyncWebServer server(80);



//GLOBAL VARIABLES
char mqtt_ip[16] {};
int network_id_length {};
char network_id[33] {};
char device_id[9] {};
char setting {};
int password_length {};
char password[64] {};
char ezama_serial_num[9];
int descript_length {};
char descript[25];



//HTML AND SERVER 
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
    <input class="t-label" type="text" id="network_id" name="input1" value="%NETWORK_ID%">
    <br><br>
    <label class="t-label" for="password">Password:</label>
    <input class="t-label" type="text" id="password" name="input2" value="%PASSWORD%">
    <br><br>
    <label class="t-label" for="mqtt">MQTT IP:</label>
    <input class="t-label" type="text" id="mqtt_ip" name="input3" value="%MQTT_IP%">
    <br><br>
    <label class="t-label" for="ezama">Raspi: Ezama-</label>
    <input class="t-label" type="text" id="ezama" name="input4" value="%EZAMA%">
    <br><br>
    <label class="t-label" for="descript">Description (24 char max):</label>
    <input class="t-label" type="text" id="descript" name="input5" maxlength="24">
    <br><br>
    <input class="units" type="submit" value="Submit">
  </form></p>

</body>
</html>)rawliteral";

String processor(const String& var) {
  if(var == "DEVICE_ID"){
    return String(device_id);
  }
  if(var == "TYPE"){
    return String(type);
  }
  if(var == "NETWORK_ID"){
    return String(network_id);
  }
  if(var == "PASSWORD"){
    return String(password);
  }
  if(var == "MQTT_IP"){
    return String(mqtt_ip);
  }
  if(var == "EZAMA"){
    return String(ezama_serial_num);
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
        String input4str = request->getParam("input4")->value();
        String input5str = request->getParam("input5")->value();
        
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
        
        int len4 = 8;
        EEPROM.write(ezama_serial_num_addr, len4);
        for(int i=0;i<len4;i++) {
          EEPROM.write(ezama_serial_num_addr+i, input4str[i]);
        }
        
        int len5 = input5str.length();
        EEPROM.write(descript_length_addr, len5);
        for(int i=0;i<len5;i++) {
          EEPROM.write(descript_addr+i, input5str[i]);
        }
        
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



//LOGIN TO STRONGEST EZAMA
void login_to_strongest_ezama() {
  String targetSSIDPrefix = "Ezama";
  String strongestSSID;

  while (true) {
    int numNetworks = WiFi.scanNetworks();
    
    if (numNetworks == 0) {
      Serial.println("No available networks found. Retrying...");
      delay(5000);
      continue;
    }

    int strongestSignal = -100;
    
    for (int i = 0; i < numNetworks; i++) {
      String ssid = WiFi.SSID(i);
      int rssi = WiFi.RSSI(i);

      if (ssid.startsWith(targetSSIDPrefix) && rssi > strongestSignal) {
        strongestSignal = rssi;
        strongestSSID = ssid;
      }
    }

    if (!strongestSSID.isEmpty()) {
      Serial.print("Strongest SSID: ");
      Serial.println(strongestSSID);
      //WiFi.mode(WIFI_STA);
      WiFi.begin(strongestSSID.c_str(), "EzamaNet");      
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print("Connecting to EzamaNet... ");
      }      
      Serial.println("Connected");      
      break;
    } else {
      Serial.println("No suitable network found. Retrying...");
      delay(5000);
    }
  }
}


//LOOKUP AND LOGIN TO EZAMA
void login_to_wireless_ezama() {
  for (int i = 0; i < 8; i++) {
    ezama_serial_num[i] = char(EEPROM.read(ezama_serial_num_addr + i));
  }
  
  String ssid = "EzamaNet-" + String(ezama_serial_num);
  Serial.print("Logging into: ");
  Serial.print(ssid);
  
  WiFi.begin(ssid.c_str(), "EzamaNet");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  
  Serial.println("... Success!");
}



//USED IN 'CALLBACK'
void set_parameters(const String& network_idString, const String& passwordString,
                    const String& mqtt_ipString, const String& ezama_serial_numString) {
  strcpy(network_id, network_idString.c_str());
  strcpy(password, passwordString.c_str());
  strcpy(mqtt_ip, mqtt_ipString.c_str());
  strcpy(ezama_serial_num, ezama_serial_numString.c_str());

  Serial.println("New Parameters:");
  Serial.println(network_id);
  Serial.println(password);
  Serial.println(mqtt_ip);
  Serial.println(ezama_serial_num);
}



//USED IN 'CALLBACK'
void update_parameters(const String& network_idString, const String& passwordString,
                       const String& mqtt_ipString, const String& ezama_serial_numString) {
  strcpy(network_id, network_idString.c_str());
  strcpy(password, passwordString.c_str());
  strcpy(mqtt_ip, mqtt_ipString.c_str());
  strcpy(ezama_serial_num, ezama_serial_numString.c_str());
  
  Serial.println("Updated Parameters:");
  Serial.println(network_id);
  Serial.println(password);
  Serial.println(mqtt_ip);
  Serial.println(ezama_serial_num);

  int network_id_length = strlen(network_id);
  EEPROM.write(network_id_length_addr, network_id_length);
  for (int i = 0; i < network_id_length; i++) {
    EEPROM.write(network_id_addr + i, network_id[i]);
  }

  int password_length = strlen(password);
  EEPROM.write(password_length_addr, password_length);
  for (int i = 0; i < password_length; i++) {
    EEPROM.write(password_addr + i, password[i]);
  }

  int addr_parts[4];
  int len3 = std::strlen(mqtt_ip);
  for (int i = 0, start_idx = 0, end_idx = 0; i < 4; i++, start_idx = end_idx + 1) {
    end_idx = -1;
    for (int j = start_idx; j < len3; j++) {
      if (mqtt_ip[j] == '.') {
        end_idx = j;
        break;
      }
    }
    if (end_idx == -1) end_idx = len3;
    addr_parts[i] = std::atoi(mqtt_ip + start_idx);
  }
  
  EEPROM.write(ip1_addr, addr_parts[0]);
  EEPROM.write(ip2_addr, addr_parts[1]);
  EEPROM.write(ip3_addr, addr_parts[2]);
  EEPROM.write(ip4_addr, addr_parts[3]);

  for (int i = 0; i < 8; i++) {
    EEPROM.write(ezama_serial_num_addr + i, ezama_serial_num[i]);
  }

  EEPROM.write(setting_addr, 'n');
  EEPROM.commit();
  delay(1000);
  ESP.restart();
}



//CALLBACK W/ THE ABOVE 2 FUNCTIONS
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");

  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (strcmp(topic, "parameters") == 0) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, messageTemp);
    JsonObject parameter = doc[0];

    String network_idString = parameter["network_id"].as<String>();
    String passwordString = parameter["password"].as<String>();
    String mqtt_ipString = parameter["ip"].as<String>();
    String ezama_serial_numString = parameter["ezamanet"].as<String>();

    if (setting == 's' && EEPROM.read(ip1_addr) == 255) {
      set_parameters(network_idString, 
                     passwordString, 
                     mqtt_ipString, 
                     ezama_serial_numString);
      Serial.print(network_idString);
      Serial.print(passwordString);
      Serial.print(mqtt_ipString); 
      Serial.println(ezama_serial_numString);
      WiFi.softAP(ssid_s, password_s);
      def_server();
    }
  
    else {
      update_parameters(network_idString, 
                        passwordString, 
                        mqtt_ipString, 
                        ezama_serial_numString);
      }
    }

  if (strcmp(topic, "broadcast") == 0 && setting == 'n') {
    if (messageTemp == "ids") { publish_ids_json(); }
    if (messageTemp == "ping") { publish_reporting_json(); }
    if (messageTemp == "restart") { ESP.restart(); }
  }

  if (strcmp(topic, device_id) == 0) {
    receive_controls_json(messageTemp);
  }
}



void publishMessage(const char* topic, const char* message) {
  client.publish(topic, message);
  Serial.print("Published message: ");
  Serial.println(message);
}



void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT... ");
    if (client.connect(device_id)) {
      Serial.println("Connected");
      client.subscribe(device_id);
      client.subscribe("broadcast");
      client.subscribe("mqtt");
      client.subscribe("parameters");   
    } else {
      Serial.print("Failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}



// SETUP WIFI
void setup_wifi() {
  Serial.println("Connecting to WiFi: ");
  Serial.println(network_id);
  WiFi.begin(network_id, password);
  for (int i = 0; i < 240 && WiFi.status() != WL_CONNECTED; i++) {
    delay(500);
    Serial.print(".");
    if (i == 239) {
      EEPROM.write(setting_addr, 's');
      EEPROM.commit();
      Serial.println("Restarting");
      ESP.restart();
    }
  }
}



void ezama_setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  delay(1000);
  
  for (int i = 0; i < 8; i++) device_id[i] = char(EEPROM.read(device_id_addr + i));
  setting = char(EEPROM.read(setting_addr));

  if (setting == 's') {
    client.setServer("10.10.1.1", 1883);
    client.setCallback(callback);
    Serial.print("Device id: ");
    Serial.println(device_id);
    Serial.print("setting: ");
    Serial.println(setting);
    
    if (EEPROM.read(ip1_addr) == 255) { // Initial login
      Serial.println("Logging into strongest ezama");
      login_to_strongest_ezama();
    } else {                           // Network changed
      Serial.println("Logging into wireless ezama");
      login_to_wireless_ezama();
    }
    
    //PING FOR NETWORK PARAMS, FINAL SETUP BY CALLBACK
    Serial.println("Setting MQTT, CALLBACK and Pinging Ezama");
    if (!client.connected()) {reconnect();}
    publishMessage("request_parameters", "ids");
  }  
  else if (setting == 'n') {
    String ip = String(EEPROM.read(ip1_addr)) + "." + String(EEPROM.read(ip2_addr)) + "." + 
                String(EEPROM.read(ip3_addr)) + "." + String(EEPROM.read(ip4_addr));
    ip.toCharArray(mqtt_ip, ip.length() + 1);
    network_id_length = EEPROM.read(network_id_length_addr);
    for (int i = 0; i < network_id_length; i++) network_id[i] = char(EEPROM.read(network_id_addr + i));
    password_length = EEPROM.read(password_length_addr);
    for (int i = 0; i < password_length; i++) password[i] = char(EEPROM.read(password_addr + i));
    for (int i = 0; i < 8; i++) ezama_serial_num[i] = char(EEPROM.read(ezama_serial_num_addr + i));
    descript_length = EEPROM.read(descript_length_addr);
    for (int i = 0; i < descript_length; i++) descript[i] = char(EEPROM.read(descript_addr + i));
    setup_wifi();
    client.setServer(mqtt_ip, 1883);
    client.setCallback(callback);
    ArduinoOTA.setHostname(device_id);
    ArduinoOTA.begin();
  }
}



void ezama_loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}


#endif // EZAMA_H
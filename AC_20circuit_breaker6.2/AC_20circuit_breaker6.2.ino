

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <cstring>
#include <ArduinoJson.h>
#include <map>
#include <TFT_eSPI.h>
#include <SPI.h>


// INITIALIZE DEVICE PARTICULAR CONSTANTS & VARIABLES
String type = "AC20 circuit break";
String ver = "6.2";

float Amps1_TRMS {};
float Amps2_TRMS {};
float Amps3_TRMS {};
float Amps4_TRMS {};
float Amps5_TRMS {};
float Amps6_TRMS {};

float amphour1 = 0;
float amphour2 = 0;
float amphour3 = 0;
float amphour4 = 0;
float amphour5 = 0;
float amphour6 = 0;


float testFrequency = 60;
float windowLength = 40.0/testFrequency;
float b1 = 9.317;
float b2 = .2077;

unsigned long printPeriod = 5000; // in milliseconds
unsigned long previousMillis = 0;

float s2a(float x) {
  float newx = 3.3 * x / 4024.0;
  float amp = b1*newx + b2*newx*newx;
  return amp;
}


//***************************
//***    DO NOT TOUCH     ***
//***************************

// INITIALIZE GLOBAL MODULES
  WiFiClient espClient;
  AsyncWebServer server(80);
  TFT_eSPI tft = TFT_eSPI();  // Invoke library


// EEPROM Definitions
const int network_id_length_addr = 189; // number len of network_id, 2 <= len <= 32
const int network_id_addr = 190; //ASCII 32 byte (190-221) unique ID for network_id 
const int device_id_addr = 222; //ASCII 8 digit (222-229) unique ID for this device
const int setting_addr = 230; //ASCII 'a','n', or 'c'  a-setup, b-get mqtt ip, c-normal
const int password_length_addr = 231; // number len of network password, 8 <= len <= 63
const int password_addr = 240; //ASCII n digit network password (240-(240+len) 8<=len<=63 


//GLOBAL VARIABLES
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
  
  tft.init();
  tft.setRotation(1);
  tft.setTextWrap(true, true);
  tft.setCursor(0, 0);
  tft.setTextSize(1);

  
  for(int i=0;i<8;i++) {device_id[i] = char(EEPROM.read(device_id_addr+i));}
  setting = char(EEPROM.read(setting_addr));
  
  
  if (setting=='s') {
    WiFi.softAP(ssid_s, password_s);
    def_server();
    }

  
  if (setting=='n') {
    network_id_length = EEPROM.read(network_id_length_addr);
    for(int i=0;i<network_id_length;i++) {network_id[i] = char(EEPROM.read(network_id_addr+i));}
    setting = char(EEPROM.read(setting_addr));
    password_length = EEPROM.read(password_length_addr);
    for(int i=0;i<password_length;i++) {password[i] = char(EEPROM.read(password_addr+i));}
    print_vars();
    setup_wifi();
    
    ArduinoOTA.setHostname(device_id);
    ArduinoOTA.begin();
    initialize_pins();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    // Define the handler for the /getPreviousMillis endpoint
    server.on("/getPreviousMillis", HTTP_GET, handleGetPreviousMillis);
    server.on("/setAction", HTTP_GET, handleSetAction);
    // Serve the main HTML page
    server.on("/", HTTP_GET, handleRoot);
    // Start the web server
    server.begin();
    Serial.println(WiFi.localIP());
  }
}


// DO NOT TOUCH - Ensures ESP is connected & sends messages to topics
void loop() {

  if (setting == 'n') {
    RunningStatistics stats1;           // create raw test signal stats 
    RunningStatistics stats2;           // create raw test signal stats
    RunningStatistics stats3;           // create raw test signal stats
    RunningStatistics stats4;           // create raw test signal stats
    RunningStatistics stats5;           // create raw test signal stats
    RunningStatistics stats6;           // create raw test signal stats

    stats1.setWindowSecs( windowLength );     //Set window length
    stats2.setWindowSecs( windowLength );     //Set window length
    stats3.setWindowSecs( windowLength );     //Set window length
    stats4.setWindowSecs( windowLength );     //Set window length
    stats5.setWindowSecs( windowLength );     //Set window length
    stats6.setWindowSecs( windowLength );     //Set window length

    while( true ) { 
      ArduinoOTA.handle();
      
      if (millis() % 17 == 0) {
        stats1.input(analogRead(36));
        stats2.input(analogRead(39));
        stats3.input(analogRead(34));
        stats4.input(analogRead(35));
        stats5.input(analogRead(32));
        stats6.input(analogRead(33));
      }

      if((unsigned long)(millis() - previousMillis) >= printPeriod) { //every 5 second we do the calculation
        previousMillis = millis();   // update time

        Amps1_TRMS = s2a(stats1.sigma());
        Amps2_TRMS = s2a(stats2.sigma());
        Amps3_TRMS = s2a(stats3.sigma());
        Amps4_TRMS = s2a(stats4.sigma());
        Amps5_TRMS = s2a(stats5.sigma());
        Amps6_TRMS = s2a(stats6.sigma());
        
        amphour1 += Amps1_TRMS * 2/3600;
        amphour2 += Amps2_TRMS * 2/3600;
        amphour3 += Amps3_TRMS * 2/3600;
        amphour4 += Amps4_TRMS * 2/3600;
        amphour5 += Amps5_TRMS * 2/3600;
        amphour6 += Amps6_TRMS * 2/3600;
      }
    }
    delay(5000);
  }

} 

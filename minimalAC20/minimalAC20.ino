#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Mull-2.4GHz";
const char* password = "4086794040";

AsyncWebServer server(80);

//bool isCounting = false;
unsigned int counter = 0;

float Amps1_TRMS = 5.0;
unsigned long previousMillis = 0;
const long interval = 2000; 

bool gpio25State = false; // Variable to track the state of GPIO25
bool gpio33State = true;  // Initialize GPIO33 as HIGH

String page;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  pinMode(33, OUTPUT);      // Set GPIO33 as output

  define_page();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    switch (static_cast<int>(Amps1_TRMS)) {
      case 5:
        Amps1_TRMS = 10.0;
        break;
      case 10:
        Amps1_TRMS = 15.0;
        break;
      case 15:
        Amps1_TRMS = 20.0;
        break;
      case 20:
        Amps1_TRMS = 5.0;  // Reset back to 5
        break;
    }
  }
}

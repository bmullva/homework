#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Mull-2.4GHz";
const char* password = "4086794040";

AsyncWebServer server(80);

//bool isCounting = false;
unsigned int counter = 0;

float Amps1_TRMS = 5.0;
float Amps2_TRMS = 20.0;
float Amps3_TRMS = 5.0;
float Amps4_TRMS = 10.0;
float Amps5_TRMS = 15.0;
float Amps6_TRMS = 20.0;
unsigned long previousMillis = 0;
const long interval = 2000; 

bool gpio25State = true; // Variable to track the state of output GPIO25
bool gpio22State = true;
bool gpio21State = true;
bool gpio19State = true;
bool gpio17State = true;
bool gpio16State = true;

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

  pinMode(25, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(25, HIGH);
  digitalWrite(22, HIGH);
  digitalWrite(21, HIGH);
  digitalWrite(19, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(16, HIGH);
  define_page();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    switch (static_cast<int>(Amps1_TRMS)) {
      case 5:
        Amps1_TRMS = 10.0;
        Amps2_TRMS = 10.0;
        break;
      case 10:
        Amps1_TRMS = 15.0;
        Amps2_TRMS = 5.0;
        break;
      case 15:
        Amps1_TRMS = 20.0;
        Amps2_TRMS = 0.0;
        break;
      case 20:
        Amps1_TRMS = 5.0;  // Reset back to 5
        Amps2_TRMS = 15.0;
        break;
    }
  }
}

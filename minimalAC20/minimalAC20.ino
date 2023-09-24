#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Mull-2.4GHz";
const char* password = "4086794040";

AsyncWebServer server(80);

//bool isCounting = false;
unsigned int counter = 0;

float Amps1_TRMS = 10.0;

bool gpio25State = false; // Variable to track the state of GPIO25
bool gpio33State = false;  // Initialize GPIO33 as HIGH

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

}

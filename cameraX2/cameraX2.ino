#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <esp_camera.h>

// Replace with your network credentials
const char* ssid = "Mull-2.4GHz";
const char* password = "4086794040";

// Create an instance of ESPAsyncWebServer
AsyncWebServer server(80);

// HTML code for the video streaming page
const char PROGMEM stream_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>ESP32-CAM Video Stream</title>
  </head>
  <body>
    <img src="/video" width="640" height="480">
  </body>
</html>
)rawliteral";

void setup() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print(WiFi.localIP());

  // Camera configuration
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera initialization failed");
    return;
  }

  // Route for streaming video
  server.on("/stream", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", stream_html);
  });

  // Route for streaming video frames
  server.on("/video", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->hasParam("frame")) {
      request->send(200, "text/plain", "Invalid request");
      return;
    }
    
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      request->send(200, "text/plain", "Camera capture failed");
      return;
    }

    request->send_P(200, "image/jpeg", (const uint8_t*)fb->buf, fb->len);

    esp_camera_fb_return(fb);
  });

  // Start the server
  server.begin();
}

void loop() {
  // Not much to do here
}

#include "esp_camera.h"
#include <WiFi.h>
#include "Arduino.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems
#include "ESPAsyncWebServer.h"

const char* ssid = "Mull-2.4GHz";
const char* password = "4086794040";

#define PART_BOUNDARY "123456789000000000000987654321"

#define CAMERA_MODEL_AI_THINKER


#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
  
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

AsyncWebServer server(80);

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
 
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  
  camera_config_t config;
  // ... Camera configuration ...

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.print(WiFi.localIP());

  // Start streaming web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", "<html><body><img src='/stream'></body></html>");
  });

server.on("/stream", HTTP_GET, [](AsyncWebServerRequest *request){
    String contentTypeHeaderValue = "multipart/x-mixed-replace;boundary=" + String(PART_BOUNDARY);
    AsyncWebServerResponse *response = request->beginResponse(200, contentTypeHeaderValue.c_str(), "");
    response->addHeader("Cache-Control", "no-store");
    response->addHeader("Connection", "close");
    response->addHeader("Pragma", "no-cache");
    request->send(response);

    while (true) {
      camera_fb_t * fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        continue;
      }
      // Send image data as raw bytes
      request->sendContent("--" PART_BOUNDARY "\r\n");
      request->sendContent("Content-Type: image/jpeg\r\n");
      request->sendContent("Content-Length: " + String(fb->len) + "\r\n");
      request->sendContent("\r\n");
      request->sendContent((const char *)fb->buf, fb->len);
      esp_camera_fb_return(fb);
    }
});




  server.begin();
}

void loop() {
  delay(1);
}

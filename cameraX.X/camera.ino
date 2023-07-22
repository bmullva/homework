#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <esp_camera.h>

// Replace with your network credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// Replace with your MQTT broker address
const char* mqttServer = "your.mqtt.server";
const int mqttPort = 1883;
const char* mqttUser = "YourMQTTUser";
const char* mqttPassword = "YourMQTTPassword";

// MQTT topics
const char* mqttPublishTopic = "image1";
const char* mqttSubscribeTopic = "broadcast";

// Create an instance of the camera
camera_fb_t* fb = NULL;

// Create an instance of the WiFiClient to handle the MQTT connection
WiFiClient espClient;

// Create an instance of the PubSubClient
PubSubClient client(espClient);

// Flag to indicate when to take a picture and publish it
bool shouldCaptureImage = false;

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT); // Optional - This is the flash LED pin, you can comment this line if your board doesn't have a flash LED.

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");

  // Connect to MQTT broker
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    if (client.connect("ESP32-CAM", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(mqttSubscribeTopic);
    } else {
      Serial.println("Failed to connect to MQTT broker");
      delay(2000);
    }
  }

  // Initialize the camera
  camera_config_t config;
  // ... (same camera configuration as before) ...

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed with error 0x%x", err);
    return;
  }

  // Wait for the camera to initialize
  delay(2000);

  // Start capturing images and publish when necessary
  captureAndPublish();
}

void captureAndPublish() {
  while (true) {
    if (shouldCaptureImage) {
      // Take a photo with the camera
      fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
      } else {
        // Publish the image to the MQTT topic
        client.publish(mqttPublishTopic, (uint8_t*)fb->buf, fb->len);
        Serial.println("Image published");

        // Release the frame buffer
        esp_camera_fb_return(fb);
      }
      
      // Reset the flag
      shouldCaptureImage = false;
    }

    // Maintain the MQTT connection
    if (!client.connected()) {
      reconnect();
    }

    // Loop the MQTT client
    client.loop();
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle MQTT messages
  payload[length] = '\0'; // Null-terminate the payload

  String receivedTopic = String(topic);
  String message = String((char*)payload);

  Serial.print("Received message on topic: ");
  Serial.println(receivedTopic);
  Serial.print("Message payload: ");
  Serial.println(message);

  // Check if the received message is "ping" on the "broadcast" topic
  if (receivedTopic.equals(mqttSubscribeTopic) && message.equals("ping")) {
    shouldCaptureImage = true;
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32-CAM", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(mqttSubscribeTopic);
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  // The main loop doesn't need to do anything in this example
}

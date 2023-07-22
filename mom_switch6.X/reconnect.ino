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

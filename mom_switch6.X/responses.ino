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

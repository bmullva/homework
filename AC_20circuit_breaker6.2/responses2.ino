//*************************************************
//***  FROM CALLBACK, CALLBACK, LOOP, CALLBACK  ***
//*************************************************

void publish_reporting_json() {
  String output;
  DynamicJsonDocument state_json(1024);
  char sj[1024];
  String topic = "reporting";
  state_json["device_id"] = device_id;
  state_json["type"] = type;
  state_json["ver"] = ver;
  state_json["IP"] = WiFi.localIP();
  state_json["watt"] = watt;
  if (digitalRead(13) == 0) {
    state_json["power_switch"] = "on";
  }
  if (digitalRead(13) == 1) {
    state_json["power_switch"] = "off";
  }
  serializeJson(state_json, output);
  output.toCharArray(sj, 1024);
  client.publish(topic.c_str(), sj);
  Serial.print("Returning State json: ");
  Serial.print(topic);
  Serial.print(sj);
}

void publish_ids_json() {
  String output;
  DynamicJsonDocument state_json(1024);
  char sj[1024];
  String topic = "broadcast";
  state_json["device_id"] = device_id;
  state_json["type"] = type;
  state_json["cat"] = "sensor-actuator";
  state_json["power_switch"]= "0,1";
  state_json["cmds"] = "on, off";
  state_json["ex"] = "on BOARD_ID topic, 'on'";
  serializeJson(state_json, output);
  output.toCharArray(sj, 1024);
  client.publish(topic.c_str(), sj);
}
  
void publish_controls_json(String pin_name, String pin_msg) {

}

void receive_controls_json(String messageTemp) {
   if(messageTemp == "restart") {ESP.restart();}
   
   if(messageTemp == "on") {
    digitalWrite(13, LOW);
    Serial.print("Writing 13 to LOW");
   }

   if(messageTemp == "off") {
    digitalWrite(13, HIGH);
    Serial.print("Writing 13 to HIGH");
   }   
}

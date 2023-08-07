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
  state_json["pin"]= "[p14,p12,p13]";
  state_json["action"] = "[click, dbl-click, hold, release]";
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
  state_json["cat"] = "actuator";
  state_json["pin"]= "[p14,p12,p13]";
  state_json["cmds"] = "[click, dbl-click, hold, release]";
  state_json["ex"] = "on CONTROL topic, ['p14':'click']";
  serializeJson(state_json, output);
  output.toCharArray(sj, 1024);
  client.publish(topic.c_str(), sj);
}
  
void publish_controls_json(String pin_name, String pin_msg) {
  String output;
  DynamicJsonDocument state_json(1024);
  char sj[1024];
  String topic = "control";
  state_json["device_id"] = device_id;
  state_json[pin_name] = pin_msg;
  serializeJson(state_json, output);
  output.toCharArray(sj, 1024);
  client.publish(topic.c_str(), sj);
}

void receive_controls_json(String messageTemp) {
  if(messageTemp == "restart") {ESP.restart();}
}

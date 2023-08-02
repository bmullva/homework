#include <ezama.h>


// 1 INITIALIZE DEVICE PARTICULAR CONSTANTS & VARIABLES
String type = "light_cntrl";
String ver = "7.0";

String dimmer14 = "off";
String dimmer12 = "off";
String dimmer15 = "off";
int p14 = 0;
int p12 = 0;
int p15 = 0;
int p_array[] = {p14, p12, p15};
int n_array[] = {14, 12, 15};
String d_array[] = {dimmer14, dimmer12, dimmer15};


// 2 REPORT (SENT EVERY 6 SECONDS)
void publish_reporting_json() {
  String output;
  DynamicJsonDocument state_json(1024);
  char sj[1024];
  String topic = "reporting";
  state_json["device_id"] = device_id;
  state_json["type"] = type;
  state_json["ver"] = ver;
  state_json["IP"] = WiFi.localIP();
  state_json["p14"] = p_array[0];
  state_json["p12"] = p_array[1];
  state_json["p15"] = p_array[2];
  serializeJson(state_json, output);
  output.toCharArray(sj, 1024);
  client.publish(topic.c_str(), sj);
  Serial.print("Returning State json: ");
  Serial.print(topic);
  Serial.print(sj);
}

// sensor, controller, actuator, actuator-sensor
// 3 REPORT ID: "mqtt_pub -h XXX.XXX.XXX.XXX -m ids -t broadcast"
void publish_ids_json() {
  String output;
  DynamicJsonDocument state_json(1024);
  char sj[1024];
  String topic = "broadcast";
  state_json["device_id"] = device_id;
  state_json["type"] = type;
  state_json["cat"] = "actuator";  // sensor, controller, actuator, actuator-controller
  state_json["pins"] = "[p14, p12, p15]";
  state_json["actions"]= "[switch, on, off, dim, stop, brighten, 0-255]";
  state_json["ex"] = "on device ID topic, ['p14':'dim']";
  serializeJson(state_json, output);
  output.toCharArray(sj, 1024);
  client.publish(topic.c_str(), sj);
}


// 4 RECEIVE CONTROLS (to this exact device, from callback)
void receive_controls_json(String messageTemp) {
  if(messageTemp == "restart") {ESP.restart();}
  
  StaticJsonDocument<256> jdoc;     
  DeserializationError error = deserializeJson(jdoc, messageTemp);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  String p14_msg = jdoc["p14"];
  String p12_msg = jdoc["p12"];
  String p15_msg = jdoc["p15"];
  String msg_array[] = {p14_msg, p12_msg, p15_msg};
      
  for (int i = 0; i<3; i++) {        
    if (msg_array[i] == "switch" && p_array[i] != 0) {
      analogWrite(n_array[i], 0);
      p_array[i] = 0;
    }
    else if (msg_array[i] == "switch" && p_array[i] == 0){
      analogWrite(n_array[i], 256); //CAN GO UP TO 255
      p_array[i] = 255;
    }      
    if(msg_array[i] == "on"){
      analogWrite(n_array[i], 255);
      p_array[i] = 255;
    }
    if(msg_array[i] == "off"){
      analogWrite(n_array[i], 0);
      p_array[i] = 0;
    }
    if(msg_array[i] == "dim"){
      d_array[i] = "dim";
    }
    if(msg_array[i] == "stop"){
      d_array[i] = "off";
    }
    if(msg_array[i] == "brighten"){
      d_array[i] = "brighten";
    }
    if(isDigit(msg_array[i][0])){
      p_array[i] = msg_array[i].toInt();
      analogWrite(n_array[i], p_array[i]);
    }
  }  
}


// 5 SEND CONTROLS SEND CONTROLS (publish_controls only if controller module)
void publish_controls_json(String pin_name, String pin_msg) {

}


//6 SETUP (pins)
void setup() { 
  ezama_setup();  //in ezama.h
  
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(15, OUTPUT);

}


//7 MAIN LOOP
void loop() {
  ezama_loop();  //in ezama.h
  
  for(int i=0;i<3;i++) {
    if(d_array[i] == "dim"){
      if (p_array[i] == 0) {p_array[i] = 255+5;}
      p_array[i] -= 5;
      if (p_array[i] < 5) {
        d_array[i] = "off";
        p_array[i] = 0;    
      }
      analogWrite(n_array[i], p_array[i]);
    }
    if(d_array[i] == "brighten"){
      p_array[i] += 5;
      if (p_array[i] > 256) {
        d_array[i] = "off";
        p_array[i] = 256;
      }
      analogWrite(n_array[i], p_array[i]);
    }
  }
}

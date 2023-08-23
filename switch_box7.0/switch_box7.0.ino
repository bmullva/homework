#include <Ezama.h>


// 1 INITIALIZE DEVICE PARTICULAR CONSTANTS & VARIABLES
String type = "switchBox";
String ver = "7.0";

const int d_pin [3] = {1, 2, 3};
const char* d_pin_name [3] = {"p1", "p2", "p3"};
int d_pin_reading [3] = {HIGH, HIGH, HIGH};
int d_pin_n1_reading [3] = {HIGH, HIGH, HIGH};
String d_pin_reading2state(int n) {
  if(n == 0) {return "on";}
  else {return "off";} 
  }
String timer [3] = {"off","off","off"};
unsigned long startMillis [3] = {0,0,0};
int clk [3] = {0,0,0};
String hld [3] = {"off","off","off"};


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
  state_json["pin"]= "[p14,p12,p13]";
  state_json["action"] = "[click, dbl-click, hold, release]";
  state_json["descript"] = descript;
  serializeJson(state_json, output);
  output.toCharArray(sj, 1024);
  client.publish(topic.c_str(), sj);
  Serial.print("Returning State json: ");
  Serial.print(topic);
  Serial.print(sj);
}


// 3 REPORT ID: "mqtt_pub -h XXX.XXX.XXX.XXX -m ids -t broadcast"
void publish_ids_json() {
  String output;
  DynamicJsonDocument state_json(1024);
  char sj[1024];
  String topic = "broadcast";
  state_json["device_id"] = device_id;
  state_json["type"] = type;
  state_json["cat"] = "controller";  // sensor, controller, actuator
  state_json["pin"]= "[p1,p2,p3]";
  state_json["cmds"] = "[click, dbl-click, hold, release]";
  state_json["ex"] = "on CONTROL topic, ['p1':'click']";
  serializeJson(state_json, output);
  output.toCharArray(sj, 1024);
  client.publish(topic.c_str(), sj);
}


// 4 RECEIVE CONTROLS (to this exact device, from callback)
void receive_controls_json(String messageTemp) {
  if(messageTemp == "restart") {ESP.restart();}
}


// 5 SEND CONTROLS SEND CONTROLS (publish_controls only if controller module)
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


//6 SETUP (pins)
void setup() { 
  ezama_setup();  //in ezama.h
  
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

}


//7 MAIN LOOP
void loop() {
  ezama_loop();  //in ezama.h
  
  for(int i=0;i<sizeof(d_pin_reading)/sizeof(d_pin_reading[0]);i++) {d_pin_reading[i] = digitalRead(d_pin[i]);}  

  for (int i = 0; i<3; i++){  
    if (d_pin_n1_reading[i] == HIGH && d_pin_reading[i] == LOW && timer[i] == "off") {
      startMillis[i] = millis();
      timer[i] = "on";
    }
  
    if (d_pin_n1_reading[i] == LOW && d_pin_reading[i] == HIGH && timer[i] == "on" && clk[i] == 0) {
      clk[i] += 1;
      publish_controls_json(String(d_pin_name[i]), "click");
    }

    else if (d_pin_n1_reading[i] == LOW && d_pin_reading[i] == HIGH && timer[i] == "on" && clk[i] != 0) {
      clk[i] = 0;
      publish_controls_json(String(d_pin_name[i]), "dbl-click");
      timer[i] = "off";
    }    
  
    else if (millis() - startMillis[i] > 1000 && d_pin_reading[i] == LOW && timer[i] == "on" && hld[i] == "off") {
      hld[i] = "on";
      publish_controls_json(String(d_pin_name[i]), "hold");
      timer[i] = "off";
    }

    else if (d_pin_n1_reading[i] == LOW && d_pin_reading[i] == HIGH && hld[i] == "on") {
      publish_controls_json(String(d_pin_name[i]), "release");
      hld[i] = "off";
    }
  
    if (millis() - startMillis[i] > 1000) {
      clk[i] = 0;
      timer[i] = "off";}
  
    d_pin_n1_reading[i] = d_pin_reading[i];
  }
}

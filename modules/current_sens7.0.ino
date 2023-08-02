#include <ezama.h>


// 1 INITIALIZE DEVICE PARTICULAR CONSTANTS & VARIABLES
String type = "current sens";
String ver = "7.0";

Adafruit_INA260 ina260 = Adafruit_INA260();
float current {};
float voltage {};
float power {};


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
  state_json["current_mA"] = current;
  state_json["voltage_mV"] = voltage;
  state_json["power_mW"] = power;
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
  state_json["cat"] = "sensor";  // sensor, controller, actuator
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

}


//6 SETUP (pins)
void setup() { 
  ezama_setup();  //in ezama.h
  
  if (!ina260.begin()) {
    Serial.println("Couldn't find INA260 chip");
    while (1);
  }

}


//7 MAIN LOOP
void loop() {
  ezama_loop();  //in ezama.h
  
  current = ina260.readCurrent();
  voltage = ina260.readBusVoltage();
  power = ina260.readPower();

  delay(1000);

}

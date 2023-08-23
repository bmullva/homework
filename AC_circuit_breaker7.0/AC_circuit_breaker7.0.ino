#include <Ezama.h>
#include <Filters.h>

// 1 INITIALIZE DEVICE PARTICULAR CONSTANTS & VARIABLES
String type = "AC Circuit";
String ver = "7.0";

float Amps1_TRMS {};
float AmpsX_TRMS {};
float Amps3_TRMS {};
float Amps4_TRMS {};
float Amps5_TRMS {};
float Amps6_TRMS {};
float Amps7_TRMS {};
float Amps8_TRMS {};
float Amps9_TRMS {};
float Amps10_TRMS {};
float Amps11_TRMS {};
float Amps12_TRMS {};
float Amps13_TRMS {};
float Amps14_TRMS {};
float Amps15_TRMS {};
float amphour1 = 0;
float amphour2 = 0;
float amphour3 = 0;
float amphour4 = 0;
float amphour5 = 0;
float amphour6 = 0;
float amphour7 = 0;
float amphour8 = 0;
float amphour9 = 0;
float amphour10 = 0;
float amphour11 = 0;
float amphour12 = 0;
float amphour13 = 0;
float amphour14 = 0;
float amphour15 = 0;
float testFrequency = 60;                    // test signal frequency (Hz)
float windowLength = 40.0/testFrequency;     // how long to average the signal, for statistist
unsigned long printPeriod = 2000; // in milliseconds
// Track time in milliseconds since last reading 
unsigned long previousMillis = 0;
//Please check the ACS712 Tutorial video by SurtrTech to see how to get them because it depends on your sensor, or look below
//float intercept = -0.0636; // to be adjusted based on calibration testing
//float slope = 0.0637; // to be adjusted based on calibration testing
//float sqd_slope = -0.0000754;
float intercept = -.64743685305625609;
float slope = .0070009828069142127;
float sqd_slope = .0000021417778313551217;

float s2a(float x) {
  float amp = intercept + slope*x + sqd_slope*x*x;
  if (amp < 0) {return 0;}
  if (amp >= 0) {return amp;}
}


// 2 REPORT (SENT EVERY 6 SECONDS)
void publish_reporting_json() {
  String topic = "reporting";
  String output1;
  String output2;
  String output3;
  String output4;
  DynamicJsonDocument state_json1(1024);
  DynamicJsonDocument state_json2(1024);
  DynamicJsonDocument state_json3(1024);
  DynamicJsonDocument state_json4(1024);
  char sj1[1024];
  char sj2[1024];
  char sj3[1024];
  char sj4[1024];
  state_json1["device_id"] = device_id;
  state_json1["descript"] = descript;
  state_json1["type"] = type;
  state_json1["ver"] = ver;
  state_json1["IP"] = WiFi.localIP();
  state_json2["device_id"] = device_id;
  state_json2["A1"] = Amps1_TRMS;
  state_json2["AX"] = AmpsX_TRMS;
  state_json2["A3"] = Amps3_TRMS;
  //state_json2["A4"] = Amps4_TRMS;
  state_json2["A5"] = Amps5_TRMS;
  state_json2["A6"] = Amps6_TRMS;
  //state_json2["A7"] = Amps7_TRMS;
  state_json2["A8"] = Amps8_TRMS;
  //state_json2["A9"] = Amps9_TRMS;
  state_json2["A10"] = Amps10_TRMS;
  //state_json2["A11"] = Amps11_TRMS;
  //state_json2["A12"] = Amps12_TRMS;
  //state_json2["A13"] = Amps13_TRMS;
  //state_json2["A14"] = Amps14_TRMS;
  //state_json2["A15"] = Amps15_TRMS;
  state_json3["device_id"] = device_id;
  state_json3["Ah1"] = amphour1;
  //state_json3["Ah2"] = amphour2;
  state_json3["Ah3"] = amphour3;
  //state_json3["Ah4"] = amphour4;
  state_json3["Ah5"] = amphour5;
  state_json3["Ah6"] = amphour6;
  //state_json3["Ah7"] = amphour7;
  state_json3["Ah8"] = amphour8;
  //state_json3["Ah9"] = amphour9;
  state_json3["Ah10"] = amphour10;
  //state_json3["Ah11"] = amphour11;
  //state_json3["Ah12"] = amphour12;
  //state_json3["Ah13"] = amphour13;
  //state_json3["Ah14"] = amphour14;
  //state_json3["Ah15"] = amphour15;
  state_json4["device_id"] = device_id;
  //state_json4["slg1"] = (digitalRead(9) == LOW) ? "on" : "off";
  //state_json4["slg2"] = (digitalRead(10) == LOW) ? "on" : "off";
  //state_json4["slg3"] = (digitalRead(11) == LOW) ? "on" : "off";
  //state_json4["slg4"] = (digitalRead(23) == LOW) ? "on" : "off";
  state_json4["slg5"] = (digitalRead(22) == LOW) ? "on" : "off";
  state_json4["slg6"] = (digitalRead(1) == LOW) ? "on" : "off";
  //state_json4["slg7"] = (digitalRead(3) == LOW) ? "on" : "off";
  state_json4["slg8"] = (digitalRead(21) == LOW) ? "on" : "off";
  //state_json4["slg9"] = (digitalRead(19) == LOW) ? "on" : "off";
  state_json4["slg10"] = (digitalRead(18) == LOW) ? "on" : "off";
  //state_json4["slg11"] = (digitalRead(5) == LOW) ? "on" : "off";
  //state_json4["slg12"] = (digitalRead(17) == LOW) ? "on" : "off";
  //state_json4["slg13"] = (digitalRead(16) == LOW) ? "on" : "off";
  //state_json4["slg14"] = (digitalRead(6) == LOW) ? "on" : "off";
  //state_json4["slg15"] = (digitalRead(7) == LOW) ? "on" : "off";
  serializeJson(state_json1, output1);
  serializeJson(state_json2, output2);
  serializeJson(state_json3, output3);
  serializeJson(state_json4, output4);
  output1.toCharArray(sj1, 1024);
  output2.toCharArray(sj2, 1024);
  output3.toCharArray(sj3, 1024);
  output4.toCharArray(sj4, 1024);
  publishMessage(topic.c_str(), sj1);
  publishMessage(topic.c_str(), sj2);
  publishMessage(topic.c_str(), sj3);
  publishMessage(topic.c_str(), sj4);
}


// 3 REPORT ID: "mqtt_pub -h XXX.XXX.XXX.XXX -m ids -t broadcast"
void publish_ids_json() {
  String output;
  DynamicJsonDocument state_json(1024);
  char sj[1024];
  String topic = "broadcast";
  state_json["device_id"] = device_id;
  state_json["type"] = type;
  state_json["cat"] = "sensor,actuator";  // sensor, controller, actuator
  state_json["power_switch"]= "0,1";
  state_json["cmds"] = "on, off";
  state_json["ex"] = "on BOARD_ID topic, 'on'";
  serializeJson(state_json, output);
  output.toCharArray(sj, 1024);
  //client.publish(topic.c_str(), sj);
  publishMessage(topic.c_str(), sj);
  
}


// 4 RECEIVE CONTROLS - ("slg3:"on")
void receive_controls_json(String messageTemp) {
  if(messageTemp == "restart") {ESP.restart();}
   
  StaticJsonDocument<256> jdoc;    
  DeserializationError error = deserializeJson(jdoc, messageTemp);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  //String slg1_msg = jdoc["slg1"];
  //String slg2_msg = jdoc["slg2"];
  //String slg3_msg = jdoc["slg3"];
  //String slg4_msg = jdoc["slg4"];
  String slg5_msg = jdoc["slg5"];
  String slg6_msg = jdoc["slg6"];
  //String slg7_msg = jdoc["slg7"];
  String slg8_msg = jdoc["slg8"];
  //String slg9_msg = jdoc["slg9"];
  String slg10_msg = jdoc["slg10"];
  //String slg11_msg = jdoc["slg11"];
  //String slg12_msg = jdoc["slg12"];
  //String slg13_msg = jdoc["slg13"];
  //String slg14_msg = jdoc["slg14"];
  //String slg15_msg = jdoc["slg15"];

  //if(slg1_msg == "on") {digitalWrite(9, LOW);}
  //if(slg2_msg == "on") {digitalWrite(10, LOW);}
  //if(slg3_msg == "on") {digitalWrite(11, LOW);}
  //if(slg4_msg == "on") {digitalWrite(23, LOW);}
  if(slg5_msg == "on") {digitalWrite(22, LOW);}
  if(slg6_msg == "on") {digitalWrite(1, LOW);}
  //if(slg7_msg == "on") {digitalWrite(3, LOW);}
  if(slg8_msg == "on") {digitalWrite(21, LOW);}
  //if(slg9_msg == "on") {digitalWrite(19, LOW);}
  if(slg10_msg == "on") {digitalWrite(18, LOW);}
  //if(slg11_msg == "on") {digitalWrite(5, LOW);}
  //if(slg12_msg == "on") {digitalWrite(17, LOW);}
  //if(slg13_msg == "on") {digitalWrite(16, LOW);}
  //if(slg14_msg == "on") {digitalWrite(6, LOW);}
  //if(slg15_msg == "on") {digitalWrite(7, LOW);}
  
  //if(slg1_msg == "off") {digitalWrite(9, HIGH);}
  //if(slg2_msg == "off") {digitalWrite(10, HIGH);}
  //if(slg3_msg == "off") {digitalWrite(11, HIGH);}
  //if(slg4_msg == "off") {digitalWrite(23, HIGH);}
  if(slg5_msg == "off") {digitalWrite(22, HIGH);}
  if(slg6_msg == "off") {digitalWrite(1, HIGH);}
  //if(slg7_msg == "off") {digitalWrite(3, HIGH);}
  if(slg8_msg == "off") {digitalWrite(21, HIGH);}
  //if(slg9_msg == "off") {digitalWrite(19, HIGH);}
  if(slg10_msg == "off") {digitalWrite(18, HIGH);}
  //if(slg11_msg == "off") {digitalWrite(5, HIGH);}
  //if(slg12_msg == "off") {digitalWrite(17, HIGH);}
  //if(slg13_msg == "off") {digitalWrite(16, HIGH);}
  //if(slg14_msg == "off") {digitalWrite(6, HIGH);}
  //if(slg15_msg == "off") {digitalWrite(7, HIGH);}
}


// 5 SEND CONTROLS SEND CONTROLS (publish_controls only if controller module)
void publish_controls_json(String pin_name, String pin_msg) {

}


//6 SETUP (pins)
void setup() { 
  ezama_setup();  //in ezama.h
  
  //LEFT SIDE
  pinMode(36, INPUT); // GOV 1  // TRUE ADC, INPUT ONLY
  pinMode(39, INPUT); // GOV 3  // TRUE ADC, INPUT ONLY
  pinMode(34, INPUT); // GOV 5  // TRUE ADC, INPUT ONLY
  pinMode(35, INPUT); // GOV 6  // TRUE ADC, INPUT ONLY
  pinMode(32, INPUT); // GOV 8  // TRUE ADC
  pinMode(33, INPUT); // GOV 10 // TRUE ADC 
  //pinMode(25, INPUT); // GOV 11
  //pinMode(26, INPUT); // GOV 13
  //pinMode(27, INPUT); // GOV 15
  //pinMode(14, INPUT); // VOG 2  //OUTPUT ONLY
  //pinMode(12, INPUT); // VOG 4  //OUTPUT ONLY******
  //GND
  //pinMode(13, INPUT);  // VOG 7  //OUTPUT ONLY******
  //pinMode(9, OUTPUT);  // SLG 1  //DO NOT USE
  //pinMode(10, OUTPUT); // SLG 2  //DO NOT USE
  //pinMode(11, OUTPUT); // SLG 3  //DO NOT USE
  
  //RIGHT SIDE
  //pinMode(23, OUTPUT); // SLG 4
  pinMode(22, OUTPUT); // SLG 5
  pinMode(1, OUTPUT); // SLG 6  //STAY AWAY*****
  //pinMode(3, OUTPUT); // SLG 7  //STAY AWAY*****
  pinMode(21, OUTPUT); // SLG 8
  //GND
  //pinMode(19, OUTPUT); // SLG 9
  pinMode(18, OUTPUT); // SLG 10
  //pinMode(5, OUTPUT); // SLG 11  //OUTPUT ONLY
  //pinMode(17, OUTPUT); // SLG 12
  //pinMode(16, OUTPUT); // SLG 13
  //pinMode(4, INPUT);  // VOG 9   //OUTPUT ONLY
  //pinMode(0, INPUT);  // VOG 12  //STAY AWAY
  //pinMode(2, INPUT);  // VOG 14  //OUTPUT ONLY 
  //pinMode(15, OUTPUT);  // UNUSED  //OUTPUT ONLY
  //pinMode(8, OUTPUT); // UNUSED  //DO NOT USE
  //pinMode(6, OUTPUT); // SLG 14  //DO NOT USE
  //pinMode(7, OUTPUT); // SLG 15  //DO NOT USE
  
  //digitalWrite(9, LOW);
  //digitalWrite(10, LOW);
  //digitalWrite(11, LOW);
  //digitalWrite(23, LOW);
  digitalWrite(22, LOW);
  digitalWrite(1, LOW);
  //digitalWrite(3, LOW);
  digitalWrite(21, LOW);
  //digitalWrite(19, LOW);
  digitalWrite(18, LOW);
  //digitalWrite(5, LOW);
  //digitalWrite(17, LOW);
  //digitalWrite(16, LOW);
  //digitalWrite(7, LOW);
  //digitalWrite(6, LOW);
}


void loop() {
  
  if (setting == 'n') {
    RunningStatistics stats1;           // create raw test signal stats 
    //RunningStatistics stats2;           // create raw test signal stats
    RunningStatistics stats3;           // create raw test signal stats
    //RunningStatistics stats4;           // create raw test signal stats
    RunningStatistics stats5;           // create raw test signal stats
    RunningStatistics stats6;           // create raw test signal stats
    //RunningStatistics stats7;           // create raw test signal stats
    RunningStatistics stats8;           // create raw test signal stats
    //RunningStatistics stats9;           // create raw test signal stats
    RunningStatistics stats10;          // create raw test signal stats
    //RunningStatistics stats11;          // create raw test signal stats
    //RunningStatistics stats12;          // create raw test signal stats
    //RunningStatistics stats13;          // create raw test signal stats
    //RunningStatistics stats14;          // create raw test signal stats
    //RunningStatistics stats15;          // create raw test signal stats

    stats1.setWindowSecs( windowLength );     //Set window length
    //stats2.setWindowSecs( windowLength );     //Set window length
    stats3.setWindowSecs( windowLength );     //Set window length
    //stats4.setWindowSecs( windowLength );     //Set window length
    stats5.setWindowSecs( windowLength );     //Set window length
    stats6.setWindowSecs( windowLength );     //Set window length
    //stats7.setWindowSecs( windowLength );     //Set window length
    stats8.setWindowSecs( windowLength );     //Set window length
    //stats9.setWindowSecs( windowLength );     //Set window length
    stats10.setWindowSecs( windowLength );    //Set window length
    //stats11.setWindowSecs( windowLength );    //Set window length
    //stats12.setWindowSecs( windowLength );    //Set window length
    //stats13.setWindowSecs( windowLength );    //Set window length
    //stats14.setWindowSecs( windowLength );    //Set window length
    //stats15.setWindowSecs( windowLength );    //Set window length
  
    while( true ) { 
      ezama_loop();  //in Ezama.h
      
      if (millis() % 13 == 0) {
        stats1.input(analogRead(36));
        stats3.input(analogRead(39));
        stats5.input(analogRead(34));
        stats6.input(analogRead(35));
        stats8.input(analogRead(32));
        stats10.input(analogRead(33));
        //stats11.input(analogRead(25));
        //stats13.input(analogRead(26));
        //stats15.input(analogRead(27));
        //stats2.input(analogRead(14));
        //stats4.input(analogRead(12));
        //stats7.input(analogRead(13));
        //stats9.input(analogRead(4));
        //stats12.input(analogRead(0));
        //stats14.input(analogRead(2));
      }

      if((unsigned long)(millis() - previousMillis) >= printPeriod) { //every second we do the calculation
        previousMillis = millis();   // update time

        AmpsX_TRMS = stats1.sigma();
        Amps1_TRMS = s2a(stats1.sigma());
        Amps3_TRMS = s2a(stats3.sigma());
        Amps5_TRMS = s2a(stats5.sigma());
        Amps6_TRMS = s2a(stats6.sigma());
        Amps8_TRMS = s2a(stats8.sigma());
        Amps10_TRMS = s2a(stats10.sigma());
        
        amphour1 += Amps1_TRMS * 2/3600;
        //amphour2 += Amps2_TRMS * 2/3600;
        amphour3 += Amps3_TRMS * 2/3600;
        //amphour4 += Amps4_TRMS * 2/3600;
        amphour5 += Amps5_TRMS * 2/3600;
        amphour6 += Amps6_TRMS * 2/3600;
        //amphour7 += Amps7_TRMS * 2/3600;
        amphour8 += Amps8_TRMS * 2/3600;
        //amphour9 += Amps9_TRMS * 2/3600;
        amphour10 += Amps10_TRMS * 2/3600;
        //amphour11 += Amps11_TRMS * 2/3600;
        //amphour12 += Amps12_TRMS * 2/3600;
        //amphour13 += Amps13_TRMS * 2/3600;
        //amphour14 += Amps14_TRMS * 2/3600;
        //amphour15 += Amps15_TRMS * 2/3600;
      }
    }
  }
}

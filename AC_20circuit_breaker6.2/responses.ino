//*************************************************
//***  FROM CALLBACK, CALLBACK, LOOP, CALLBACK  ***
//*************************************************

void publish_reporting() {   
  // THIS WILL BE SENT TO THE WEBSITE WHEN IT IS PROPERLY WRITTEN
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
  state_json2["A2"] = Amps2_TRMS;
  state_json2["A3"] = Amps3_TRMS;
  state_json2["A4"] = Amps4_TRMS;
  state_json2["A5"] = Amps5_TRMS;
  state_json2["A6"] = Amps6_TRMS;
  
  state_json3["device_id"] = device_id;
  state_json3["Ah1"] = amphour1;
  state_json3["Ah2"] = amphour2;
  state_json3["Ah3"] = amphour3;
  state_json3["Ah4"] = amphour4;
  state_json3["Ah5"] = amphour5;
  state_json3["Ah6"] = amphour6;
  
  state_json4["device_id"] = device_id;
  state_json4["slg1"] = (digitalRead(23) == LOW) ? "on" : "off";
  state_json4["slg2"] = (digitalRead(22) == LOW) ? "on" : "off";
  state_json4["slg3"] = (digitalRead(21) == LOW) ? "on" : "off";
  state_json4["slg4"] = (digitalRead(19) == LOW) ? "on" : "off";
  state_json4["slg5"] = (digitalRead(17) == LOW) ? "on" : "off";
  state_json4["slg6"] = (digitalRead(16) == LOW) ? "on" : "off";
  
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

  // THIS WILL BE SENT TO THE SCREEN WHEN IT IS WRITTEN
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.print("IP:  ");
  tft.println(ip_addr);
  tft.println();

  tft.setTextSize(1);
  if (true) {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.print("1  ");

  if (true) {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.print("2  ");
  
  if (false) {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.print("3  ");
  
  if (true) {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.print("4  ");
  
  if (false) {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.print("5  ");
  
  if (true) {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.println("6  ");
}


void receive_controls_json(String messageTemp) {  // NOT SURE WHAT THIS LOOKS LIKE, BUT WILL RECEIVE BUTTON CLICKS FROM WEBSITE
   
  StaticJsonDocument<256> jdoc;    
  DeserializationError error = deserializeJson(jdoc, messageTemp); // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  String slg1_msg = jdoc["slg1"];
  String slg2_msg = jdoc["slg2"];
  String slg3_msg = jdoc["slg3"];
  String slg4_msg = jdoc["slg4"];
  String slg5_msg = jdoc["slg5"];
  String slg6_msg = jdoc["slg6"];

  if(slg1_msg == "on") {digitalWrite(23, LOW);}
  if(slg2_msg == "on") {digitalWrite(22, LOW);}
  if(slg3_msg == "on") {digitalWrite(21, LOW);}
  if(slg4_msg == "on") {digitalWrite(19, LOW);}
  if(slg5_msg == "on") {digitalWrite(17, LOW);}
  if(slg6_msg == "on") {digitalWrite(16, LOW);}
  
  if(slg1_msg == "off") {digitalWrite(23, HIGH);}
  if(slg2_msg == "off") {digitalWrite(22, HIGH);}
  if(slg3_msg == "off") {digitalWrite(21, HIGH);}
  if(slg4_msg == "off") {digitalWrite(19, HIGH);}
  if(slg5_msg == "off") {digitalWrite(17, HIGH);}
  if(slg6_msg == "off") {digitalWrite(16, HIGH);}

}

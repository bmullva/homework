//*************************************************
//***  FROM CALLBACK, CALLBACK, LOOP, CALLBACK  ***
//*************************************************

void publish_reporting() {   

  slg1 = (digitalRead(25) == LOW) ? "on" : "off";
  slg2 = (digitalRead(22) == LOW) ? "on" : "off";
  slg3 = (digitalRead(21) == LOW) ? "on" : "off";
  slg4 = (digitalRead(19) == LOW) ? "on" : "off";
  slg5 = (digitalRead(17) == LOW) ? "on" : "off";
  slg6 = (digitalRead(16) == LOW) ? "on" : "off";
  
  //output1.toCharArray(sj1, 1024);

  // THIS WILL BE SENT TO THE SCREEN WHEN IT IS WRITTEN
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.print("IP:  ");
  tft.println(WiFi.localIP());
  tft.println();

  tft.setTextSize(1);
  if (slg1 == "on") {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.print("1  ");

  if (slg1 == "on") {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.print("2  ");
  
  if (slg1 == "on") {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.print("3  ");
  
  if (slg1 == "on") {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.print("4  ");
  
  if (slg1 == "on") {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.print("5  ");
  
  if (slg1 == "on") {tft.setTextColor(TFT_GREEN, TFT_BLACK);}
  else {tft.setTextColor(TFT_RED, TFT_BLACK);}
  tft.println("6  ");
}


void receive_controls(String messageTemp) {  // NOT SURE WHAT THIS LOOKS LIKE, BUT WILL RECEIVE BUTTON CLICKS FROM WEBSITE

  //if(slg1_msg == "on") {digitalWrite(25, LOW);}
  //if(slg2_msg == "on") {digitalWrite(22, LOW);}
  //if(slg3_msg == "on") {digitalWrite(21, LOW);}
  //if(slg4_msg == "on") {digitalWrite(19, LOW);}
  //if(slg5_msg == "on") {digitalWrite(17, LOW);}
  //if(slg6_msg == "on") {digitalWrite(16, LOW);}
  
  //if(slg1_msg == "off") {digitalWrite(25, HIGH);}
  //if(slg2_msg == "off") {digitalWrite(22, HIGH);}
  //if(slg3_msg == "off") {digitalWrite(21, HIGH);}
  //if(slg4_msg == "off") {digitalWrite(19, HIGH);}
  //if(slg5_msg == "off") {digitalWrite(17, HIGH);}
  //if(slg6_msg == "off") {digitalWrite(16, HIGH);}

}

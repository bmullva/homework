//*********************************
//***  FROM SETUP() AND LOOP()  ***
//*********************************

void initialize_pins() {
  pinMode(14, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
}


void send_messages() {
  for(int i=0;i<sizeof(d_pin_reading)/sizeof(d_pin_reading[0]);i++) {d_pin_reading[i] = digitalRead(d_pin[i]);}  

  for (int i = 0; i<3; i++){
  
  if (d_pin_n1_reading[i] == HIGH && d_pin_reading[i] == LOW && timer[i] == "off") {
    startMillis[i] = millis();
    timer[i] = "on";}
  
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

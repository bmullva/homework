
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 2.0rem; }
    .units { font-size: 1.2rem; }
    .t-label{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8266 Setup</h2>
  <label class="units" id="device_id">Device ID: %DEVICE_ID%</label><br>
  <label class="units" id="type">Part Type: %TYPE%</label>
  <p><form action="/">
    <label class="t-label" for="network_id">Network ID:</label>
    <input class="t-label" type="text" id="network_id" name="input1">
    <br><br>
    <label class="t-label" for="password">Password:</label>
    <input class="t-label" type="text" id="password" name="input2">
    <br><br>
    <label class="t-label" for="mqtt">MQTT Addr:</label>
    <input class="t-label" type="text" id="mqtt_addr" name="input3">
    <br><br>
    <input class="units" type="submit" value="Submit">
  </form></p>
</body>
<script>
</script>
</html>)rawliteral";


// 'processor' replaces placeholder in html with X values
String processor(const String& var){
  //Serial.println(var);
  if(var == "DEVICE_ID"){
    return String(device_id);
  }
  if(var == "TYPE"){
    return String(type);
  }
  return String();
}



void def_server() {
    IPAddress myIP = WiFi.softAPIP();    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("input1") && request->hasParam("input2")) {
        String input1str = request->getParam("input1")->value();
        String input2str = request->getParam("input2")->value();
        String input3str = request->getParam("input3")->value();
        
        int len = input1str.length();
        EEPROM.write(network_id_length_addr, len);
        for(int i=0;i<len;i++) {
          EEPROM.write(network_id_addr+i, input1str[i]);
        }
        
        int len2 = input2str.length();
        EEPROM.write(password_length_addr, len2);
        for(int i=0;i<len2;i++) {
          EEPROM.write(password_addr+i, input2str[i]);
        }
        
        int addr_parts[4];
        int len3 = input3str.length();
        for (int i=0, start_idx=0, end_idx=0; i<4; i++, start_idx=end_idx+1) {
          end_idx = input3str.indexOf('.', start_idx);
          if (end_idx == -1) end_idx = input3str.length();
          addr_parts[i] = input3str.substring(start_idx, end_idx).toInt();
        }          
        EEPROM.write(ip1_addr, addr_parts[0]);
        EEPROM.write(ip2_addr, addr_parts[1]);
        EEPROM.write(ip3_addr, addr_parts[2]);
        EEPROM.write(ip4_addr, addr_parts[3]);
        
        EEPROM.write(setting_addr, 'n');
        
        
        EEPROM.commit();
        delay(1000);
        ESP.restart();
      }
      request->send_P(200, "text/html", index_html, processor);
    });

    // Start server
    server.begin();
}

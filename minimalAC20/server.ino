void define_page() {
page = R"(
  <!DOCTYPE html>
  <html>
  <body>
    <h1>GPIO25: <span id='gpio25'></span></h1>
    <h1>GPIO33: <span id='gpio33'></span></h1>
    <input type='checkbox' id='toggleGPIO33' )";

page += gpio33State ? "checked" : "";

page += R"(
    ><label for='toggleGPIO33'>Toggle GPIO33</label><br>
    <script>
      var gpio33Checkbox = document.getElementById('toggleGPIO33');
      gpio33Checkbox.addEventListener('change', function() {
        fetch('/toggleGPIO33?state=' + (gpio33Checkbox.checked ? 'on' : 'off'));
      });
      function updateGPIO25() {
        fetch('/getGPIO25')
          .then(response => response.text())
          .then(data => {
            document.getElementById('gpio25').textContent = data;
          });
      }
      setInterval(updateGPIO25, 1000); // Update every second
    </script>
  </body>
  </html>
)";

  
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){    
    request->send(200, "text/html", page);
  });

  server.on("/toggleGPIO33", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasArg("state")) {
      String state = request->arg("state");
      gpio33State = (state == "on");
      digitalWrite(33, gpio33State ? HIGH : LOW); // Set GPIO33 state
    }
    request->send(200, "text/plain", gpio33State ? "on" : "off");
  });

  server.on("/getGPIO25", HTTP_GET, [](AsyncWebServerRequest *request){
    int gpio25Value = digitalRead(25);
    request->send(200, "text/plain", String(gpio25Value));
  });

  server.on("/getGPIO33", HTTP_GET, [](AsyncWebServerRequest *request){
    int gpio33Value = digitalRead(33);
    request->send(200, "text/plain", String(gpio33Value));
  });

  server.begin();
  
  }

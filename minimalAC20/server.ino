void define_page() {
  // Calculate the value of Amps1_TRMS divided by 20.0
  float ampsValue = Amps1_TRMS / 20.0;

  // Ensure the value is within the valid range for <meter>
  ampsValue = constrain(ampsValue, 0.0, 1.0);

  page = R"(
    <!DOCTYPE html>
    <html>
    <head>
    <style>
    /* Style the toggle switch container */
    .switch-container {
      display: flex;
      align-items: center;
      margin-bottom: 20px; /* Add some spacing below the switch */
    }

    /* Style the toggle switch */
    .switch {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 30px;
    }

    /* Hide the default HTML checkbox */
    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    /* Style the slider (rounded shape) */
    .slider {
      position: absolute;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      border-radius: 30px; /* Make it rounded */
      transition: 0.4s;
    }

    /* Style the slider when it's in the "on" state */
    .slider:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 2px;
      bottom: 2px;
      background-color: white;
      border-radius: 50%;
      transition: 0.4s;
    }

    /* When the checkbox is checked, move the slider to the right */
    input:checked + .slider {
      background-color: #2196F3; /* Change to your desired "on" color */
    }

    input:checked + .slider:before {
      transform: translateX(26px);
    }
    </style>
    </head>
    <body>
      <h1>GPIO33: <span id='gpio33'></span></h1>
      <div class="switch-container">
        <label class="switch">
          <input type='checkbox' id='toggleGPIO33')";
          
page += gpio33State ? "checked" : "";

page += R"(>
          <span class="slider"></span>
        </label>
        <label for='toggleGPIO33'>Toggle GPIO33</label>
      </div>
      <h1>Amps1_TRMS: <meter value=')";
      
page += String(ampsValue);

page += R"(' min='0' max='1' low='0.2' high='0.8' optimum='1.0'></meter></h1>
      <script>
        var gpio33Checkbox = document.getElementById('toggleGPIO33');
        gpio33Checkbox.addEventListener('change', function() {
          fetch('/toggleGPIO33?state=' + (gpio33Checkbox.checked ? 'on' : 'off'));
        });
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


  server.on("/getGPIO33", HTTP_GET, [](AsyncWebServerRequest *request){
    int gpio33Value = digitalRead(33);
    request->send(200, "text/plain", String(gpio33Value));
  });

  
  server.begin();
}

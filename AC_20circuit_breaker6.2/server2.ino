const char index_html2[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<script>
function updateAmps1_TRMS() {
  // Fetch and update Amps1_TRMS
  var xhr1 = new XMLHttpRequest();
  xhr1.onreadystatechange = function() {
    if (xhr1.readyState === 4 && xhr1.status === 200) {
      document.getElementById("Amps1_TRMS").innerHTML = xhr1.responseText;
    }
  };
  xhr1.open("GET", "/getAmps1_TRMS", true);
  xhr1.send();

  // Fetch and update Amps2_TRMS
  var xhr2 = new XMLHttpRequest();
  xhr2.onreadystatechange = function() {
    if (xhr2.readyState === 4 && xhr2.status === 200) {
      document.getElementById("Amps2_TRMS").innerHTML = xhr2.responseText;
    }
  };
  xhr2.open("GET", "/getAmps2_TRMS", true);
  xhr2.send();
}


function sendAction(checkboxNumber) {
  var checkboxId = "actionCheckbox" + checkboxNumber;
  var checkbox = document.getElementById(checkboxId);
  var action = checkbox.checked ? "on" : "off";

  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/setAction" + checkboxNumber + "?state=" + action, true); // Include checkboxNumber in the URL
  xhr.send();
}


  // Update every 5000 milliseconds (5 seconds)
  setInterval(updateAmps1_TRMS, 5000);
</script>
</head>

<body>
  <h1>Previous Millis Value 1:</h1>
  <p id="Amps1_TRMS">Fetching...</p>

  <h1>Previous Millis Value 2:</h1>
  <p id="Amps2_TRMS">Fetching...</p>


<h2>Control Arduino:</h2>
<label for="actionCheckbox1">Perform Action 1:</label>
<input type="checkbox" id="actionCheckbox1">

<label for="actionCheckbox2">Perform Action 2:</label>
<input type="checkbox" id="actionCheckbox2">

<button onclick="sendAction(1)">Send Action 1</button>
<button onclick="sendAction(2)">Send Action 2</button>

</body>

</html>
)rawliteral";


void handleGetAmps_TRMS(AsyncWebServerRequest *request) {
  publish_reporting();
  
  String response;
  String reqUri = request->url(); // Rename the local variable

  // Check the request URI to determine which variable to respond with
  if (reqUri == "/getAmps1_TRMS") {
    response = String("This will be Amps1_TRMS");
  } else if (reqUri == "/getAmps2_TRMS") {
    response = String("This will be Amps2_TRMS");
  } else {
    response = "Invalid request"; // Handle invalid requests
  }

  request->send(200, "text/html", response);
}


void handleSetAction1(AsyncWebServerRequest *request) {
  if (request->hasArg("state")) {
    String state = request->arg("state");
    // Process the action based on the 'state' value for checkbox 1
    // You can add your specific action code here for checkbox 1.
    request->send(200, "text/plain", "Action 1 set to: " + state);
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
}

void handleSetAction2(AsyncWebServerRequest *request) {
  if (request->hasArg("state")) {
    String state = request->arg("state");
    // Process the action based on the 'state' value for checkbox 2
    // You can add your specific action code here for checkbox 2.
    request->send(200, "text/plain", "Action 2 set to: " + state);
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
}

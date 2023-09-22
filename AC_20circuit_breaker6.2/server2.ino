const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<script>
  function updatePreviousMillis() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
      if (xhr.readyState === 4 && xhr.status === 200) {
        document.getElementById("previousMillis").innerHTML = xhr.responseText;
      }
    };
    xhr.open("GET", "/getPreviousMillis", true);
    xhr.send();
  }

  function sendAction() {
    var checkbox = document.getElementById("actionCheckbox");
    var action = checkbox.checked ? "on" : "off"; // Convert checkbox state to "on" or "off"

    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/setAction?state=" + action, true); // Send the action state to Arduino
    xhr.send();
  }

  // Update every 5000 milliseconds (5 seconds)
  setInterval(updatePreviousMillis, 5000);
</script>
</head>

<body>
  <h1>Previous Millis Value:</h1>
  <p id="previousMillis">Fetching...</p>

  <h2>Control Arduino:</h2>
  <label for="actionCheckbox">Perform Action:</label>
  <input type="checkbox" id="actionCheckbox">
  <button onclick="sendAction()">Send Action</button>
</body>

</html>
)rawliteral";


void handleGetPreviousMillis() {
  String response = String(previousMillis);
  server.send(200, "text/plain", response);
  //NEED TO UPDATE THIS TO PULL THE PROPER 
}

void handleSetAction() {
  if (server.hasArg("state")) {
    String state = server.arg("state");
    // Process the action based on the 'state' value (e.g., perform some action if "on")
    // You can add your specific action code here.
    server.send(200, "text/plain", "Action set to: " + state);
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

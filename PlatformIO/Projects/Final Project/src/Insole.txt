#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

const char* ssid = "AngelaMei";
const char* password = "123456789";

// Force Sensors
const int Force1 = 32;
const int Force2 = 33;
const int Force3 = 34;
int fsrReading1, fsrReading2, fsrReading3;
float forceInNewtons1, forceInNewtons2, forceInNewtons3;

float convertToForce(int adcValue) {
  float maxForce = 98.0;  // Max force in Newtons
  float maxAdcValue = 4095.0;
  return (adcValue / maxAdcValue) * maxForce;
}

// Web server setup
WebServer server(80);

// WebSocket setup
WebSocketsServer webSocket = WebSocketsServer(81); // WebSocket on port 81


// HTML content served by ESP32
const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Insole Force Sensors</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
    }
    .dot {
      position: absolute;
      background-color: red;
      border-radius: 50%;
      opacity: 0.8;
    }
  </style>
  <script>
    const updateDot = (id, force, x, y) => {
      const dot = document.getElementById(id);
      dot.style.width = force * 10 + "px"; // Adjust scaling factor
      dot.style.height = force * 10 + "px";
      dot.style.left = x + "px";
      dot.style.top = y + "px";
    };

    const socket = new WebSocket(`ws://${window.location.hostname}:81`);

    socket.onmessage = (event) => {
      const data = JSON.parse(event.data);
      updateDot('dot1', data.force1, 50, 100);
      updateDot('dot2', data.force2, 150, 200);
      updateDot('dot3', data.force3, 250, 300);
    };

    socket.onerror = () => console.error("WebSocket error occurred");
  </script>
</head>
<body>
  <h1>Insole Force Sensors</h1>
  <div id="dot1" class="dot"></div>
  <div id="dot2" class="dot"></div>
  <div id="dot3" class="dot"></div>
</body>
</html>
)rawliteral";

// Serve the HTML page
void handleRoot() {
  server.send(200, "text/html", INDEX_HTML);
}

// Send force data to the client over WebSocket
void sendForceData() {
  String jsonData = "{\"force1\":" + String(forceInNewtons1) +
                    ",\"force2\":" + String(forceInNewtons1) +
                    ",\"force3\":" + String(forceInNewtons1) + "}";
  webSocket.broadcastTXT(jsonData); // Send data to all connected clients
  // Serial.print(forceInNewtons1);
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  // Initialize web server
  server.on("/", handleRoot);
  server.begin();

  // Initialize WebSocket server
  webSocket.begin();

  Serial.println("Web server and WebSocket server started.");
}

void loop() {
  // Read ADC values from the force sensors
  fsrReading1 = analogRead(Force1);
  fsrReading2 = analogRead(Force2);
  fsrReading3 = analogRead(Force3);

  // Convert ADC values to force in Newtons
  forceInNewtons1 = convertToForce(fsrReading1);
  forceInNewtons2 = convertToForce(fsrReading2);
  forceInNewtons3 = convertToForce(fsrReading3);

  // Handle WebSocket and web server
  server.handleClient();
  webSocket.loop(); 

  // Send force data every 1 second
  static unsigned long lastTime = 0;
  if (millis() - lastTime > 1000) {
    sendForceData();  // Send the updated force data
    lastTime = millis();
  }
}
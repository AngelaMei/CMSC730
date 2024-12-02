#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

const char* ssid = "AngelaMei";
const char* password = "123456789";

// const char* ssid = "5120Navahoe";
// const char* password = "5120Roommates!";

// Web server setup
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81); // WebSocket on port 81

// Pin Definitions
const int fsrPins[] = {34, 35, 32}; // Array of FSR pin numbers
const int numFSR = sizeof(fsrPins) / sizeof(fsrPins[0]); // Number of FSR sensors

// Variables
float fsrReadings[numFSR]; // Array to store analog readings
float voltages[numFSR];    // Array to store calculated voltages
float forces[numFSR];      // Array to store calculated forces (optional)

// ADC Reference Voltage
const float Vcc = 3.3; // ESP32 ADC reference voltage
const int ADCResolution = 4095; // 12-bit ADC resolution

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    Serial.printf("WebSocket [%u] received: %s\n", num, payload);
  }
}

// JSON data string
String jsonData;

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP Address: ");
  Serial.println(WiFi.localIP());

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);

  // Start HTTP server
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/plain", "FSR Data Server");
  });

  // Add HTTP endpoint to serve FSR data as JSON
  server.on("/get-data", HTTP_GET, []() {
    server.send(200, "application/json", jsonData);
  });

  server.begin();
}

void loop() {
 // Read and process FSR data
  jsonData = "{";
  for (int i = 0; i < numFSR; i++) {
    fsrReadings[i] = analogRead(fsrPins[i]);
    voltages[i] = fsrReadings[i] * (Vcc / ADCResolution);
    forces[i] = voltages[i] > 0.1 ? pow(voltages[i], 2.5) * 10 : 0;

    // Append data to JSON string
    jsonData += "\"FSR" + String(i) + "\":" + String(forces[i]);
    if (i < numFSR - 1) jsonData += ", ";
  }
  jsonData += "}";

  // Print to Serial Monitor
  Serial.println(jsonData);

  webSocket.broadcastTXT(jsonData);

  webSocket.loop();
  server.handleClient();

  delay(10);
}



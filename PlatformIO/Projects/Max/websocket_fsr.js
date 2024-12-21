autowatch =  1;

post("here")

// JavaScript for MAX/MSP `js` object
var outlet; // Outlet for MAX output

// WebSocket variables
var ws1 = null;
var ws2 = null;

function connect() {
    // Connect to ESP32 #1 WebSocket
    ws1 = new WebSocket('ws://10.0.0.123:82'); // Replace with ESP32 #1's IP
    ws1.onopen = function() {
        post('Connected to ESP32 #1 WebSocket\n');
    };
    ws1.onmessage = function(event) {
        var data = JSON.parse(event.data); // Parse the JSON data
        outlet(0, JSON.stringify(data));  // Send data to outlet 0
    };
    ws1.onerror = function(error) {
        post('ESP32 #1 WebSocket Error: ' + error.message + '\n');
    };
    ws1.onclose = function() {
        post('ESP32 #1 WebSocket Closed\n');
    };

    // Connect to ESP32 #2 WebSocket
    ws2 = new WebSocket('ws://10.0.0.126:81'); // Replace with ESP32 #2's IP
    ws2.onopen = function() {
        post('Connected to ESP32 #2 WebSocket\n');
    };
    ws2.onmessage = function(event) {
        var data = JSON.parse(event.data); // Parse the JSON data
        outlet(1, JSON.stringify(data));  // Send data to outlet 1
    };
    ws2.onerror = function(error) {
        post('ESP32 #2 WebSocket Error: ' + error.message + '\n');
    };
    ws2.onclose = function() {
        post('ESP32 #2 WebSocket Closed\n');
    };
}

// Function to close connections
function disconnect() {
    if (ws1) {
        ws1.close();
        ws1 = null;
    }
    if (ws2) {
        ws2.close();
        ws2 = null;
    }
    post('Disconnected WebSocket connections\n');
}

connect();
let socket;
let socket2;

// Function to initialize WebSocket connection for ESP32 1
function startWebSocket1() {
    const esp32IP1 = "ws://10.0.0.123:82"; // ESP32 #1 IP address
    socket = new WebSocket(esp32IP1);

    socket.onopen = function() {
        post("Connected to WebSocket 1\n");
    };

    socket.onmessage = function(event) {
        try {
            const data = JSON.parse(event.data); // Parse the incoming JSON data
            post("Received data from ESP32 #1: " + JSON.stringify(data) + "\n");
            outlet(0, data); // Send data to Max
        } catch (e) {
            post("Error parsing data from ESP32 #1: " + e.message + "\n");
        }
    };

    socket.onerror = function(error) {
        post("WebSocket 1 Error: " + error.message + "\n");
    };

    socket.onclose = function() {
        post("WebSocket 1 closed\n");
    };
}

// Function to initialize WebSocket connection for ESP32 2
function startWebSocket2() {
    const esp32IP2 = "ws://10.0.0.126:81"; // ESP32 #2 IP address
    socket2 = new WebSocket(esp32IP2);

    socket2.onopen = function() {
        post("Connected to WebSocket 2\n");
    };

    socket2.onmessage = function(event) {
        try {
            const data = JSON.parse(event.data); // Parse the incoming JSON data
            post("Received data from ESP32 #2: " + JSON.stringify(data) + "\n");
            outlet(1, data); // Send data to Max
        } catch (e) {
            post("Error parsing data from ESP32 #2: " + e.message + "\n");
        }
    };

    socket2.onerror = function(error) {
        post("WebSocket 2 Error: " + error.message + "\n");
    };

    socket2.onclose = function() {
        post("WebSocket 2 closed\n");
    };
}

// Define the 'bang' function that Max will trigger
function bang() {
    post("Bang triggered! Opening WebSocket connections...\n");

    // Start both WebSocket connections
    startWebSocket1();
    startWebSocket2();
}

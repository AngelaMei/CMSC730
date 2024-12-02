let socket1, socket2;

// Initialize WebSocket for ESP32 #1
function startWebSocket1() {
    const esp32IP1 = "ws://10.0.0.123:82"; // Replace with ESP32 #1's IP
    socket1 = new WebSocket(esp32IP1);

    socket1.onopen = function() {
        console.log("Connected to WebSocket 1");
    };

    socket1.onmessage = function(event) {
        const data = JSON.parse(event.data); // Parse JSON data
        updateCanvas("canvas1", data); // Update visualization
    };

    socket1.onerror = function(error) {
        console.error("WebSocket 1 Error: ", error);
    };

    socket1.onclose = function() {
        console.log("WebSocket 1 closed");
    };
}

// Initialize WebSocket for ESP32 #2
function startWebSocket2() {
    const esp32IP2 = "ws://172.20.10.3:81"; // Replace with ESP32 #2's IP
    socket2 = new WebSocket(esp32IP2);

    socket2.onopen = function() {
        console.log("Connected to WebSocket 2");
    };

    socket2.onmessage = function(event) {
        try {
            const data = JSON.parse(event.data); // Parse JSON data
            console.log("Parsed Data:", data); // Log for debugging
    
            // Convert object values to an array
            const fsrValues = Object.values(data);
            updateCanvas("canvas2", fsrValues); // Pass the array to the canvas function
        } catch (e) {
            console.error("Failed to parse JSON:", e);
        }
    };    

    socket2.onerror = function(error) {
        console.error("WebSocket 2 Error: ", error);
    };

    socket2.onclose = function() {
        console.log("WebSocket 2 closed");
    };
}

function updateCanvas(canvasId, data) {
    if (!Array.isArray(data)) {
        console.error(`Expected an array but got:`, data);
        return;
    }

    const canvas = document.getElementById(canvasId);
    if (!canvas) {
        console.error(`Canvas with id "${canvasId}" not found.`);
        return;
    }

    const ctx = canvas.getContext("2d");
    if (!ctx) {
        console.error("Failed to get canvas 2D context.");
        return;
    }

    // Clear the canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // Draw circles based on FSR data
    data.forEach((fsrValue, index) => {
        const x = (index + 1) * 100;
        const y = canvas.height / 2;
        const radius = Math.max(5, fsrValue); // Minimum size to prevent disappearing

        ctx.beginPath();
        ctx.arc(x, y, radius, 0, 2 * Math.PI);
        ctx.fillStyle = "rgba(255, 0, 0, 0.5)";
        ctx.fill();
        ctx.closePath();
    });
}


// Initialize both WebSocket connections on window load
window.onload = function() {
    startWebSocket1();
    startWebSocket2();
};
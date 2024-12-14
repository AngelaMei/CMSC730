// --- Constants and Global Variables ---
// const websockets = {};
let sockets = {}; // Store all WebSocket connections
const calibrationDuration = 5000;

/**
 * Sound
 */
const backgroundMusic = new Audio('/static/APT.m4a')
backgroundMusic.loop = true
backgroundMusic.volume = 0.4
let isPlaying = false;

// const toggleButton = document.getElementById("toggleButton");
// const musicIcon = document.getElementById("music-icon");

// toggleButton.addEventListener("click", () => {
//     if (isPlaying) {
//         backgroundMusic.pause();
//         musicIcon.src = "/static/musicoff.png";
//         console.log("Music paused.");
//     } else {
//         backgroundMusic.play();
//         musicIcon.src = "/static/music-player.png";
//         console.log("Music playing.");
//     }
//     isPlaying = !isPlaying;
// });

/**
 * Sound Effect
 */
// const soundEffects = {
//     left: {
//         toe: new Audio('/static/730A.mp3'),
//         middle: new Audio('/static/730C3.mp3'),
//         heel: new Audio('/static/730D.mp3')
//     },
//     right: {
//         toe: new Audio('/static/730D3.mp3'),
//         middle: new Audio('/static/730F.mp3'),
//         heel: new Audio('/static/730G2.mp3')
//     }
// };

/**
 * Websocket
 */
function initializeWebSocket(url, label, onMessageCallback) {
    const socket = new WebSocket(url);

    socket.onopen = function () {
        console.log(`Connected to WebSocket (${label}): ${url}`);
    };

    socket.onmessage = function (event) {
        try {
            const data = JSON.parse(event.data);
            // console.log(`Data received from ${label}:`, data);
            if (onMessageCallback) onMessageCallback(data);
        } catch (e) {
            console.error(`Failed to parse JSON from ${label}:`, e);
        }
    };

    socket.onerror = function (error) {
        console.error(`WebSocket Error (${label}):`, error);
    };

    socket.onclose = function () {
        console.log(`WebSocket (${label}) closed`);
    };

    return socket;
}

function initializeAllWebSockets() {
    sockets["ESP32_1"] = initializeWebSocket("ws://10.0.0.215:81", "ESP32_1", (data) => {
        // console.log("ESP32_1 Data:", data);
        const pressedPart = detectPressedFSRWithCalibration("player1", "left", data);
        // console.log("Detected Pressed Part (ESP32_1):", pressedPart);
        if (pressedPart) {
            handlePress("player1", "left", pressedPart);
        }
    });

    sockets["ESP32_2"] = initializeWebSocket("ws://10.0.0.179:82", "ESP32_2", (data) => {
        // console.log("ESP32_2 Data Received:", data);
        const pressedPart = detectPressedFSRWithCalibration("player1", "right", data);
        // console.log("Detected Pressed Part (ESP32_2):", pressedPart);
        if (pressedPart) {
            handlePress("player1", "right", pressedPart);
        }
    });

    sockets["ESP32_3"] = initializeWebSocket("ws://10.0.0.126:84", "ESP32_3", (data) => {
        // console.log("ESP32_3 Data:", data);
        const pressedPart = detectPressedFSRWithCalibration("player2", "left", data);
        // console.log("Detected Pressed Part (ESP32_3):", pressedPart);
        if (pressedPart) {
            handlePress("player2", "left", pressedPart);
        }
    });


    sockets["ESP32_4"] = initializeWebSocket("ws://10.0.0.181:83", "ESP32_4", (data) => {
        // console.log("ESP32_4 Data:", data);
        const pressedPart = detectPressedFSRWithCalibration("player2", "right", data);
        // console.log("Detected Pressed Part (ESP32_4):", pressedPart);
        if (pressedPart) {
            handlePress("player2", "right", pressedPart);
        }
    });
}

/**
 * calibration
 */
let calibrationData = {
    player1: { left: {}, right: {} },
    player2: { left: {}, right: {} },
};

let isCalibrating = false;
let reactiongame = false;

function calculateAverage(dataArray) {
    return dataArray.reduce((sum, value) => sum + value, 0) / dataArray.length;
}

function formatReactionTime(milliseconds) {
    if (milliseconds <= 0 || isNaN(milliseconds)) return "Invalid Reaction Time";
    return milliseconds < 60000
        ? (milliseconds / 1000).toFixed(3) + " seconds"
        : `${Math.floor(milliseconds / 60000)} minutes and ${(milliseconds % 60000 / 1000).toFixed(3)} seconds`;
}

function calibrateFSRsForPlayer(player, side, fsrDataArray) {
    const fsrParts = ["toe", "middle", "heel"];
    calibrationData[player][side] = {};

    fsrParts.forEach((part, index) => {
        const fsrKey = `FSR${index}`;
        const allValues = fsrDataArray.map(data => Number(data[fsrKey]) || 0).filter(value => !isNaN(value));

        if (allValues.length === 0) {
            console.warn(`No valid calibration data for ${player} ${side} ${part}`);
            calibrationData[player][side][part] = { baseline: 0, threshold: 10 };
            return;
        }

        const baseline = calculateAverage(allValues);
        calibrationData[player][side][part] = {
            baseline,
            threshold: baseline + 10,
        };

        console.log(`${player} ${side} ${part} Calibration: Baseline: ${baseline}, Threshold: ${baseline + 10}`);
    });

    console.log(`Final Calibration Data for ${player} ${side}:`, calibrationData[player][side]);
}



function startCalibration() {
    if (isCalibrating) {
        document.querySelector('#CalibrationStatus').textContent = "Calibration is already in progress!";
        return;
    }

    isCalibrating = true;
    document.querySelector('#CalibrationStatus').textContent = "Starting calibration. Please stand still...";

    let fsrDataCollection = { player1: { left: [], right: [] }, player2: { left: [], right: [] } };
    const interval = setInterval(() => {
    //     const simulatedFSRData = { toe: Math.random() * 100, middle: Math.random() * 100, heel: Math.random() * 100 };
    //     fsrDataCollection.player1.left.push(simulatedFSRData);
    //     fsrDataCollection.player1.right.push(simulatedFSRData);
    //     fsrDataCollection.player2.left.push(simulatedFSRData);
    //     fsrDataCollection.player2.right.push(simulatedFSRData);
    }, 100);

    setTimeout(() => {
        clearInterval(interval);
        calibrateFSRsForPlayer("player1", "left", fsrDataCollection.player1.left);
        calibrateFSRsForPlayer("player1", "right", fsrDataCollection.player1.right);
        calibrateFSRsForPlayer("player2", "left", fsrDataCollection.player2.left);
        calibrateFSRsForPlayer("player2", "right", fsrDataCollection.player2.right);

        isCalibrating = false;
        document.querySelector('#CalibrationStatus').textContent = "Calibration complete for all players!";
        document.querySelector('#start').style.display = "block";
        document.querySelector('#startCalibrationButton').style.display = "none";
        document.querySelector('.intro').style.display = "block";
    }, calibrationDuration);
}

/**
 * Game
 */
const gameState = {
    currentTarget: null,
    targetHit: false,
    playerScores: { player1: 0, player2: 0 },
    roundCount: 0,
    maxRounds: 15,
    isGameActive: false,
    targetAssignedTime: null,
};

function detectPressedFSRWithCalibration(player, side, fsrData) {
    const calibrated = calibrationData[player]?.[side];
    if (!calibrated) {
        console.warn(`Calibration data not found for ${player} ${side}`);
        return null;
    }

    const fsrMapping = { FSR0: "toe", FSR1: "middle", FSR2: "heel" };
    let detectedTarget = null;

    for (const [fsrKey, part] of Object.entries(fsrMapping)) {
        const fsrValue = Number(fsrData[fsrKey]) || 0;
        const threshold = calibrated[part]?.threshold;

        console.log(`FSR Data (${part}): ${fsrValue}, Threshold: ${threshold}`);

        if (threshold !== undefined && fsrValue > threshold) {
            detectedTarget = `${side}${part.charAt(0).toUpperCase()}${part.slice(1)}`;
            console.log(`Detected Press: ${detectedTarget}`);

            // // Play the corresponding sound effect
            // const soundEffect = soundEffects[side]?.[part];
            // if (soundEffect) {
            //     soundEffect.currentTime = 0;
            //     soundEffect.play().catch((error) => {
            //         console.error(`Error playing sound for ${side} ${part}:`, error);
            //     });
            // }

            break;
        }
    }

    if (!detectedTarget) {
        console.warn(`No press detected for ${player} ${side}. FSR values:`, fsrData);
    }

    return detectedTarget;
}

const targets = {
    leftToe: document.getElementById("targetLeftToe"),
    leftMiddle: document.getElementById("targetLeftMiddle"),
    leftHeel: document.getElementById("targetLeftHeel"),
    rightToe: document.getElementById("targetRightToe"),
    rightMiddle: document.getElementById("targetRightMiddle"),
    rightHeel: document.getElementById("targetRightHeel"),
};

const targetSounds = {
    leftToe: new Audio('/static/730A.mp3'),
    leftMiddle: new Audio('/static/730C3.mp3'),
    leftHeel: new Audio('/static/730D.mp3'),
    rightToe: new Audio('/static/730D3.mp3'),
    rightMiddle: new Audio('/static/730F.mp3'),
    rightHeel: new Audio('/static/730G2.mp3'),
};

Object.values(targetSounds).forEach(sound => {
    sound.volume = 1; // Adjust to desired volume level
});

function showTarget(target) {
    // First, hide all target markers
    Object.values(targets).forEach((marker) => {
        if (marker) {
            marker.style.display = "none"; // Ensure no marker is visible initially
        }
    });

    // Show the marker for the current target
    const targetMarker = targets[target];
    if (targetMarker) {
        targetMarker.style.display = "block"; // Make the target marker visible
        console.log(`Target ${target} is now visible.`);

        // Play the corresponding sound
        const targetSound = targetSounds[target];
        if (targetSound) {
            targetSound.currentTime = 0; // Reset audio playback
            targetSound.play().catch((error) => {
                console.error(`Error playing sound for ${target}:`, error);
            });
        } else {
            console.warn(`Sound not found for target: ${target}`);
        }
    } else {
        console.warn(`Target marker for ${target} not found.`);
    }
}

function startGame() {
    backgroundMusic.play();
    console.log("Game starting soon...");
    
    // Hide irrelevant sections
    document.querySelector("#calibrationSection").style.display = "none";
    document.querySelector("#gameSection").style.display = "block";

    // Countdown container in the UI
    const countdownContainer = document.querySelector("#countdown");
    countdownContainer.style.display = "block";

    let countdownValue = 3; // Countdown from 3 seconds
    countdownContainer.textContent = countdownValue;

    const countdownInterval = setInterval(() => {
        countdownValue -= 1;
        if (countdownValue > 0) {
            countdownContainer.textContent = countdownValue;
        } else {
            clearInterval(countdownInterval);
            countdownContainer.style.display = "none";
            console.log("Game started!");
            
            // Initialize game state
            gameState.isGameActive = true;
            gameState.roundCount = 0;
            gameState.playerScores = { player1: 0, player2: 0 };
            gameState.targetHit = false;

            randomizeTarget(); // Set the first target

            // Run the game loop
            const gameInterval = setInterval(() => {
                if (!gameState.isGameActive) {
                    clearInterval(gameInterval);
                    return;
                }

                randomizeTarget();
            }, 800); // Target changes time
        }
    }, 800);
}

const playerSounds = {
    player1: new Audio('/static/wow.m4a'), // Replace with the path to sound1
    player2: new Audio('/static/yay.m4a')  // Replace with the path to sound2
};

// Set volume for each player's sound
playerSounds.player1.volume = 0.5;
playerSounds.player2.volume = 0.5;

function handlePress(player, side, pressedPart) {
    console.log(`Comparing pressedPart: ${pressedPart} with currentTarget: ${gameState.currentTarget}`);
    if (!gameState.isGameActive) {
        console.warn("Game is not active.");
        return;
    }

    if (!gameState.currentTarget) {
        console.warn("No target is currently active.");
        return;
    }
    
    console.log(`Player ${player} pressed ${pressedPart}, Target: ${gameState.currentTarget}`);

    if (pressedPart === gameState.currentTarget) {
        if (!gameState.targetHit) {
            gameState.playerScores[player] += 1;
            gameState.targetHit = true;

            // Play the corresponding player's sound
            const playerSound = playerSounds[player];
            if (playerSound) {
                playerSound.currentTime = 0; // Reset playback
                playerSound.play().catch((error) => {
                    console.error(`Error playing sound for ${player}:`, error);
                });
            }

            updateScores();

            console.log(`Player ${player} scored! Current Score:`, gameState.playerScores);

            if (gameState.playerScores[player] >= gameState.maxRounds) {
                endGame(player);
            } else {
                setTimeout(() => {
                    gameState.targetHit = false; // Reset target hit
                    console.log("Target hit reset. Ready for the next target.");
                }, 500);
            }
        } else {
            console.warn(`Player ${player} already hit the target.`);
        }
    } else {
        console.warn(`Wrong target pressed! Player ${player} pressed ${pressedPart}, Target: ${gameState.currentTarget}`);
    }
}

function randomizeTarget() {
    const possibleTargets = ["leftToe", "leftMiddle", "leftHeel", "rightToe", "rightMiddle", "rightHeel"];
    const randomIndex = Math.floor(Math.random() * possibleTargets.length);
    gameState.currentTarget = possibleTargets[randomIndex];
    gameState.targetAssignedTime = Date.now();

    showTarget(gameState.currentTarget); // Update the UI with the new target
    console.log(`New Target Assigned: ${gameState.currentTarget}`);
} 


function endGame(winningPlayer) {
    console.log(`Game over! Player ${winningPlayer} wins with ${gameState.playerScores[winningPlayer]} points.`);
    backgroundMusic.pause();
    gameState.isGameActive = false;
    gameState.currentTarget = null;
    showTarget(null); // Hide the target marker

    // Get the game-over container
    const gameOverContainer = document.getElementById("gameOverContainer");
    if (!gameOverContainer) {
        console.error("Game Over Container not found. Ensure it's defined in the HTML.");
        return;
    }

    // Hide insole containers
    document.querySelectorAll(".insole-container").forEach(container => {
        container.style.display = "none";
    });

    // Show the game-over message
    gameOverContainer.style.display = "block";
    gameOverContainer.innerHTML = `
        <h1>Game Over!</h1>
        <h1>Winner: ${winningPlayer} with ${gameState.playerScores[winningPlayer]} points</h1>
        <button id="restartGameButton" class="button">
            Restart Game
        </button>
    `;
    console.log("Game Over Container style updated to block.");

    // Add an event listener for restarting the game
    const restartButton = document.getElementById("restartGameButton");
    if (restartButton) {
        restartButton.addEventListener("click", restartGame);
        console.log("Restart button event listener added.");
    } else {
        console.error("Restart button not found.");
    }

    // Hide other elements
    const scoreBars = document.querySelector(".score-bars");
    if (scoreBars) scoreBars.style.display = "none";

    const startGameButton = document.querySelector("#startGameButton1");
    if (startGameButton) startGameButton.style.display = "none";

    const calibrationSection = document.querySelector("#calibrationSection");
    if (calibrationSection) calibrationSection.style.display = "none";

    const targetMessage = document.querySelector("#targetMessage");
    if (targetMessage) targetMessage.textContent = "No target.";

    const reactionTimeMessage = document.querySelector("#reactionTimeMessage");
    if (reactionTimeMessage) reactionTimeMessage.textContent = "";

    console.log("End game UI displayed.");
}

function restartGame() {
    console.log("Restarting game...");

    // Reset game state
    gameState.isGameActive = false;
    gameState.currentTarget = null;
    gameState.targetHit = false;
    gameState.playerScores = { player1: 0, player2: 0 };
    gameState.roundCount = 0;

    // Reset the game-over UI
    const gameOverContainer = document.getElementById("gameOverContainer");
    if (gameOverContainer) {
        gameOverContainer.style.display = "none";
        gameOverContainer.innerHTML = ""; // Clear the content
    }

    // Reset the background color
    document.body.style.backgroundColor = "#FFFFFF"; // Default to white

    // Reset other UI elements
    const scoreBars = document.querySelector(".score-bars");
    if (scoreBars) scoreBars.style.display = "block";

    const startGameButton = document.querySelector("#startGameButton1");
    if (startGameButton) startGameButton.style.display = "block";

    const calibrationSection = document.querySelector("#calibrationSection");
    if (calibrationSection) calibrationSection.style.display = "block";

    const targetMessage = document.querySelector("#targetMessage");
    if (targetMessage) targetMessage.textContent = "";

    const reactionTimeMessage = document.querySelector("#reactionTimeMessage");
    if (reactionTimeMessage) reactionTimeMessage.textContent = "";

    // Reset the score display
    updateScores();

    console.log("Game reset. Ready to start again.");
}



// --- Update Score Display ---
function updateScores() {
    // Select the score elements
    const player1ScoreElement = document.querySelector("#player1-score");
    const player2ScoreElement = document.querySelector("#player2-score");

    // Ensure the elements exist
    if (!player1ScoreElement || !player2ScoreElement) {
        console.error("Score elements not found in the DOM. Check your HTML structure.");
        return;
    }

    // Update the score in the UI
    player1ScoreElement.textContent = gameState.playerScores.player1;
    player2ScoreElement.textContent = gameState.playerScores.player2;

    console.log("Scores updated in the UI. Player 1:", gameState.playerScores.player1, "Player 2:", gameState.playerScores.player2);
}

window.onload = () => {
    console.log("Initializing websocket");
    initializeAllWebSockets();

    // Set up other event listeners
    document.querySelector('#startGameButton1').addEventListener("click", () => {
        console.log("#startGameButton1 pressed");
        reactiongame = true;
        document.querySelector('#calibrationwording').style.display = "block";
        document.querySelector('#startCalibrationButton').style.display = "block";
        document.querySelector('#optionswording').style.display = "none";
        document.querySelector('.buttonGroup').style.display = "none";
        document.querySelector('.Monster').style.display = "none";
    });

    document.querySelector('#startGameButton2').addEventListener("click", () => {
        console.log("#startGameButton2 pressed");
    
        // Trigger the file input
        const fileInput = document.getElementById('fileInput');
        fileInput.click();
    
        // Handle file selection
        fileInput.addEventListener('change', (event) => {
            const file = event.target.files[0];
            if (file) {
                console.log(`File selected: ${file.name}`);
    
                // Create a URL for the selected file
                const imageURL = URL.createObjectURL(file);
    
                // Display the image
                const imagePreview = document.getElementById('imagePreview');
                imagePreview.src = imageURL;
                imagePreview.style.display = "block"; // Make the image visible
    
                console.log(`Image preview updated with ${file.name}`);
            } else {
                console.log("No file selected.");
            }
        });
    });
    
    
    document.querySelector('#startCalibrationButton').addEventListener("click", () => startCalibration());

    document.querySelector('#start').addEventListener("click", () => {
        document.querySelector('#calibrationSection').style.display = "none";
        document.querySelector('#gameSection').style.display = "block";
        startGame();
    });
};

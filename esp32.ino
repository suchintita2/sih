#include <Arduino.h>

#define EYE_OPEN 1
#define EYE_CLOSED 0

void setup() {
    Serial.begin(115200);  // Serial communication with Arduino
}

void loop() {
    int eyeStatus = detectEAR();  // Call function to check EAR
    Serial.println(eyeStatus);    // Send eye status to Arduino
    delay(100);  // Adjust delay as needed
}

// Mock function for EAR detection (replace with actual OpenCV processing)
int detectEAR() {
    // Placeholder: Replace with real EAR detection logic
    float EAR_value = 0.2;  // Example value (adjust based on threshold)

    if (EAR_value < 0.25) {  // Example threshold for eye closure
        return EYE_CLOSED;
    } else {
        return EYE_OPEN;
    }
}
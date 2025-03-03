#include <WiFi.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>

#define IR_SENSOR_PIN 34  // Adjust as per your connection
#define BUZZER_PIN 5       // Adjust as per your connection

const char* ssid = "your_SSID";     
const char* password = "your_PASSWORD";
const char* serverURL = "http://your-server.com/alert";  // Replace with actual server URL

SoftwareSerial sim800(16, 17); // RX, TX pins for SIM800L (Connect as per your board)

void setup() {
    Serial.begin(115200);
    sim800.begin(9600);
    pinMode(IR_SENSOR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");

    int timeout = 10;  // Try for 10 seconds
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
        delay(1000);
        Serial.print(".");
        timeout--;
    }

    if (Wi-Fi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to Wi-Fi");
    } else {
        Serial.println("\nWi-Fi failed, will use GSM if needed.");
    }
}

void loop() {
    static unsigned long startTime = 0;
    bool eyesClosed = digitalRead(IR_SENSOR_PIN) == LOW;  // Adjust based on sensor logic

    if (eyesClosed) {
        if (startTime == 0) {
            startTime = millis();  // Start timing
        } else if (millis() - startTime >= 2000) {  // 2 seconds
            digitalWrite(BUZZER_PIN, HIGH);
            Serial.println("Microsleep detected! Sending alert...");

            if (WiFi.status() == WL_CONNECTED) {
                sendAlertWiFi();
            } else {
                sendAlertGSM();
            }

            delay(5000);  // Prevent multiple alerts in a short time
            digitalWrite(BUZZER_PIN, LOW);
            startTime = 0;  // Reset timer
        }
    } else {
        startTime = 0;  // Reset if eyes open
    }
}

void sendAlertWiFi() {
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");
    
    String postData = "{\"message\": \"Driver is sleeping!\"}";
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
        Serial.println("Wi-Fi Alert Sent!");
    } else {
        Serial.println("Wi-Fi Alert Failed! Switching to GSM.");
        sendAlertGSM();
    }

    http.end();
}

void sendAlertGSM() {
    Serial.println("Sending SMS Alert...");
    sim800.println("AT+CMGF=1");  // Set SMS mode
    delay(100);
    sim800.println("AT+CMGS=\"+91xxxxxxxxxx\"");  // Replace with actual phone number
    delay(100);
    sim800.print("Driver is sleeping! Immediate action needed.");
    delay(100);
    sim800.write(26);  // End SMS with Ctrl+Z
    delay(5000);
    Serial.println("SMS Sent!");
}
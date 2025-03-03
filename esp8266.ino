#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define TRIGGER_PIN 5  // Input from Arduino

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

String smsAPI = "https://www.fast2sms.com/dev/bulkV2";
String apiKey = "your_api_key";  
String phoneNumber = "91XXXXXXXXXX"; 

void setup() {
    pinMode(TRIGGER_PIN, INPUT);
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi!");
}

void loop() {
    if (digitalRead(TRIGGER_PIN) == HIGH) {
        sendSMS();
        delay(5000);  // Prevent spamming SMS
    }
}

// Function to send SMS
void sendSMS() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String postData = "authorization=" + apiKey + "&route=q&message=Microsleep detected!&language=english&numbers=" + phoneNumber;

        http.begin(smsAPI);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpResponseCode = http.POST(postData);
        if (httpResponseCode == 200) {
            Serial.println("SMS Sent Successfully!");
        } else {
            Serial.print("Error sending SMS: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    } else {
        Serial.println("Wi-Fi not connected. SMS not sent.");
    }
}
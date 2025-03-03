#define IR_SENSOR_PIN 2  
#define BUZZER_PIN 3    
#define ESP8266_TRIGGER_PIN 4  // Pin to send signal to ESP8266

unsigned long eyeCloseStart = 0;
bool isEyeClosed = false;

void setup() {
    pinMode(IR_SENSOR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(ESP8266_TRIGGER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.begin(115200);  // Serial for ESP32
}

void loop() {
    int irSensorValue = digitalRead(IR_SENSOR_PIN);
    int esp32EyeStatus = Serial.parseInt();  // Read eye status from ESP32

    // If IR sensor or ESP32 detects closed eyes
    if (irSensorValue == LOW || esp32EyeStatus == 0) {
        if (!isEyeClosed) {
            eyeCloseStart = millis();
            isEyeClosed = true;
        }

        if (millis() - eyeCloseStart >= 2000) {
            digitalWrite(BUZZER_PIN, HIGH);  // Activate buzzer
            digitalWrite(ESP8266_TRIGGER_PIN, HIGH); // Notify ESP8266 to send SMS
        }
    } else {
        isEyeClosed = false;
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(ESP8266_TRIGGER_PIN, LOW);
    }
}
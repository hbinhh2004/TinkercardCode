#include <LiquidCrystal.h>

// LCD Configuration: RS=12, E=11, D4=5, D5=4, D6=3, D7=2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Define pins
int button1 = 8;  // Pause button
int button2 = 9;  // Start button
int ledPin = 10;  // LED pin
int buzzerPin = 7; // Buzzer pin

// Time variables
int u_hour = 0, d_hour = 0;
int u_minute = 0, d_minute = 0;  // Default to 1 minute
int u_second = 30, d_second = 0;
long previousMillis = 0;
bool running = false;
bool alarmTriggered = false;

void setup() {
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);

    lcd.begin(16, 2);
    lcd.clear();
    display_time();
}

void loop() {
    handle_buttons();
    if (running) {
        update_seconds();
        display_time();
    }
    trigger_alarm();
}

void update_seconds() {
    if (millis() - previousMillis >= 1000) {
        previousMillis = millis();
        if (d_hour == 0 && u_hour == 0 && d_minute == 0 && u_minute == 0 && d_second == 0 && u_second == 0) {
            running = false;
            alarmTriggered = true;
        } else {
            countdown();
        }
    }
}

void countdown() {
    if (u_second > 0) {
        u_second--;
    } else {
        u_second = 9;
        if (d_second > 0) {
            d_second--;
        } else {
            d_second = 5;
            if (u_minute > 0) {
                u_minute--;
            } else {
                u_minute = 9;
                if (d_minute > 0) {
                    d_minute--;
                } else {
                    d_minute = 5;
                    if (u_hour > 0) {
                        u_hour--;
                    } else {
                        if (d_hour > 0) {
                            d_hour--;
                            u_hour = 9;
                        }
                    }
                }
            }
        }
    }
}

void display_time() {
    lcd.setCursor(0, 0);
    lcd.print("Countdown Timer ");
    lcd.setCursor(3, 1);
    lcd.print(d_hour);
    lcd.print(u_hour);
    lcd.print(":");
    lcd.print(d_minute);
    lcd.print(u_minute);
    lcd.print(":");
    lcd.print(d_second);
    lcd.print(u_second);
}

void handle_buttons() {
    if (digitalRead(button1) == LOW) { // Pause timer
        running = false;
        delay(300);
    }
    if (digitalRead(button2) == LOW) { // Start timer
        running = true;
        alarmTriggered = false;
        start_alarm();
        delay(300);
    }
}

void start_alarm() {
    for (int i = 0; i < 2; i++) {
        tone(buzzerPin, 1500);
        delay(200);
        noTone(buzzerPin);
        delay(200);
    }
}

void trigger_alarm() {
    if (alarmTriggered) {
        for (int i = 0; i < 5; i++) {
            digitalWrite(ledPin, HIGH);
            tone(buzzerPin, 1000);
            delay(500);
            noTone(buzzerPin);
            digitalWrite(ledPin, LOW);
            delay(500);
        }
        alarmTriggered = false;
    }
}
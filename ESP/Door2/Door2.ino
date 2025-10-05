// #include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h> 
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>  // Add Keypad library

// Wi-Fi credentials
const char* ssid = "WIFI_SSID";
const char* password = "YOUR_Pass";
// API URL to your backend
const char* host = "www.example.com"; 
const int httpPort = PORT;  

WiFiClient client;

// Motor control pins
int motorPin1 = 13;  // IN1 - Motor Direction 
int motorPin2 = 12;  // IN2 - Motor Direction 
int motorPWM = 14;   // PWM - Speed Control 

// Buzzer Pin
#define BUZZER_PIN 4 // ESP32 pin GPIO4 for the Buzzer

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD address 0x27, 16 columns, 2 rows

// Keypad Setup
const byte ROW_NUM    = 4; // four rows
const byte COLUMN_NUM = 4; // four columns

// Define the Keypad layout
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Define the row and column pins
byte pin_rows[ROW_NUM] = {27, 26, 33, 32}; 
byte pin_column[COLUMN_NUM] = {23, 15, 18, 19}; 
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);
String correctPassword = "1234";  
String enteredPassword = "";     
int unauthorizedAttempts = 0; 
/********** Keypoard Data ******************/
int KeypoardRead = 0;  
String KeypoardID = "67e54bf904f8393aa15dcc4f";  
void resetPasswordEntry() {
  enteredPassword = "";
  lcd.clear();       
  lcd.setCursor(0, 0);
  lcd.print("****Welcome*****");  
}

void showMessage(String message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  delay(2000);
}
void setup() {
  Serial.begin(115200);
  // Wi-Fi Setup
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Motor Setup
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  stopDoor();

  // Buzzer Setup
  pinMode(BUZZER_PIN, OUTPUT);  

  // LCD Setup
  lcd.init();
  delay(100);  
  lcd.backlight();   
  lcd.setCursor(0, 0);
  lcd.print("****Welcome*****");
  delay(2000); 
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    // Add the pressed key to the password
    enteredPassword += key;
    KeypoardRead = 0;
    Keypoard(KeypoardRead);
    if (enteredPassword.length() == correctPassword.length()) {
      if (enteredPassword == correctPassword) {
        KeypoardRead = 1;
        Keypoard(KeypoardRead);
        showMessage("***AUTHORIZED***");
        showMessage("Door Opening...");
        openDoor();  
        delay(2000);
        closeDoor();
        resetPasswordEntry();
        lcd.clear();
        lcd.setCursor(0, 0);
        unauthorizedAttempts = 0; 
      } else 
      {
        showMessage("Wrong Password");
        unauthorizedAttempts++;
        if (unauthorizedAttempts > 2) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("UnAuthorized...");
        for (int i = 0; i < 5; i++) {
          digitalWrite(BUZZER_PIN, HIGH);  
          delay(1000);                    
          digitalWrite(BUZZER_PIN, LOW);   
          delay(1000);                    
        }
        // delay(5000); 
        unauthorizedAttempts = 0;
        resetPasswordEntry();
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Try again...");
          delay(1000); // Display for a short time
          resetPasswordEntry();
        }
      }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Entered: ");
    lcd.setCursor(0, 1);
    for (int i = 0; i < enteredPassword.length(); i++) {
      lcd.print('*');
    }
  }
  if (enteredPassword.length() == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("****Welcome*****");
  }
}

void openDoor() {
  digitalWrite(motorPin1, LOW);   
  digitalWrite(motorPin2, HIGH);   
  analogWrite(motorPWM, 255);       
  delay(500);  
  stopDoor();  
}
void closeDoor() {
  digitalWrite(motorPin1, HIGH);   
  digitalWrite(motorPin2, LOW);    
  analogWrite(motorPWM, 255);      
  delay(500);
  stopDoor(); 
}

void stopDoor() {
  digitalWrite(motorPin1, LOW);  
  digitalWrite(motorPin2, LOW);    
}
void Keypoard(int read) {
  String payload = "{\"value\": " + String(read) + "}"; 
  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, sending data...");
    client.println("POST /sensors/" + KeypoardID + " HTTP/1.1");  
    client.println("Host: " + String(host));
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.print(payload);
    while (client.available()) {
      String response = client.readString();
      Serial.println(response);
    }
    client.stop();
    Serial.println("Door Status sent to server.");
  } else {
    Serial.println("Connection failed!");
  }
}
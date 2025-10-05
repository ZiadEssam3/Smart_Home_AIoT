#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Include the I2C LCD library

// Replace with your network credentials
const char *ssid = "WIFI_SSID";
const char *password = "YOUR_Pass";
#define WIFI_PIN D8
// API URL to your backend
const char *host = "www.example.com";
const int httpPort = PORT;
WiFiClient client;
/****************************** Rain Sensor Setup ********************************************/
#define RAIN_SENSOR_PIN D6
#define IR_LED_PIN D1
int RainStatus;
String RainSensorId = "67dc2bcc6343587ee09e49bf";
/***************************** Motor Control Setup *********************************************/
int motorPin1 = D4; // IN1 - Motor Direction (GPIO pin)
int motorPin2 = D3; // IN2 - Motor Direction (GPIO pin)
int motorPWM = D0;  // PWM - Speed Control (GPIO pin)
bool isDoorOpen = false;
/****************************** DHT11 Sensor Setup ********************************************/
#define DHT11_SENSOR_PIN D5
#define Type DHT11
String DHTHumiditySensorId = "67dc351d570803ac0902725b";
String DHTTemperatureSensorId = "67dc391f570803ac0902b47b";
DHT HT(DHT11_SENSOR_PIN, Type);
float humidity, tempC;
/****************************** Pulse Sensor Setup ********************************************/
#define PULSE_SENSOR_PIN A0
String PulseSensorId = "67dc5f0d55f5ced80bbe0fba";
int heartRate;
/****************************** LCD Setup ********************************************/
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16x2 LCD
/****************************** Heart Rate Setup ********************************************/
// Placeholder value for heart rate
void setup()
{
  Serial.begin(9600);
  pinMode(WIFI_PIN, OUTPUT);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  digitalWrite(WIFI_PIN, HIGH);
  /********* Rain Sensor Configrations ************/
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(IR_LED_PIN, OUTPUT);
  /****************** Set motor *********************/
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  stopDoor();
  /******** DHT 11 Sensor ************************/
  HT.begin();
  /******** Initialize I2C (Wire.h) and LCD *******************/
  Wire.begin(D2, D1);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("**System Ready**");
}
void loop()
{
  /*************************** Rain Sensor ***********************/
  RainStatus = digitalRead(RAIN_SENSOR_PIN);
  if (RainStatus == HIGH)
  { // No rain detected
    Serial.println("No Rain");
    if (!isDoorOpen)
    {                    // If the door is closed
      openDoor();        // Open the door
      isDoorOpen = true; // Update the state to open
      Serial.println("Open The Door...");
    }
    else
    {
      Serial.println("Door is already open.");
    }
  }
  else
  { // Rain detected
    Serial.println("Rain");
    if (isDoorOpen)
    {                     // If the door is open
      closeDoor();        // Close the door
      isDoorOpen = false; // Update the state to closed
      Serial.println("Close The Door...");
    }
    else
    {
      Serial.println("Door is already closed.");
    }
  }
  /*************************** DHT11 Sensor *****************************/
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
  if (isnan(humidity) || isnan(tempC))
  {
    Serial.println("Failed to read from DHT sensor!");
    delay(250);
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C ");
  /*************************** PusleRate Sensor *****************************/
  heartRate = analogRead(PULSE_SENSOR_PIN);
  Serial.println(heartRate);
  /*************************** LCD Display *****************************/
  lcd.clear();
  lcd.setCursor(0, 0); // Set cursor to the first line
  lcd.print("T:");
  lcd.print(tempC);
  lcd.print(" C H:");
  lcd.print((int)humidity);
  lcd.print("%");
  lcd.setCursor(0, 1); // Set cursor to the second line
  lcd.print("HR: ");
  lcd.print(heartRate);
  lcd.print(" BPM");
  /************************  Send The Values ****************************/
  sendRainSensorData(RainStatus);
  sendDHTHumiditySensorData(humidity);
  sendDHTTemperatureSensorData(tempC);
  sendPulseRateData(heartRate);
  delay(2000); // Delay before the next reading
}

/************************ Send Data Functions ***************************/
void sendRainSensorData(int rainStatus)
{
  String payload = "{\"value\": " + String(rainStatus) + "}";
  if (client.connect(host, httpPort))
  {
    client.println("POST /sensors/" + RainSensorId + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.print(payload);
    client.stop();
  }
  else
  {
    Serial.println("Connection failed!");
  }
}
void sendDHTHumiditySensorData(float humidity)
{
  String payload = "{\"value\": " + String(humidity) + "}";
  if (client.connect(host, httpPort))
  {
    client.println("POST /sensors/" + DHTHumiditySensorId + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.print(payload);
    client.stop();
  }
  else
  {
    Serial.println("Connection failed!");
  }
}
void sendDHTTemperatureSensorData(float temp)
{
  String payload = "{\"value\": " + String(temp) + "}";
  if (client.connect(host, httpPort))
  {
    client.println("POST /sensors/" + DHTTemperatureSensorId + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.print(payload);
    client.stop();
  }
  else
  {
    Serial.println("Connection failed!");
  }
}
void sendPulseRateData(int pulseRate)
{
  // Create a JSON payload with the pulse rate value
  String payload = "{\"value\": " + String(pulseRate) + "}";

  // Connect to the server
  if (client.connect(host, httpPort))
  {
    Serial.println("Connected to backend, sending Pulse Rate data...");

    // Send HTTP POST request with Pulse Rate data
    client.println("POST /sensors/" + PulseSensorId + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.print(payload);

    // Wait for the response from the server
    while (client.available())
    {
      String response = client.readString();
      Serial.println(response);
    }

    client.stop();
    Serial.println("Pulse Rate data sent to server.");
  }
  else
  {
    Serial.println("Connection failed!");
  }
}
/******************** Utility Functions ********************************/
// Function to open the door
void openDoor()
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(motorPWM, 255);
  delay(50);
  stopDoor();
}
// Function to close the door
void closeDoor()
{
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(motorPWM, 255);
  delay(50);
  stopDoor();
}
// Function to stop the motor
void stopDoor()
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  // analogWrite(motorPWM, 0);
}
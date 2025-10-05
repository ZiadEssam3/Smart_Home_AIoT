#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ArduinoJson.h>
// Wi-Fi credentials
const char* ssid = "WIFI_SSID";
const char* password = "YOUR_Pass";
// API URL to your backend
const char* host = "www.example.com"; 
const int httpPort = PORT;  

WiFiClient client;
#define WIFI_PIN D8


/********** water detection level sensor ************/
#define WATER_DETECTION_SENSOR_PIN D0 
String WaterDetectionId = "67c837050f2f99e4aae6f878";  
int WaterLevelThreshold = 500;
int WaterLevelValue = 0;
int percentage;
/********** Flame sensor ***************************/
#define Flame_SENSOR_PIN D1  
int FlameStatus;  
String FlameSensorId = "67c841c80f2f99e4aaedf6a0";  
/************* MQ-7 Sensor ************************/
#define MQ7_SENSOR_PIN D7
int mq7status;
String MQ7SensorId = "67c84adf0f2f99e4aa4a1403";  
String MQ7Name = "CO";
/************* MQ-5 Sensor ************************/
#define MQ5_SENSOR_PIN D5
int mq5status;
String MQ5SensorId = "67c854aa0f2f99e4aadebe15";  
String MQ5Name = "Methane";
/*************** Microphone Sensor **************/
#define MIC_PIN D6
#define MIC_LED_PIN 19  
// threshold value for detecting sound
int SOUND_THRESHOLD = 500;
// Number of samples for averaging
#define SAMPLE_SIZE 10  
int MicValue = 0;
long sum = 0;
int averageValue = 0;
/*** Detection Indecators ***/ 
#define INDICATOR_PIN D4


void setup() {
  Serial.begin(115200);
  pinMode(WIFI_PIN,OUTPUT);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  digitalWrite(WIFI_PIN,HIGH);
  /************ Water Level Sensor *************/
  pinMode(WATER_DETECTION_SENSOR_PIN,INPUT);
  /************ Flame Sensor *******************/
  pinMode(Flame_SENSOR_PIN,INPUT);
  /************** MQ-7 Sensor ******************/
  pinMode(MQ7_SENSOR_PIN,INPUT);
  /************** MQ-5 Sensor ******************/
  pinMode(MQ5_SENSOR_PIN,INPUT);
  /************** Microphone Sensor ***********/
  pinMode(MIC_PIN, INPUT);
  pinMode(MIC_LED_PIN,OUTPUT);
  /************** INDEICATORS *******************/
  pinMode(INDICATOR_PIN,OUTPUT);
}


void loop() {
  /************** WaterLevelValue **************/
  WaterLevelValue = analogRead(WATER_DETECTION_SENSOR_PIN);
  Serial.print("Water Level: ");
  Serial.println(WaterLevelValue);
  percentage = map(WaterLevelValue, 0, 3800, 0, 100);
  // Print the water level percentage
  Serial.print("Water Level: ");
  Serial.print(percentage);
  Serial.println(" %");
  delay(500);
  /*************** Flame Sensor ***************************/
  FlameStatus = digitalRead(Flame_SENSOR_PIN);
  if (FlameStatus == HIGH) {
    Serial.println("No Fire");
    digitalWrite(INDICATOR_PIN, LOW);
  } else if(FlameStatus == LOW) {
    Serial.println("There is Fire!!");
    digitalWrite(INDICATOR_PIN, HIGH);
    delay(300);
    digitalWrite(INDICATOR_PIN, LOW);
    delay(300);
  }
  /******************* MQ7 Sensor ******************************/
  int mq7status = digitalRead(MQ7_SENSOR_PIN);
  if (mq7status == HIGH) {
    Serial.println("No Carbon Detected");
    digitalWrite(INDICATOR_PIN, LOW);
  } else if(mq7status == LOW){
    Serial.println("Carbon Detected");
    digitalWrite(INDICATOR_PIN, HIGH);
    delay(300);
    digitalWrite(INDICATOR_PIN, LOW);
    delay(300);
  }
  /******************* MQ5 Sensor ******************************/
  int mq5status = digitalRead(MQ5_SENSOR_PIN);
  if (mq5status == HIGH) {
    Serial.println("No Mehtan Leaks");
    digitalWrite(INDICATOR_PIN, LOW);
  } else if(mq5status == LOW) {
    Serial.println("Methan Leaks");
    digitalWrite(INDICATOR_PIN, HIGH);
    delay(300);
    digitalWrite(INDICATOR_PIN, LOW);
    delay(300);
  }
  /*************** Microphone Sensor **************************/
  MicValue = analogRead(MIC_PIN);
  Serial.println(MicValue);
  if (MicValue > SOUND_THRESHOLD) {
    digitalWrite(MIC_LED_PIN, HIGH);
  } else {
    digitalWrite(MIC_LED_PIN, LOW);
  }
  /******** Send Sensors Data To The Backend **************/
  SendWaterLevelData(percentage);
  SendFlameSensorData(FlameStatus);
  sendMQ_XSensorData(mq7status,MQ7SensorId,MQ7Name);
  sendMQ_XSensorData(mq5status,MQ5SensorId,MQ5Name);
  // delay(500);
}

void SendWaterLevelData(int WaterLevelValue) {
  // Prepare the JSON payload with water level sensor data
  String payload = "{\"value\": " + String(WaterLevelValue) + "}";  
  // Connect to the server
  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, sending data...");
    // Send HTTP POST request with IR sensor data
    client.println("POST /sensors/" + WaterDetectionId + " HTTP/1.1");  
    client.println("Host: " + String(host));
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.print(payload);
    // Wait for the response from the server
    while (client.available()) {
      String response = client.readString();
      Serial.println(response);
    }
    client.stop();
    Serial.println("Water Level Data sent to server.");
  } else {
    Serial.println("Connection failed!");
  }
}
void SendFlameSensorData(int FlameStatus) {
  // Prepare the JSON payload with IR sensor data
   // Only send value (0 or 1)
  String payload = "{\"value\": " + String(FlameStatus) + "}"; 
  // Connect to the server
  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, sending data...");
    // Send HTTP POST request with IR sensor data
    // Use the sensor ID for this POST request
    client.println("POST /sensors/" + FlameSensorId + " HTTP/1.1");  
    client.println("Host: " + String(host));
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.print(payload);

    // Wait for the response from the server
    while (client.available()) {
      String response = client.readString();
      Serial.println(response);
    }
    client.stop();
    Serial.println("Flame sensor data sent to server.");
  } else {
    Serial.println("Connection failed!");
  }
}
void sendMQ_XSensorData(int mqStatus,String mqId,String mqName) {
  // Prepare the JSON payload with IR sensor data
  String payload = "{\"value\": " + String(mqStatus) + "}";  // Only send value (0 or 1)

  // Connect to the server
  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, sending data...");

    // Send HTTP POST request with IR sensor data
    client.println("POST /sensors/" + mqId + " HTTP/1.1");  // Use the sensor ID for this POST request
    client.println("Host: " + String(host));
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.print(payload);

    // Wait for the response from the server
    while (client.available()) {
      String response = client.readString();
      Serial.println(response);
    }
    client.stop();
    Serial.print("MQ ");
    Serial.print(mqName);
    Serial.println(" Data sent to server.");
  } else {
    Serial.println("Connection failed!");
  }
}
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>  // If using HTTPS, otherwise use WiFiClient
#include <Servo.h>
#include <ArduinoJson.h>
// Replace with your network credentials
const char* ssid = "WIFI_SSID";
const char* password = "YOUR_Pass";
// API URL to your backend
const char* host = "www.example.com"; 
const int httpPort = PORT;   
WiFiClient client;

/******************************IR Sensor Setup ********************************************/
// IR sensor setup (assuming it's connected to D1 pin)
#define IR_SENSOR_PIN D2  // Change this to the GPIO pin where your IR sensor is connected
#define IR_LED_PIN D1
// IR sensor status (0 for no detection, 1 for object detected)
int irStatus;  
// Sensor information
// Sensor ID, you should already have the sensor created on the backend
String sensorId = "6764933c8d23a26233f90d09";  // Set the sensor ID you want to update
String sensorType = "IR Sensor";  // We can just say it's an IR sensor
String location = "Living Room";  // Set your location
String pin = "D0";  // GPIO pin used for the IR sensor

// Soil Moisture sensor setup (assuming it's connected to D3 pin)
#define SOIL_MOISTURE_PIN A0                          // Use the analog pin where your soil moisture sensor is connected (e.g., A0 or D3)
int _moisture, moistureLevel; 
// String soilSensorId = "your-soil-sensor-id";     // Set the sensor ID for the soil moisture sensor
String soilSensorId = "6765a21127520115c5583f11";  // Set the sensor ID for the soil moisture sensor
String sensorTypeSoil = "soil moisture";          // Type of the sensor
String locationSoil = "garden";                  // Set the location where the sensor is placed
String pinSoil = "A0";                          // Analog pin used for the soil moisture sensor
#define SOIL_MOTOR_PIN D5


Servo myServo;         // Create a Servo object to control the servo
#define SERVO_PIN D0 // Pin for detecting "open" or "close" state (e.g., button)
String actuatorId = "676b15f5ef5632dcd4084dff";
String actuatorLocation = "garden";


// Servo myServoCam;
// #define SERVO_CAM_PIN D3
// String actuatorCamId = "67b9cfbccb4c8edccad05cd5";
// String actuatorCamLocation = "garden";

#define MQ2_SENSOR_PIN D3
int mq2status;
String MQ2sensorId = "67b9d6bbcb4c8edccad69a19";  
String MQ2sensorType = "MQ-2"; 
String MQ2location = "garden";  
String MQ2pin = "D3";



// Define ultrasonic sensor pins
const int triggerPin = D7;  // Pin to trigger the ultrasonic sensor
const int echoPin = D8;    // Pin to receive the echo from the ultrasonic sensor
const int WaterledPin = D6;    // Pin to control the LED
const int threshold = 3;  // Threshold value in cm
const int tankHeight = 8; // Height of the tank in cm

// Variables for ultrasonic sensor
String ultrasonicSensorId = "6776de1c221f3557a5145a3f";  // Set the ultrasonic sensor ID
String sensorTypeUltrasonic = "Ultrasonic";  // Type of the sensor (for water level)
String locationUltrasonic = "garden";  // Location where the sensor is placed




void setup() {
  // Start Serial communication for debugging
  Serial.begin(9600);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  /********* IR & IR_Led Configrations ************/
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(IR_LED_PIN, OUTPUT);
  /************* Servo_Door Configrations ********/
  // servo motor 1  
  myServo.attach(SERVO_PIN);  // Attach the servo to pin D2
  /************* Servo Camera Configrations *******/
  // myServoCam.attach(SERVO_CAM_PIN);  
  /********* SoilMoisture Configrations ************/
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(SOIL_MOTOR_PIN,OUTPUT);
  /************ MQ-2 Sensor ************************/
  pinMode(MQ2_SENSOR_PIN,INPUT);
  /********** Ultrasonic **************************/
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(WaterledPin, OUTPUT);
}


void loop() {
  /************************* Fetch Values From Backend *****************/
  /************************* Read The Values ***************************/ 
  
  // Read IR sensor value (0 or 1)
  /*************************** IR and IR_LED ***********************/ 
  // Read the IR sensor data
  irStatus = digitalRead(IR_SENSOR_PIN);  
  if (irStatus == HIGH) {
    Serial.println("No object detected");
    digitalWrite(IR_LED_PIN,LOW);
  } else {
    Serial.println("Object detected");
    digitalWrite(IR_LED_PIN,HIGH);
  }
  /******************* MQ2 Sensor ******************************/
  int mq2status = digitalRead(MQ2_SENSOR_PIN);
  if (mq2status == HIGH) {
    Serial.println("Some one Smoke");
  } else {
    Serial.println("No Smoking");
  }
  /************************** Servo_Door ************************/
  int actuatorAction = getActuatorAction(actuatorId); 
  Serial.print("ziad is state :");
  Serial.println(actuatorAction);
  //Act Based on the State of servo open or close 
  if (actuatorAction == 1){
    myServo.write(180);  // Move the servo to 90 degrees
  } else if (actuatorAction == 0){
    myServo.write(0);   // Move the servo to 0 degrees (close)
  }
  /************************** Servo_CAM ************************/
  /*int actuatorCamAction = getActuatorCAMAction(actuatorCamId); 
  Serial.print("CamServo state :");
  Serial.println(actuatorCamAction);
  // Act Based on the State of servo open or close 
  if (actuatorCamAction == 1){
    // Move the servo to 0 degrees
    myServo.write(0);  
  } else if (actuatorCamAction == 2){
    // Move the servo to 45 degrees
    myServo.write(45);   
  }
  else if (actuatorCamAction == 3){
    // Move the servo to 90 degrees
    myServo.write(90);   
  }
  else if (actuatorCamAction == 4){
    // Move the servo to 135 degrees
    myServo.write(135);   
  }
  else if (actuatorCamAction == 5){
    // Move the servo to 180 degrees
    myServo.write(180);   
  }*/

  /****************** SOIL_MOISTURE ****************************/
  // Read soil moisture sensor value (analog value from 0 to 1023)
  moistureLevel = analogRead(SOIL_MOISTURE_PIN);
  _moisture = (float(100 - ((moistureLevel) / 1023.0) * 100.0));
  // Display The Values
  Serial.print("Soil Moisture value: ");
  Serial.println(_moisture);
  if (_moisture < 20){
    digitalWrite(SOIL_MOTOR_PIN,HIGH);
  } 
  else if (_moisture >= 20){
    digitalWrite(SOIL_MOTOR_PIN,LOW);
  }
  /********************* Ultrasonic Water Level *************************/
  // Measure the water level (distance)
  long waterLevel = measureWaterLevel();
  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.println(" cm");
  // Control the LED based on the threshold value
  if (waterLevel < threshold) {
    // Turn on the LED if water level is less than the threshold
    digitalWrite(WaterledPin, HIGH); 
  } else {
    // Turn off the LED if water level is greater than or equal to the threshold
    digitalWrite(WaterledPin, LOW);  
  }

  
  /************************  Send The Values ****************************/
  // Send IR sensor data to backend using HTTP POST
  sendIRSensorData(irStatus);
  // Send the mq-2  data to backend
  sendMQ2SensorData(mq2status);
  // Send the soil moisture data to backend
  sendSoilMoistureData(_moisture);
  // Send water level data to the server
  sendWaterLevelData(waterLevel);

  // Delay before the next reading 
  delay(2000); 
}
void sendIRSensorData(int irStatus) {
  // Prepare the JSON payload with IR sensor data
  String payload = "{\"value\": " + String(irStatus) + "}";  // Only send value (0 or 1)

  // Connect to the server
  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, sending data...");

    // Send HTTP POST request with IR sensor data
    client.println("POST /sensors/" + sensorId + " HTTP/1.1");  // Use the sensor ID for this POST request
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
    Serial.println("IR sensor data sent to server.");
  } else {
    Serial.println("Connection failed!");
  }
}
void sendMQ2SensorData(int mq2Status) {
  // Prepare the JSON payload with IR sensor data
  String payload = "{\"value\": " + String(mq2Status) + "}";  // Only send value (0 or 1)

  // Connect to the server
  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, sending data...");

    // Send HTTP POST request with IR sensor data
    client.println("POST /sensors/" + MQ2sensorId + " HTTP/1.1");  // Use the sensor ID for this POST request
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
    Serial.println("MQ2 Data sent to server.");
  } else {
    Serial.println("Connection failed!");
  }
}
void sendSoilMoistureData(int soilMoistureValue) {
  // Prepare the JSON payload with soil moisture sensor data
  String payload = "{\"value\": " + String(soilMoistureValue) + "}";  // Send moisture value (0-1023)

  // Connect to the server
  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, sending data...");

    // Send HTTP POST request with soil moisture sensor data
    client.println("POST /sensors/" + soilSensorId + " HTTP/1.1");  // Use the soil moisture sensor ID for this POST request
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
    Serial.println("Soil moisture data sent to server.");
  } else {
    Serial.println("Connection failed!");
  }
}
int getActuatorAction(String actuatorId) {
  String url = "/actuators/" + actuatorId + "/latest";  // Endpoint to fetch the latest actuator action

  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, requesting actuator action...");

    // Send HTTP GET request to the backend to get the latest actuator action
    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Connection: close");
    client.println();

    // Wait for the response from the server
    String response = "";
    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        response += c;  // Append the response data
      }
    }

    // Print the entire response for debugging
    Serial.println("Response from server:");
    Serial.println(response);  // Full JSON response

    client.stop();
    Serial.println("Actuator action request complete.");

    // Now let's parse the JSON response to extract the actuator action
    int actionIndex = response.indexOf("\"action\":");
    if (actionIndex != -1) {
      int actionValueIndex = actionIndex + 10;  // Move index to the value after "action":
      if (response.substring(actionValueIndex, actionValueIndex + 4) == "OPEN") {
        Serial.println("Actuator OPEN (action = 1)");
        return 1;  // Actuator is OPEN
      } else if (response.substring(actionValueIndex, actionValueIndex + 5) == "CLOSE") {
        Serial.println("Actuator CLOSED (action = 0)");
        return 0;  // Actuator is CLOSED
      }
    }
  } else {
    Serial.println("Connection to backend failed!");
    return -1;  // Return -1 in case of failure to fetch the actuator action
  }

  return -1;  // Default return in case of error
}
int getActuatorCAMAction(String actuatorId) {
  String url = "/actuators/" + actuatorId + "/latest";  // Endpoint to fetch the latest actuator action

  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, requesting actuator action...");

    // Send HTTP GET request to the backend to get the latest actuator action
    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Connection: close");
    client.println();

    // Wait for the response from the server
    String response = "";
    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        response += c;  // Append the response data
      }
    }

    // Print the entire response for debugging
    Serial.println("Response from server:");
    Serial.println(response);  // Full JSON response

    client.stop();
    Serial.println("Actuator action request complete.");

    // Now let's parse the JSON response to extract the actuator action
    int actionIndex = response.indexOf("\"action\":");
    if (actionIndex != -1) {
      int actionValueIndex = actionIndex + 10;  // Move index to the value after "action":

      // Check for each of the new possible actions (CAM1, CAM2, CAM3, CAM4, CAM5)
      if (response.substring(actionValueIndex, actionValueIndex + 4) == "CAM1") {
        Serial.println("Actuator CAM1 (action = 1)");
        return 1;  // Action for CAM1
      } else if (response.substring(actionValueIndex, actionValueIndex + 4) == "CAM2") {
        Serial.println("Actuator CAM2 (action = 2)");
        return 2;  // Action for CAM2
      } else if (response.substring(actionValueIndex, actionValueIndex + 4) == "CAM3") {
        Serial.println("Actuator CAM3 (action = 3)");
        return 3;  // Action for CAM3
      } else if (response.substring(actionValueIndex, actionValueIndex + 4) == "CAM4") {
        Serial.println("Actuator CAM4 (action = 4)");
        return 4;  // Action for CAM4
      } else if (response.substring(actionValueIndex, actionValueIndex + 4) == "CAM5") {
        Serial.println("Actuator CAM5 (action = 5)");
        return 5;  // Action for CAM5
      }
    }
  } else {
    Serial.println("Connection to backend failed!");
    return -1;  // Return -1 in case of failure to fetch the actuator action
  }

  return -1;  // Default return in case of error
}

long measureWaterLevel() {
  // Send a pulse to trigger the ultrasonic sensor
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Read the time it takes for the pulse to return
  long duration = pulseIn(echoPin, HIGH);
  // Calculate the distance in centimeters (speed of sound: 343 m/s)
  long distance = duration * 0.034 / 2;
  // Calculate the water level
  long waterLevel = tankHeight - distance; // Subtract distance from tank height to get the water level
  return waterLevel;  // Return the measured water level
}
void sendWaterLevelData(long waterLevel) {
  // Prepare the JSON payload with ultrasonic sensor data (water level in cm)
  String payload = "{\"value\": " + String(waterLevel) + "}";  // Send water level value (in cm)
  // Connect to the server
  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, sending data...");
    // Send HTTP POST request with water level data
    client.println("POST /sensors/" + ultrasonicSensorId + " HTTP/1.1");  // Use the ultrasonic sensor ID
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
    Serial.println("Water level data sent to server.");
  } else {
    Serial.println("Connection failed!");
  }
}
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

// Create an instance of WiFiClient
WiFiClient client;



#define LED_GARDEN D1 
String LED_G_ID = "676599ec27520115c558361f";  
String ledLocation = "garden";  

#define LED_FIRST_FLOOR D2
String LED_FF_ID = "6787a73846243beea39b83a6";
String LED_FF_Location = "firstfloor";


#define LED_SECOND_FLOOR D3
String LED_SF_ID = "6787a8d446243beea39b83a8";
String LED_SF_Location = "secondfloor";


/*RGB*/
#define RED_PIN D5
#define GREEN_PIN D6
#define BLUE_PIN D7
String RGB_ID = "6787adba5ec2d2f971c62770";
String RGB_Location = "SecondFloor";


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  pinMode(LED_GARDEN, OUTPUT);
  pinMode(LED_FIRST_FLOOR,OUTPUT);
  pinMode(LED_SECOND_FLOOR,OUTPUT);
  /*RGB*/
  pinMode(RED_PIN, OUTPUT); 
  pinMode(GREEN_PIN, OUTPUT); 
  pinMode(BLUE_PIN, OUTPUT);
  // Initial RGB LED state 
  setRGB(255, 0, 0); // Red
}
void loop() {
  /************************* Fetch Values From Backend *****************/
  /*********************************** Garden Lighting ******************************/
  // Fetch the latest LED state from the backend
  int ledState1 = getLEDState(LED_G_ID);  // Get the LED state (0 or 1)
  Serial.print("Light1 State is: ");
  Serial.println(ledState1);
  // Act based on the LED state fetched from the backend
  if (ledState1 == 0) {
    // Turn the local LED ON if the state is 1
    digitalWrite(LED_GARDEN, HIGH);
    Serial.println("Light1 is ON (state = 1)");
  } else if (ledState1 == 1){
    // Turn the local LED OFF if the state is 0
    digitalWrite(LED_GARDEN, LOW);
    Serial.println("Light1 is OFF (state = 0)");
  }
  /****************************** First Floor Lighting *******************************/
  // Fetch the latest LED state from the backend
  int ledState2 = getLEDState(LED_FF_ID);  // Get the LED state (0 or 1)
  Serial.print("Light2 State is: ");
  Serial.println(ledState2);
  // Act based on the LED state fetched from the backend
  if (ledState2 == 0) {
    // Turn the local LED ON if the state is 1
    digitalWrite(LED_FIRST_FLOOR, HIGH);
    Serial.println("Light2 is ON (state = 1)");
  } else if (ledState2 == 1){
    // Turn the local LED OFF if the state is 0
    digitalWrite(LED_FIRST_FLOOR, LOW);
    Serial.println("Light2 is OFF (state = 0)");
  }
  /*************************** Second Floor Lighting ********************************/
  // Fetch the latest LED state from the backend
  int ledState3 = getLEDState(LED_SF_ID);  // Get the LED state (0 or 1)
  Serial.print("Light3 State is: ");
  Serial.println(ledState3);
  // Act based on the LED state fetched from the backend
  if (ledState3 == 0) {
    // Turn the local LED ON if the state is 1
    digitalWrite(LED_SECOND_FLOOR, HIGH);
    Serial.println("Light3 is ON (state = 1)");
  } else if (ledState3 == 1){
    // Turn the local LED OFF if the state is 0
    digitalWrite(LED_SECOND_FLOOR, LOW);
    Serial.println("Light3 is OFF (state = 0)");
  }
  /******************************** RGB Lighting *************************************/
  int red = 0 , green = 0 , blue = 0;
  int result = getRGBValues(RGB_ID, red, green, blue); 
  if (result == 1) { 
    setRGB(red, green, blue); 
    } else { 
      Serial.println("Failed to fetch RGB values");
    }

  delay(2000);  // 2 seconds delay between readings
}


int getLEDState(String ledId) {
  String url = "/leds/" + ledId + "/latest";  

  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, requesting LED state...");

    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Connection: close");
    client.println();

    String response = "";
    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        response += c;  
      }
    }

    // Print the entire response for debugging
    Serial.println("Response from server:");
    Serial.println(response);  

    client.stop();
    Serial.println("LED state request complete.");
    int stateIndex = response.indexOf("\"state\":");
    if (stateIndex != -1) {
      int stateValueIndex = stateIndex + 8;  
      if (response.substring(stateValueIndex, stateValueIndex + 4) == "true") {
        Serial.println("LED ON (state = 1)");
        return 1;  
      } else if (response.substring(stateValueIndex, stateValueIndex + 5) == "false") {
        Serial.println("LED OFF (state = 0)");
        return 0;  
      }
    }
  } else {
    Serial.println("Connection to backend failed!");
    return -1; 
  }

  return -1;  
}
/*RGB Functions*/
void setRGB(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
int getRGBValues(String rgbId, int &red, int &green, int &blue) {
  String url = "/RGB/" + rgbId + "/latest"; 

  if (client.connect(host, httpPort)) {
    Serial.println("Connected to backend, requesting RGB values...");
    // Send HTTP GET request to the backend to get the latest RGB values
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
    Serial.println("RGB values request complete.");

    // Now let's parse the JSON response to extract the RGB values
    int redIndex = response.indexOf("\"red\":");
    int greenIndex = response.indexOf("\"green\":");
    int blueIndex = response.indexOf("\"blue\":");

    if (redIndex != -1 && greenIndex != -1 && blueIndex != -1) {
      red = response.substring(redIndex + 6, response.indexOf(",", redIndex)).toInt();
      green = response.substring(greenIndex + 8, response.indexOf(",", greenIndex)).toInt();
      blue = response.substring(blueIndex + 7, response.indexOf("}", blueIndex)).toInt();

      Serial.println("RGB values fetched successfully:");
      Serial.print("Red: "); Serial.println(red);
      Serial.print("Green: "); Serial.println(green);
      Serial.print("Blue: "); Serial.println(blue);
      
      return 1;  // Success
    }
  } else {
    Serial.println("Connection to backend failed!");
    return -1; 
  }

  return -1;  
}



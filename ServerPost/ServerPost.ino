#include <WiFi.h>
#include <HTTPClient.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
int lasttime = 0;


//Initializing Force Sensing Resistors
//change pin numbers here to reorder data being sent
int fsr1 = 36;
int fsr2 = 39;
int fsr3 = 33;
int fsr4 = 35;
int fsr5 = 32;
int fsr6 = 34;
String ForceData = "";
String GyroData = "";
String AccelData = "";

//Define Wifi Credentials
const char* ssid = "MIT";
const char* password = "";

//Server Names
const char* server = "http://10.31.106.194:5000/parkinsons";

//Delay or Frequency
unsigned long lastTime = 0;
unsigned long Delay = 0;
bool checker = true;

void setup() {
  Serial.begin(115200);

  // TInitialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  //Adjust range and filter bandwidth for MPU6050
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("");
  delay(100);

  //Connect to WIFI
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Delay set at ");
  Serial.println(Delay);
  delay(500);
}

void loop() {
  
  if ((millis() - lastTime) > Delay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, server);

      while (checker) {
      //ForceData Acquisition
      ForceData = String(analogRead(fsr1));
      ForceData = ForceData + ", " + String(analogRead(fsr2));
      ForceData = ForceData + ", " + String(analogRead(fsr3));
      ForceData = ForceData + ", " + String(analogRead(fsr4));
      ForceData = ForceData + ", " + String(analogRead(fsr5));
      ForceData = ForceData + ", " + String(analogRead(fsr6));

      //GyroData Acquistion
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      AccelData = String(a.acceleration.x);
      AccelData = AccelData + ", " + String(a.acceleration.y);
      AccelData = AccelData + ", " + String(a.acceleration.z);

      GyroData = String(g.gyro.x);
      GyroData = GyroData + "," + String(g.gyro.y);
      GyroData = GyroData + "," + String(g.gyro.z);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // ForceData to send with HTTP POST
      String httpRequestData = "force_values=" + ForceData + "&gyro_values=" + GyroData + "&accel_values=" + AccelData;           

      // Send HTTP POST request
      lasttime = millis();
      int httpResponseCode = http.POST(httpRequestData);
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      delay(Delay);

      //insert code here to stop loop by setting checker to "false" if needed
      }
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

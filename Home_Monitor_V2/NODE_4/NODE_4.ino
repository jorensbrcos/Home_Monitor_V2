// Home monitoring system Version 1.0
// Temperature and Humidity readings sent to Base station and displayed trough serial monitor
// 4 Nodes, 1 Base station, 5 sensors
//18.11.2021

#include <Wire.h>
//I2C: SDA = G21; SCL = G22
#include "SparkFun_Si7021_Breakout_Library.h"

// Wireless transmission information
#include <esp_now.h>
#include <WiFi.h>

//Sensor registers and variables

float TEMPF = 0;
float HUMF = 0;

//Create Instance of HTU21D or SI7021 temp and humidity sensor 
Weather sensor;


// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0x7C, 0x9E, 0xBD, 0xF5, 0x44, 0xEC}; //Jaaizmaina

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int id; // must be unique for each sender board
    float temp;
    float hum;
} struct_message;

//Create a struct_message called myData
struct_message myData;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  //Initialize the I2C sensors and ping them
  sensor.begin();
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

}
 
void loop() {
  //Receive temperature an humidity data from Si7021 sensor
  getWeather();
    
  // Set values to send
  myData.id = 4;              //Change the ID for each node
  myData.temp = TEMPF;
  myData.hum = HUMF;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  //printInfo();
  delay(500);
}

/**
 * Weather stuff
 * */
void getWeather(){
  // Measure Relative Humidity from the Si7021
  HUMF = sensor.getRH();

  // Measure Temperature from the Si7021
  TEMPF = sensor.getTemp();
}

void printInfo()
{
//This function prints the weather data out to the default Serial Port

  Serial.print("Temp:");
  Serial.print(TEMPF);
  Serial.print("°C, ");

  Serial.print("Humidity:");
  Serial.print(HUMF);
  Serial.println("%");
}

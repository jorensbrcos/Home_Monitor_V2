// Home monitoring system Version 2.0
// Temperature and Humidity readings sent to Base station and displayed trough serial monitor
// 4 Nodes, 1 Base station, 5 sensors
// MASTER
// 18.11.2021

#include <Wire.h>
//I2C: SDA = G21; SCL = G22
#include "SparkFun_Si7021_Breakout_Library.h"

#include <esp_now.h>
#include <WiFi.h>

float Temp5 = 0;
float Hum5 = 0;

//Create Instance of SI7021 temp and humidity sensor 
Weather sensor;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  float temp;
  float hum;
}struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
struct_message board3;
struct_message board4;

// Create an array with all the structures
struct_message boardsStruct[4] = {board1, board2, board3, board4}; //{board1, board2, board3,...};

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  //Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.printf("Board ID %u: %u bytes\t", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].temp = myData.temp;
  boardsStruct[myData.id-1].hum = myData.hum;
  
//  Serial.print("Temp Node ");
//  Serial.print(myData.id);
//  Serial.print(" = ");
//  Serial.print(boardsStruct[myData.id-1].temp);
//
//  Serial.print("\tHum Node ");
//  Serial.print(myData.id);
//  Serial.print(" = ");
//  Serial.println(boardsStruct[myData.id-1].hum);

//  Serial.printf("x value: %d \t", boardsStruct[myData.id-1].x);
//  Serial.printf("y value: %d \t", boardsStruct[myData.id-1].y);
//  Serial.println();
}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);

  //Initialize the I2C sensors and ping them
  sensor.begin();
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  //Read from local sensor
  getWeather();
  
// Acess the variables for each board
  float Temp1 = boardsStruct[0].temp;
  float Hum1 = boardsStruct[0].hum;

  float Temp2 = boardsStruct[1].temp;
  float Hum2 = boardsStruct[1].hum;

  float Temp3 = boardsStruct[2].temp;
  float Hum3 = boardsStruct[2].hum;

  float Temp4 = boardsStruct[3].temp;
  float Hum4 = boardsStruct[3].hum;
  
//
//  
//  Serial.print("Board ID = ");
//  Serial.println(myData.id);
//  
  Serial.print("  TEMP1 = ");
  Serial.print(Temp1);
  Serial.print("  TEMP2 = ");
  Serial.print(Temp2);
  Serial.print("  TEMP3 = ");
  Serial.print(Temp3);
  Serial.print("  TEMP4 = ");
  Serial.print(Temp4);
  Serial.print("  TEMP5 = ");
  Serial.print(Temp5);

  Serial.print("    HUM1 = ");
  Serial.print(Hum1);
  Serial.print("  HUM2 = ");
  Serial.print(Hum2);
  Serial.print("  HUM3 = ");
  Serial.print(Hum3);
  Serial.print("  HUM4 = ");
  Serial.print(Hum4);
  Serial.print("  HUM5 = ");
  Serial.println(Hum5);
  
  
//
//  board1X = 0;
//  board1Y = 0;
//  int board2X = boardsStruct[1].x;
//  int board2Y = boardsStruct[1].y;
//  int board3X = boardsStruct[2].x;
//  int board3Y = boardsStruct[2].y;

//  Serial.print("ESP Board MAC Address:  ");
//  Serial.println(WiFi.macAddress());
  delay(500);  
}

void getWeather(){
  // Measure Relative Humidity from the Si7021
  Hum5 = sensor.getRH();

  // Measure Temperature from the Si7021
  Temp5 = sensor.getTemp();
}

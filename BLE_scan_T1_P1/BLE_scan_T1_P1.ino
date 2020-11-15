/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEBeacon.h>

int scanTime = 5; //In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice){
        //all devices will have a MACID
        Serial.printf("MACID: %s, ",advertisedDevice.getAddress().toString().c_str());
        
        if(advertisedDevice.haveRSSI()){
          //If RSSI is availble, display it
          Serial.printf("RSSI: %d, ",advertisedDevice.getRSSI());
          } 
        else{
          Serial.printf("RSSI: N/A, ");
          }
        
        if(advertisedDevice.haveTXPower()){
          //If RSSI is availble, display it
          Serial.printf("TX Power: %d, ",advertisedDevice.getTXPower());
          }
        else{
          Serial.printf("TX Power: N/A, ");
          }

        if(advertisedDevice.haveServiceUUID()){
          //If UUID is availble, display it
          Serial.printf("UUID: %s",advertisedDevice.getServiceUUID().toString().c_str());
          }
        else{
          Serial.printf("UUID: N/A");
          }
 
        if(advertisedDevice.getAddress().toString()=="f0:08:d1:c8:45:16"){ 
          //if the MACID matches then label this device as a target.  This is the MACID of my iBeacon
          Serial.printf(", Target \n");
        }
        else{
          Serial.printf("\n");
        }
    }
};


void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}

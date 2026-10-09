//Libraries for BLE
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

//Libraries for WIFI
#include <WiFi.h>


//define time for BLE
int scanTime = 1; // in seconds
BLEScan* pBLEScan;

//define names for WIFI
#define MAX_SAMPLES 55

volatile int sampleCount = 0;
bool collectionDone = false;

// ----- Wi-Fi target (your anchor SSID) -----
const char* targetWiFiSSID1 = "WIFI_ANCHOR_01";
const char* targetWiFiSSID2 = "WIFI_ANCHOR_02";
const char* targetWiFiSSID3 = "WIFI_ANCHOR_03";


//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26



//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

//OLED pins
#define OLED_SDA 21
#define OLED_SCL 22 
#define OLED_RST -1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String LoRaData;

// ----- Modes -----
#define MODE_BLE  1
#define MODE_WIFI 2
#define MODE_LORA  3

int mode = MODE_LORA;  // Change this to select mode

int counter=0; // count the number of results

bool ONE_received=false;
bool TWO_received=false;
bool THREE_received=false;

int ONE_rssi=0;
int TWO_rssi=0;
int THREE_rssi=0;


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
   // Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());

    if((advertisedDevice.getName() == "ONE"  || advertisedDevice.getName() == "TWO" || advertisedDevice.getName() == "THREE") && counter<56){
          int rssi = advertisedDevice.getRSSI();

        //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
                if(advertisedDevice.getName() == "ONE" && ONE_received==false){
                 // ONE_received=true;
                  ONE_rssi=advertisedDevice.getRSSI();     
                  //Serial.print("ONE:");
                 // Serial.print(ONE_rssi);
                  }else if(advertisedDevice.getName() == "TWO" && TWO_received==false){
                  TWO_received=true;
                  TWO_rssi=advertisedDevice.getRSSI();
                 // Serial.print("TWO:");
                  //Serial.print(TWO_rssi);
                  }else if(advertisedDevice.getName() == "THREE" && THREE_received==false){
                  THREE_received=true;
                  THREE_rssi=advertisedDevice.getRSSI();
                  //Serial.print("THREE:");
                 // Serial.print(THREE_rssi);
                  }

              //  if(ONE_received==true){// && TWO_received==true && THREE_received==true){
                ONE_received=false;
                TWO_received=false;
                THREE_received=false;
                Serial.print("\n");
                Serial.print(counter);
                Serial.print(",");
                Serial.print("ONE,");
                Serial.print(ONE_rssi);
              //  Serial.print(",TWO,");
               // Serial.print(TWO_rssi);
               // Serial.print(",THREE,");
               // Serial.print(THREE_rssi);
                counter++;
               // }

                // Display information
                  display.clearDisplay();
                  display.setCursor(0,0);
                  display.print("BLE RECEIVER");
                  display.setCursor(0,20);
                  display.print(advertisedDevice.getName());
                  display.setCursor(30,20);
                  display.print("RSSI:");
                  display.setCursor(60,20);
                  display.print(rssi);
                  display.display();   
      }
  }
};

void setup() { 
  //initialize Serial Monitor
  Serial.begin(115200);
  
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);

  if(mode == MODE_LORA){
      display.print("LORA RECEIVER ");
      display.display();

      Serial.println("LoRa Receiver Test");
      
      //SPI LoRa pins
      SPI.begin(SCK, MISO, MOSI, SS);
      //setup LoRa transceiver module
      LoRa.setPins(SS, RST, DIO0);

      if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        while (1);
      }
      Serial.println("LoRa Initializing OK!");
      display.setCursor(0,10);
      display.println("LoRa Initializing OK!");
      display.display();  
  }else if(mode == MODE_BLE){

      display.print("BLE RECEIVER ");
      display.display();

      BLEDevice::init("");
      pBLEScan = BLEDevice::getScan(); //create new scan
      pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
      pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
      pBLEScan->setInterval(100);
      pBLEScan->setWindow(99);  // less or equal setInterval value

      //Serial.println("BLE Receiver Test");
      //Serial.println("BLE Initializing OK!");
      //display.setCursor(0,10);
      //display.println("BLE Initializing OK!");
      //display.display();

  }else if(mode == MODE_WIFI) {
    // ----- Wi-Fi scanning -----

     display.print("WIFI RECEIVER ");
      display.display();
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
  //  delay(100);
  // Serial.println("Wi-Fi scanning mode");
}
}

void loop() {


if(mode == MODE_LORA){
  //try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize && !collectionDone) {
    //received a packet
    //Serial.print("Received packet ");
  
    //read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
   //Serial.print(LoRaData);
    }

    sampleCount++;            // <-- ADD (count sample)
            
    if (sampleCount >= MAX_SAMPLES) {   // <-- ADD (stop condition)
        collectionDone = true;
        //Serial.println("=== WIFI RSSI COLLECTION COMPLETE ===");
       // break;
      }
   
    //print RSSI of packet
    int rssi = LoRa.packetRssi();
    Serial.print(sampleCount);
    Serial.print(",");
   // Serial.print(" with RSSI ");    
    Serial.println(rssi);

    // Dsiplay information
              display.clearDisplay();
              display.setCursor(0,0);
              display.print("LORA TAG");
              display.setCursor(0,20);
              display.print("Received packet:");
              display.setCursor(0,30);
              display.print(LoRaData);
              display.setCursor(0,40);
              display.print("RSSI:");
              display.setCursor(30,40);
              display.print(rssi);
              display.display();  
     }
  }else if(mode == MODE_BLE){
            // Dsiplay information

            BLEScanResults *foundDevices = pBLEScan->start(scanTime, false);
            //Serial.print("Devices found: ");
           // Serial.println(foundDevices->getCount());
           // Serial.println("Scan done!");
            pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
              display.clearDisplay();
              display.setCursor(0,0);
              display.print("BLE TAG");
              display.setCursor(0,20);
            //  display.print("Received packet:");
            //  display.setCursor(0,30);
            // display.print(LoRaData);
            // display.setCursor(0,40);
            // display.print("RSSI:");
            // display.setCursor(30,40);
            // display.print(rssi);
              display.display();  
  }else if (mode == MODE_WIFI) {

 // if (collectionDone) return;   // <-- ADD (stop when done)

  int n = WiFi.scanNetworks();
  unsigned long timestamp = millis();

  for (int i = 0; i < n && !collectionDone; ++i) {  // <-- CHANGE
    String ssid = WiFi.SSID(i);
    int32_t rssi = WiFi.RSSI(i);
    display.print("MALAKIES");

    display.clearDisplay();
              display.setCursor(0,0);

    if (ssid == targetWiFiSSID1 || ssid == targetWiFiSSID2 || ssid == targetWiFiSSID3) {
      display.print("WIFI TAG");
      display.display();
              display.clearDisplay();
              display.setCursor(0,0);
              display.print("WIFI TAG");
              display.setCursor(0,20);
              display.print("NETWORK");
              display.setCursor(0,40);
              display.print(ssid);
              display.setCursor(0,60);
      
              Serial.print(sampleCount);
              Serial.print(",");
              Serial.println(rssi);      

      //Serial.print(timestamp);
      //Serial.print(", ");
      //Serial.print(ssid);
      //Serial.print(", ");
      //Serial.println(rssi);

      sampleCount++;            // <-- ADD (count sample)

      if (sampleCount >= MAX_SAMPLES) {   // <-- ADD (stop condition)
        collectionDone = true;
        //Serial.println("=== WIFI RSSI COLLECTION COMPLETE ===");
        break;
      }
    }
  }

//  delay(1000);
  }
}

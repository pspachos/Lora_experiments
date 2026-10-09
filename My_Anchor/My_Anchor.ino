//Libraries for BLE
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


//Libraries for WIFI
#include <WiFi.h>


// ----- Wi-Fi AP settings -----
const char* wifiSSID = "WIFI_ANCHOR_01";
const char* wifiPassword = "12345678";

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define for BLE
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

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

//packet counter
int counter = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

// ----- Modes -----
#define MODE_BLE  1
#define MODE_WIFI 2
#define MODE_LORA  3

int mode = MODE_WIFI;  // Change this to select mode

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
      display.print("LORA ANCHOR 1 ");
      display.display();
      
      Serial.println("LoRa Sender Test");

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
      display.print("LoRa Initializing OK!");
      display.display();
     // delay(2000);
  }else if(mode == MODE_BLE){
      display.print("BLE ANCHOR 1 ");
      display.display();

      BLEDevice::init("ONE");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("Hello World says Neil");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  }else if(mode == MODE_WIFI){
  display.print("WIFI ANCHOR ");
    display.display();
    Serial.println("WIFI Receiver Test");
    Serial.println("Scanning...");


    WiFi.softAP(wifiSSID, wifiPassword);
    Serial.print("Wi-Fi AP started. IP: ");
    Serial.println(WiFi.softAPIP());
}
}

void loop() {
   
  Serial.print("Sending packet: ");
  Serial.println(counter);

if(mode == MODE_LORA){
      //Send LoRa packet to receiver
      LoRa.beginPacket();
      LoRa.print("hello ");
      LoRa.print(counter);
      LoRa.endPacket();
      
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("LORA ANCHOR 1");
      display.setCursor(0,20);
      display.setTextSize(1);
      display.print("LoRa packet sent.");
      display.setCursor(0,30);
      display.print("Counter:");
      display.setCursor(50,30);
      display.print(counter);      
      display.display();

      counter++;
}else if(mode == MODE_BLE){
 LoRa.beginPacket();
      
      
      LoRa.print("hello ");
      LoRa.print(counter);
      LoRa.endPacket();

      display.clearDisplay();
      display.setCursor(0,0);
      display.println("BLE ANCHOR 1");
      display.setCursor(0,20);
      display.setTextSize(1);
      display.print("BLE packet sent.");
      display.setCursor(0,30);
      display.print("Counter:");
      display.setCursor(50,30);
      display.print(counter);      
      display.display();

      counter++;
}
  
 delay(1000);
}
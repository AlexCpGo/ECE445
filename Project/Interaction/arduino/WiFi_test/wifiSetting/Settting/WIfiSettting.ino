#include <ESP8266.h>
#include "ESP8266.h"
#include<SoftwareSerial.h>
#define WIFI_TX 9
#define WIFI_RX 8

#define SSID        "iPhone"
#define PASSWORD    "19990729"
#define HOST_NAME   "172.20.10.9"
#define HOST_PORT   8800 
SoftwareSerial mySerial(WIFI_RX,WIFI_TX);
ESP8266 wifi(mySerial,115200);

void setup(void)
{
    Serial.begin(230400);  
    Serial.print("setup begin\r\n");
    
    Serial.print("FW Version:");
    Serial.println(wifi.getVersion().c_str());
    // Set operation mode
    while(wifi.setOprToStationSoftAP()==false){
      Serial.print("Waiting for setting operating mode station + softap ...\r\n");
      Serial.print("......\n");
      delay(100);
    }
    Serial.print("Operating mode station + softap ok\r\n");
    
    while(wifi.joinAP(SSID, PASSWORD)==false){
      Serial.print("waiting for joining AP...\r\n");
      delay(100);
    }
    Serial.print("Join AP success\r\n");
    
    Serial.print("IP:");
    Serial.println( wifi.getLocalIP().c_str());  
    
   //设置连接模式   
    while(wifi.disableMUX()==false){
      Serial.print("waiting for single connecting mode TCP or UDP ...\r\n");
      delay(100);
    }
    Serial.print("single mode ok\r\n");
    Serial.print("setup success\r\n"); 
}
 
void loop(void)
{undefined
    uint8_t buffer[128] = {0};

    while(wifi.registerUDP(HOST_NAME, HOST_PORT)==false){
      Serial.print("register udp err\r\n");
      delay(100);
    }
    Serial.print("register udp ok\r\n");
    
    //Need to modify
    char *ACData = "Hello, this is client!";
    
    wifi.send((const uint8_t*)ACData, strlen(ACData));


    
//    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
//    if (len > 0) {  
//        Serial.print("Received:[");
//        for(uint32_t i = 0; i < len; i++) {undefined
//            Serial.print((char)buffer[i]);
//        }
//        Serial.print("]\r\n");
//    }
    
    if (wifi.unregisterUDP()) {
        Serial.print("unregister udp ok\r\n");
    } else {undefined
        Serial.print("unregister udp err\r\n");
    }
    delay(5000);
} 

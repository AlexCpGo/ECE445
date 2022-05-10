#include "ESP8266.h"
#include<SoftwareSerial.h>
#define WIFI_TX 9
#define WIFI_RX 8

#define SSID        "lyc"
#define PASSWORD    "200807lyc"
#define HOST_NAME   "172.20.10.7"
#define HOST_PORT   8800 
SoftwareSerial mySerial(WIFI_RX,WIFI_TX);
ESP8266 wifi(mySerial,115200);
 
void setup(void)
{
    Serial.begin(9600);  
    Serial.print("setup begin\r\n");
    
    Serial.print("FW Version:");
    Serial.println(wifi.getVersion().c_str());
    // Set Working Mode
    while(wifi.setOprToStationSoftAP()==false){undefined
      Serial.print("waiting for setting operating mode station + softap ...\r\n");
      delay(100);
    }
    Serial.print("operating mode station + softap ok\r\n");
    //连接WiFi
    while(wifi.joinAP(SSID, PASSWORD)==false){undefined
      Serial.print("waiting for joining AP...\r\n");
      delay(100);
    }
    Serial.print("Join AP success\r\n");
    //WiFi连接成功后打印IP地址
    Serial.print("IP:");
    Serial.println( wifi.getLocalIP().c_str());  
    
   //设置连接模式   
    while(wifi.disableMUX()==false){undefined
      Serial.print("waiting for single connecting mode TCP or UDP ...\r\n");
      delay(100);
    }
    Serial.print("single mode ok\r\n");
    Serial.print("setup success\r\n"); 
}
 
void loop(void)
{
    uint8_t buffer[128] = {0};

    while(wifi.registerUDP(HOST_NAME, HOST_PORT)==false){undefined
      Serial.print("register udp err\r\n");
      delay(100);
    }
    Serial.print("register udp ok\r\n");
    

    char *hello = "Hello, this is client!";
    wifi.send((const uint8_t*)hello, strlen(hello));//发送数据
    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);//接收数据
    if (len > 0) {  //打印数据
        Serial.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {undefined
            Serial.print((char)buffer[i]);
        }
        Serial.print("]\r\n");
    }
    
    if (wifi.unregisterUDP()) {undefined
        Serial.print("unregister udp ok\r\n");
    } else {undefined
        Serial.print("unregister udp err\r\n");
    }
    delay(5000);
} 

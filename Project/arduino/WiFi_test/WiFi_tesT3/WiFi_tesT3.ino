#include <SoftwareSerial.h>

#define WIFI_TX 9 // TX OF wifi - RX of Arduino pin 8

#define WIFI_RX 8 // RX OF wifi - TX of Arduino pin 9

// AT command
// STA MODE:
// AT   :For testing
// AT+CWJAP="iPhone","19990729"   : Connecting Personal Wifi
// AT+CIPMUX=0    : TCP single link 
// AT+CIFSR: GET IP ADDRESS
// AT+CIPSTART="TCP","192.168.4.1",8800   : Set TCP
// AT+CIPSEND=0,25

// AT+CIPSERVER=1,8266



// AP MODE
// AT+CWJAP="ceshi","56785678",1,4
// AT+CIPSTART="TCP","172.20.10.8",8800   : Set TCP


SoftwareSerial wifi(WIFI_RX,WIFI_TX);

String _comdata_wifi = "";

 
void setup() {

  //put your setup code here, to run once:

 Serial.begin(9600);

 wifi.begin(115200);

 Serial.println("system is ready!");

// wifi.println("AT+CWJAP=\"lyc\",\"200807lyc\"");
// delay(20);
// getWifiSerialData();
// Serial.println(_comdata_wifi);
// _comdata_wifi=String("");

}

 

void loop() {

// 通过软串口写ESP8266命令
 getWifiSerialData();

 
 if(Serial.available()){

   String order = "";
   
   // Artificial read

   while(Serial.available()){

     char cc = (char)Serial.read();

     order += cc;

     delay(2);
 
    }

   // Automatical read
   String order1 = "AT+CWJAP=\"iPhone\",\"19990729\"";
   String order2 = "AT+CIPMUX=0";
   String order3 = "AT+CIFSR"; //Get the Ip address
   String order4 = "AT+CIPSTART=\"TCP\",\"192.168.4.1\",8800"

   order.trim();

   wifi.println(order);

  }

 

// Serial Print

 if(_comdata_wifi!=""){

   Serial.println(_comdata_wifi);

   _comdata_wifi=String("");

  }

}

 

void getWifiSerialData(){

   while(wifi.available()>0){

     _comdata_wifi += char(wifi.read());

     delay(4);

    }

}

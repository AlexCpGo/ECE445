#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

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
MPU6050 mpu;


#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

String _comdata_wifi = "";
String IP = "";
String str = "";
int ATflag;

//--------------------------Helper Function-----------------------
int ATcommand(string x);

//----------------------------------------------------------------
void setup() {
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

 Serial.begin(9600);

 wifi.begin(115200);

 mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  
  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

 Serial.println("system is ready!");

// wifi.println("AT+CWJAP=\"lyc\",\"200807lyc\"");
// delay(20);
// getWifiSerialData();
// Serial.println(_comdata_wifi);
// _comdata_wifi=String("");
  Serial.println("Print a to start");
  Serial.println(F("\nSend any character to begin demo: "));
  while (Serial.available() && Serial.read()); // empty buffer
  while (!Serial.available());                 // wait for data
  while (Serial.available() && Serial.read()); // empty buffer again
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
      // Calibration Time: generate offsets and calibrate our MPU6050
      mpu.CalibrateAccel(6);
      mpu.CalibrateGyro(6);
      mpu.PrintActiveOffsets();
      // turn on the DMP, now that it's ready
      Serial.println(F("Enabling DMP..."));
      mpu.setDMPEnabled(true);

      // enable Arduino interrupt detection
      Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
      Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
      Serial.println(F(")..."));
      attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus();

      // set our DMP Ready flag so the main loop() function knows it's okay to use it
      Serial.println(F("DMP ready! Waiting for first interrupt..."));
      dmpReady = true;

      // get expected DMP packet size for later comparison
      packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
      // ERROR!
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
  }

  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
  
  ATflag = 0;
  Serial.println(F("Initializing WiFi..."));
  
  ATcommand("AT+CWMODE=1");
  Serial.println("STA Mode initialized");
  ATflag = 0;
  
  ATcommand("AT+RST");
  Serial.println("Reset......");
  ATflag = 0;
  
  while(ATflag == 0){ 
    Serial.println("Connecting WiFi......");
    ATcommand("AT+CWJAP=\"lyc\",\"200807lyc\"");
  }
  Serial.println("WiFi Connected");
  
  ATcommand("AT+CIPMUX=1");
  Serial.println("IP Mux Estabilshed");

  ATcommand("AT+CIPSERVER=1,8086");
  Serial.println("Server Linked");

  // Check for IP address
  wifi.println(F("AT+CIFSR"));
  delay(2000);
  
  getWifiSerialData();
  Serial.println(_comdata_wifi);
  if(wifi.find("OK")){
    Serial.println("Success");
  }
  delay(2000);
  
  Serial.println("connected");
  while (true) {
    delay(2000);
    if(wifi.find('0')){
      Serial.println("good");
      break;
    } else {
      Serial.println("connecting");
    }
  }

void loop() {
// if programming failed, don't try to do anything
    if (!dmpReady) return;
    // read a packet from FIFO
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet 
        // display real acceleration, adjusted to remove gravity
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        str = "";
        str.concat(aaReal.x);
        while (str.length()<5) {
          str.concat('a');
        }
        str.concat(aaReal.y);
        while (str.length()<10) {
          str.concat('a');
        }
        str.concat(aaReal.z);
        while (str.length()<15) {
          str.concat('a');
        }
        // blink LED to indicate activity
//        blinkState = !blinkState;
//        digitalWrite(LED_PIN, blinkState);
        Serial.println(str.length());
        wifi.println(F("AT+CIPSEND=0,15"));
        delay(10);
        wifi.println(str);
    }
}


//void loop() {
//
//// 通过软串口写ESP8266命令
// getWifiSerialData();
//
// 
// if(Serial.available()){
//
//   String order = "";
//
//   while(Serial.available()){
//
//     char cc = (char)Serial.read();
//
//     order += cc;
//
//     delay(2);
//
//    }
//
//   order.trim();
//
//   wifi.println(order);
//
//  }
//
// 
//

//
// if(_comdata_wifi!=""){
//
//   Serial.println(_comdata_wifi);
//
//   _comdata_wifi=String("");
//
//  }
//
//}

 

void getWifiSerialData(){

   while(wifi.available()>0){

     _comdata_wifi += char(wifi.read());

     delay(4);

    }

}

int ATcommand(string x){
  wifi.println(F(x));
  delay(2000);
  if(wifi.find("OK")){
    Serial.println("Success");
    ATflag = 1;
  } else {
    Serial.println("Fail");
    ATflag = 0;
  }
  delay(2000);
}

# ZJUI ECE445 Project
Sp2022 Team 11

# LOW-COST SENSOR FOR SEAMLESS ROAD QUALITY MONITORING
An undergraduate graduation project involves hardware and software interactions

### 1. Abstract
In this project, our group designed a low-cost sensor to collect road data, aiming at conveniently
reporting the quality of seamless roads. To achieve this target, we designed an
inexpensive device with an accelerometer and a GPS chip. Our device could be fixed on
the bicycle with a device holder and then transmit data to the server. Machine learning
models have been created to process acceleration data and judge the road quality. When
abnormal patterns appeared, message where the patterns locate would be produced and
sent to our display system. Locations with problems could be visualized on a city map
constantly and could be accessible for users via web browsers as clients.

###### Key Word
Road quality monitoring; Acceleration; Arduino; Esp8266 WIFI module; Fourier Transform; Logistic regression; SVM; Random Forest; Display Webpage;

### 2. Block Diagram
###### ------------------------- Figure 1. Block Diagram of the project -------------------------
![image](https://user-images.githubusercontent.com/60203135/170616237-1bf2662f-182e-4a7b-b7ac-74ca2fe1e5d6.png)


### 3. Device System
###### ------------------------- Figure 2. Overlook of Device System -------------------------
![DeviceSystem](https://user-images.githubusercontent.com/60203135/170618257-f0057a07-d2a0-4a0c-8b06-7fc644321128.png)
###### Test On Device
![test](https://user-images.githubusercontent.com/60203135/170618264-5bb2bd68-61a6-4925-95fd-7fab90115c9c.jpg)
###### Device Interface Schematic
![Device Interface Schematic](https://user-images.githubusercontent.com/60203135/170618273-e6b78d20-2d3e-4b97-be05-923580853f33.png)

### 4. Hardware Component
###### Acceleration Sensor Module: MPU6050
![MPU6050](https://user-images.githubusercontent.com/60203135/170618440-e1cc5e5f-a208-4743-b3de-46dc6fb1076b.png)
###### GPS Module: NEO-6M
![NEO6M](https://user-images.githubusercontent.com/60203135/170618544-1a2da798-3a5e-4f6b-8429-5dc141650c61.png)
###### WIFI Module: esp8266
![esp8266](https://user-images.githubusercontent.com/60203135/170618865-ce68cee4-9790-4e93-a7a2-99f2bb063514.jpg)
###### MCU: STEMTeraBOARD
![MCU](https://user-images.githubusercontent.com/60203135/170621567-cdf2a5c6-701f-478c-ba7f-0a79918d3323.png)
###### Battery Supply: 9V Ni-MH Battery
![battery](https://user-images.githubusercontent.com/60203135/170621383-e1027ff7-3040-494f-bc37-d3f32890ef92.png)


### 5. Data Transmission Flow
The first part is Hardware part. The Arduino will integrate the signal collected from accelerometer
and GPS. It will write the data to the serial. The second part is the connection
between Hardware and server part. The serial data will be transmitted to computer using
the wire connected. The computer can receive the data through port. The third part
is the python script on computer which uses the data received from port to form log.
The fourth part is the data preprocessing implemented on server. It will get the signal
of accelerometer and GPS and form the array that can be accepted by the classification
subsystem.
![data_transmitting](https://user-images.githubusercontent.com/60203135/170617541-7984d05c-ef78-4ed6-9590-68e0b9b089e8.png)

The script of forming log keeps running. Once port receives data, the script starts get the
data from port and write it on the log. Then, once the port is disconnected, the recording
will finish. The format of log is illustrated below.
Every couple of data contains two rows. The first row is records the second. Is used to
show the sample rate. The second row records the data from accelerometer and GPS. The
format of this row is (longitude, latitude, second. x-acc y-acc z-acc). Though there is also
time recorded, it is not updated in enough frequency, so it is not used in our system.
![log](https://user-images.githubusercontent.com/60203135/170619336-eb0a0ab2-2f3d-4761-ba9b-047652c05e3e.png)

### 6. Data Pre-processing
The preprocessing part on server is a script. Once a new log is detected, the preprocessing
module will start to work. It first read the log and exclude the row without useful
information. Then it will get couple of rows need to be further processed. The first row,
which shows the time will recorded in a list. The second rows will be processed through
regular expression to get the signal from accelerometer and GPS.
After we get the raw data, we need to do resampling, because of the need of Fourier
Transform and Wavelet Transform. The signal from accelerometer and GPS recorded in
one second will be combined into a list.
For signal from accelerometer, we will use interpolation algorithm to resample each list
to 50Hz. 50Hz is chosen because the sample rate of our raw data is 40Hz 50Hz.
Finally, the lists recorded in Ten consecutive seconds will be combined into a segment.
The signal from accelerometer will form an array with dimension of (3, 500). 3 represents
three axes of accelerometer. 500 represents ten seconds of signal with sample rate with
50Hz. The signal form GPS will be a tuple of longitude, latitude, which is the tuple with
middle index from list combined.
The data from accelerometer will be sent to classification subsystem and data from GPS
will be sent to display subsystem.

### 7. Data Processing Algorithm
###### 7.1 General Processing Procedure
The classification subsystem mainly includes two parts, feature extraction and machine
learning model.
A method of feature extraction should be chosen to make the abnormal pattern can be
more easily recognized by model.
The machine learning model will need to use the feature extracted to classify the road as
accurately as possible. In addition, the speed of processing needs to be within 0.1s for
each segment.
There two tasks for the machine learning model. First, it should classify the condition
of roughness of the road. Second, it should distinguish whether there is obstacle on the
road. We will test the method from [1], which includes two method of feature extraction,
and three machine learning model. After testing, we will choose the best combination
and implement it on our server.
![Procedure](https://user-images.githubusercontent.com/60203135/170621122-7244599b-5dc3-4fdf-a0b4-7a773f11084e.png)
After we get the result, the label will be labeled on the map according to the signal form
GPS.

###### 7.2 Processing Algorithm Method: Fourier Transform + Machine Learning
###### Raw Data Frequency
![Z](https://user-images.githubusercontent.com/60203135/170622056-d5592e99-37a2-4742-80e3-11929aee4b13.png)
###### FFT
![Frequency](https://user-images.githubusercontent.com/60203135/170622021-ac7cfe58-7895-47d2-a1b8-18fa82b08b3d.png)
###### Wavelet
![Dicrete Wavelet Tranform](https://user-images.githubusercontent.com/60203135/170621683-fd9862e8-2376-4ea4-87f0-d221df99bde1.png)
###### Feature Extraction
![Z_processed](https://user-images.githubusercontent.com/60203135/170622114-a762f276-0db5-427b-a86f-f4ce0db7a58e.png)

### 8. Display System
![map](https://user-images.githubusercontent.com/60203135/170617560-63dc34dc-ee43-4c1c-8b93-569e0a733098.png)
###### Figure Map Marks automatically produced using Python
###### Webpage automatically produced using html+javascript
![WebPage](https://user-images.githubusercontent.com/60203135/170616397-bf389414-9118-4ec1-a284-ec5d1e40de92.png)



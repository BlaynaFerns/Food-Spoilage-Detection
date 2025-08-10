# Food-Spoilage-Detection

This project detects spoiled food using gas and temperature sensors with Arduino. When the sensors detect values above the threshold (indicating spoilage), the system alerts the user via a buzzer, LED, and LCD display.

---

## Table of Contents
 
- [Overview](#overview)
- [Software Used](#software-used)
- [Components Used](#components-used)
- [Circuit Connections](#circuit-connections)
- [How It Works](#how-it-works)
- [ThingSpeak Setup](#thingspeak-setup)
- [Code Breakdown](#code-breakdown)
- [Simulation](#simulation)
- [Photos](#photos)
- [How to Run](#how-to-run)
- [Performance Analysis](#performance-analysis)
- [License](#license)

---

## Overview

This project detects methane gas levels in stored food items to determine whether food is fresh or spoiled.  
It uses:

- **Arduino Uno** to read gas, light, temperature, and humidity sensor data, and control the LCD display.

- **ESP8266 Wi-Fi module** (via AT commands) to send data to the ThingSpeak IoT platform for monitoring.

- Visual feedback via a 16x2 LCD display showing live sensor readings.

When methane or other measured values exceed a set threshold, the system:

1. Displays warnings and sensor data on the LCD.

2. Sends the updated readings (temperature, humidity, LDR, gas levels) to the ThingSpeak dashboard.

3. Allows users to monitor environmental conditions remotely via the ThingSpeak web interface.

---

## Software Used

- **Operating System:** Microsoft Windows 10  
- **Development Tools:**  
  - Proteus Simulation Software  
  - Arduino IDE  
- **Programming Language:** C++
  
---

## Components Used

| **Component**            | **Description**                              | **Key Specifications**                                          |
| ------------------------ | -------------------------------------------- | --------------------------------------------------------------- |
| **Arduino UNO R3**       | Microcontroller board based on ATmega328 AVR | 20 I/O pins (6 PWM, 6 analog); easy-to-use Arduino IDE          |
| **MQ4 Sensor**           | Detects methane gas concentration            | Range: 300 ppm – 10,000 ppm; Temp: -10°C to 50°C; <150 mA @ 5 V |
| **Wi-Fi ESP8266 Module** | Low-cost Wi-Fi microchip with TCP/IP stack   | Connects microcontrollers to Wi-Fi networks                     |
| **Breadboard**           | Solderless prototyping platform              | Easy assembly and reusability                                   |
| **Buzzer**               | Audible tone generator for alerts            | Lightweight, inexpensive                                        |
| **USB Cable**            | Connects Arduino to PC                       | Used for programming and power supply                           |


---

## Circuit Connections

### Arduino UNO

| Arduino Pin | Component               | Description                                  |
| ----------- | ----------------------- | -------------------------------------------- |
| D12         | ESP8266 RX              | Serial communication from Arduino to ESP8266 |
| D13         | ESP8266 TX              | Serial communication from ESP8266 to Arduino |
| D10, D9, D5, D4, D3, D2 | LCD Display | 16x2 LCD for displaying sensor data          |
| D8          | DHT11 Sensor            | Measures temperature and humidity            |
| A1          | LDR Sensor              | Detects light intensity                      |
| A0          | MQ3 Gas Sensor          | Detects methane or spoilage gases            |
| 5V / GND    | Power & Ground          | Powers sensors, LCD, and ESP8266             |



---

## How It Works

### Hardware Implementation
- The MQ4 sensor detects methane gas from spoiled food.
- Analog signal from MQ4 is read by Arduino’s ADC.
- ESP8266 transmits data to the server for threshold comparison.
- Results displayed on the Blynk app and/or LED screen.

### Software Implementation
- Arduino collects and formats sensor data.
- Data sent to the server for processing.
- Alerts triggered if gas concentration exceeds safe levels.
- Real-time dashboard on Blynk app for user monitoring.

---

## ThingSpeak Setup

1. **Create a ThingSpeak Account**  
   - Go to [ThingSpeak](https://thingspeak.com/) and sign up for a free account.  

2. **Create a New Channel**  
   - Click **"New Channel"**.  
   - Name your channel (e.g., *Food Spoilage Monitoring*).  
   - Add 4 fields:  
     - **Field 1** – Temperature  
     - **Field 2** – Humidity  
     - **Field 3** – Light Level (LDR)  
     - **Field 4** – Gas Sensor Value  

3. **Get Your API Key**  
   - After creating the channel, go to the **API Keys** tab.  
   - Copy the **Write API Key** and replace `"your_api_key"` in the Arduino code.  

4. **Update WiFi Credentials in Code**  
   - Replace `"your_ssid"` and `"your_password"` in the `esp8266()` function with your network details.  

5. **Run the System**  
   - Upload the Arduino code to the Arduino UNO.  
   - Once running, sensor values will be sent to your ThingSpeak channel at the set update interval (default: 15 seconds).  

6. **View Data**  
   - In ThingSpeak, open your channel to view live graphs for each sensor.  


---

## Code Breakdown

#### 1. Libraries and Definitions
- **`#include <SoftwareSerial.h>`** — Enables serial communication on other digital pins to communicate with the ESP8266.  
- **`#include <LiquidCrystal.h>`** — Controls the 16x2 LCD display.  
- **`#include <DHT.h>`** — Reads temperature and humidity from the DHT11 sensor.  
- **`#define DHTPIN 8`** — DHT11 sensor connected to pin 8.  
- **`#define LDRPIN A1`** — LDR (Light Dependent Resistor) connected to analog pin A1.  
- **`#define MQ3PIN A0`** — MQ3 gas sensor connected to analog pin A0.  



#### 2. Hardware Initialization
- **`LiquidCrystal lcd(10, 9, 5, 4, 3, 2)`** — Configures LCD with the respective Arduino pins.  
- **`DHT dht(DHTPIN, DHT11)`** — Initializes the DHT11 sensor.  



#### 3. `setup()` Function
- Initializes the LCD (16x2).  
- Starts serial communication at 9600 baud rate.  



#### 4. `loop()` Function
1. **Sensor Readings**  
   - `ldr_read` → Light intensity value from LDR.  
   - `gas_read` → Gas concentration from MQ3.  
   - `t` → Temperature from DHT11.  
   - `h` → Humidity from DHT11.  

2. **LCD Display**  
   - **First row** → LDR value.  
   - **Second row** → MQ3 value.  

3. **Serial Output**  
   - Prints temperature and humidity for debugging.  

4. **Data Transmission**  
   - Calls `esp8266()` to send sensor values to ThingSpeak via Wi-Fi.  



#### 5. `esp8266()` Function
Handles ESP8266 communication via AT commands:  

1. **Serial Setup**  
   - `SoftwareSerial espSerial(12, 13)` → Defines RX (12) and TX (13) pins for ESP8266.  

2. **ESP8266 Reset**  
   - Sends `"AT+RST"` to restart the module.  

3. **Wi-Fi Connection**  
   - `"AT+CWJAP=\"SSID\",\"PASSWORD\""` → Connects to the given Wi-Fi network.  

4. **Mode Setting**  
   - `"AT+CWMODE=1"` → Sets ESP8266 to client mode.  

5. **Server Connection**  
   - `"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80"` → Opens TCP connection to ThingSpeak.  

6. **HTTP GET Request**  
   - Sends temperature, humidity, LDR, and gas data to ThingSpeak using the provided API key.  

7. **Update Delay**  
   - Waits **15 seconds** before sending the next update (ThingSpeak requirement).  



---

## Simulation

- The entire circuit was first tested in **Proteus 8** before hardware assembly.  
- Arduino UNO and connected sensors (MQ3, DHT11, LDR) were simulated to verify correct readings.  
- Gas sensor values were emulated using the **Proteus Virtual Terminal** to mimic real methane levels.  
- LEDs and buzzer were triggered in simulation exactly as they do in the physical build.  
- The ESP8266 module was also simulated to confirm ThingSpeak data transmission before hardware deployment.  

---

## Photos

### Photos

- [Circuit Diagram](https://github.com/BlaynaFerns/Food-Spoilage-Detection/blob/main/Circuit_Diagram.png)  
- [Hardware Setup](https://github.com/BlaynaFerns/Food-Spoilage-Detection/blob/main/Hardware_Setup.jpg)
- [Layout](https://github.com/BlaynaFerns/Food-Spoilage-Detection/blob/main/Layout.png)
- [Experimental Analysis](https://github.com/BlaynaFerns/Food-Spoilage-Detection/blob/main/Experimental_Analysis.png)  

---

## How to Run

1. Connect all components according to the circuit diagram.  
2. Open the Arduino IDE and upload the provided Arduino code to the **Arduino UNO**.  
3. Ensure your Wi-Fi credentials and ThingSpeak API key are correctly set in the code.  
4. Power the Arduino UNO (via USB or external power).  
5. Monitor live sensor readings on your ThingSpeak channel dashboard.  
6. Test with a methane source (e.g., lighter gas) to trigger alerts on the LCD and buzzer.  

---

## Performance Analysis

#### Result Discussion
- Real-time monitoring on LED and Blynk dashboard.  
- Alerts displayed when methane exceeds safe threshold.  
- Data updates every second.  
- Graphical trends available via Arduino IDE Serial Plotter.

---

## License

*Free to modify and distribute with credit.

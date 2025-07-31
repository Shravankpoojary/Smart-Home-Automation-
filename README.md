# Smart-Home-Automation-and-Monitoring-System
Developed a comprehensive smart home automation and monitoring system aimed at enhancing device communication efficiency and improving overall user control. This project showcased advanced IoT integration, ensuring seamless interaction between various smart devices and real-time monitoring capabilities.

- Brief Description: A system that automates home appliances and monitors environmental factors using IoT and MQTT. Features include gas leak detection, temperature   monitoring, camera surveillance, GSM alerts, and remote control via MQTT.
- Tech Stack: NodeMCU ESP8266, ESP32-CAM, MQTT, GSM SIM800L, MQ-135, NTC Thermistor, Arduino IDE, MQTT IOT Panel app.
- Features:
- Gas leak detection & auto-alert (SMS/call)
- Temperature-based fan control
- Remote appliance control (lights, pump)
- Live camera feed via ESP32-CAM
- User interface via MQTT IOT Panel App
- Screenshots: Images of your final working circuit and app interface.
- How to Run:
- Hardware setup and wiring basics
- Flash code to NodeMCU using Arduino IDE
- MQTT broker setup
- Mobile app config

.



Block Diagram of System 
<img width="908" height="539" alt="image" src="https://github.com/user-attachments/assets/21f16d9c-de69-43af-b5f4-25576dc5172d" />


SOFTWARE REQUIREMENTS 
  -ARDUINO IDE SOFTWARE
  -MQTT IOT Panel app


We used the [MQTT IoT Panel](https://play.google.com/store/apps/details?id=snr.lab.iotmqttpanel.prod&hl=en-US) Android app to visualize and control our smart home system.

**Features:**
- Real-time sensor monitoring (temperature, gas levels)
- Remote control of appliances via MQTT
- Customizable UI with buttons, sliders, graphs
- Secure communication using SSL
- Works with public MQTT brokers

**Setup:**
1. Download the app from [Google Play Store](https://play.google.com/store/apps/details?id=snr.lab.iotmqttpanel.prod&hl=en-US)
2. Configure MQTT broker settings (host, port, username/password)
3. Create panels for each sensor or actuator
4. Subscribe/publish to relevant MQTT topics used in the NodeMCU code

For more details about MQTT IOT Panel APP ,you can find resouces in yotube 
https://youtu.be/DJV2iRtZBKc?si=mzTglI7-XfU_C-my

Hardware Requirements:
🔌 Microcontrollers & Processing Units
- NodeMCU ESP8266: Central controller for sensors and actuators, communicates via MQTT over Wi-Fi.
- ESP32-CAM: Handles live video streaming for surveillance.
📡 Communication Modules
- GSM SIM800L Module: Sends emergency alerts via SMS and voice calls.
- XL6009 DC-DC Booster: Supplies stable voltage for the GSM module.
🌡️ Sensors
- MQ-135 Gas Sensor: Detects harmful gases like CO₂, smoke, and pollutants.
- NTC Thermistor (10kΩ): Measures ambient temperature for fan control.
- 4051 Multiplexer IC: Handles multiple analog sensor inputs efficiently.
⚙️ Actuators
- 5V 2-Channel Relay Module: Switches high-power devices like bulbs and water pumps.
- SG90 Micro Servo Motor: Used for automated window opening during gas leaks.
- Cooling Fan & Water Pump: Controlled based on sensor inputs.
🔋 Power Supply
- 5V 2A DC Regulated Adapter
- USB-B Cable: For uploading code and powering NodeMCU.
📯 Miscellaneous Components
- Resistors: 10kΩ, 100Ω
- Capacitors: 10μF (polarized)
- Buzzer (for audible alerts)
- Trim Potentiometer (for sensor calibration)
- PCB (for permanent circuit setup)
- SIM card (for GSM module)[SIM800L Module]

Final System Snapshot-
<img width="932" height="819" alt="image" src="https://github.com/user-attachments/assets/71a08f5f-8faa-453c-a8b0-4f9031d094fc" />

User Interface of MQTT IOT Panel APP Snapshot-
<img width="478" height="1109" alt="image" src="https://github.com/user-attachments/assets/12ecac37-da31-44b7-8821-ddbeda6bf641" />

The below Link contains complete working of proposed System uploaded in Linkedin Platform
https://www.linkedin.com/posts/shravan-k-poojary_esp8266-mqtt-iotdevelopment-activity-7291805551091400704-qkn5?utm_source=share&utm_medium=member_desktop&rcm=ACoAAEPmrFIBub-IOYlbFXlMHDWjRCAxiuouV_c

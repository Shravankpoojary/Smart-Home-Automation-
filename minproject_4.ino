#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <Wire.h>
#include <PubSubClient.h>
#include <math.h>
#include <SoftwareSerial.h>  // Use SoftwareSerial to communicate with the SIM800 module
#include <Servo.h>

WiFiClient espClient;
PubSubClient client(espClient);

// Recipient phone number for GSM
const char* phoneNumber = "9880097696";

#define relayPin1 13  // D7
#define relayPin 16  // D0
#define RELAY_PIN 14  // D5
#define buzzer 12  // D6
#define multiplexerPin1 2  // GPIO D4
#define multiplexerPin2 4  // GPIO D2
#define multiplexerPin3 5  // GPIO D1
#define analogPin A0  // A0 for analog input

Servo myServo;
const float thresholdTemperature = 40.0; // Example: 30°C
const float gas_level_threshold = 300.0;
#define wifi_ssid "vivo Y17s"
#define wifi_password "anxxtha8877"
#define mqtt_server "broker.hivemq.com"
const char* device_id = "shravan@hivemqg96"; // Manually set device ID
const char* mqtt_user = "anitha"; // Your MQTT username
const char* mqtt_password = "miniproject"; // Your MQTT password
#define temperature_celsius_topic1 "sensor/NTC/temperature_celsius"
#define gas_level_topic "sensor/gas_level"
const char* relay1_topic = "mqtt/relay1";
const char* relay2_topic = "mqtt/relay2";

SoftwareSerial mySerial(3, 1); // RX, TX

double Thermistor(int data) {
  double temp;
  // Calculate resistance (reverse logic)
  temp = log(10000.0 * (data / (1024.0 - data)));
  // Convert resistance to temperature
  temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp)) * temp);
  temp = temp - 273.15; // Convert Kelvin to Celsius

  // Print Celsius temperature
  Serial.print("NTC Temperature: ");
  Serial.print(temp);
  Serial.println(" Celsius");

  return temp; // Return the Celsius temperature value
}

void setup() {
  Serial.begin(115200);
  // mySerial.begin(9600);
  pinMode(relayPin, OUTPUT); // used 
  pinMode(relayPin1, OUTPUT); // used
  digitalWrite(relayPin, LOW);
  digitalWrite(relayPin1, LOW);
  myServo.attach(0);  // Attach servo to pin 2
  myServo.write(0); // Initial position

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(buzzer, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT); // Initialize the relay pin as an output
  digitalWrite(RELAY_PIN, LOW); // Ensure the relay is off to start
  pinMode(multiplexerPin1, OUTPUT);
  pinMode(multiplexerPin2, OUTPUT);
  pinMode(multiplexerPin3, OUTPUT);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(message);

  if (message == "ON") {
    digitalWrite(relayPin, HIGH); // Turn relay on
  } else if (message == "OFF") {
    digitalWrite(relayPin, LOW); // Turn relay off
  } else if (String(topic) == relay2_topic) { // Check if the message is for relay 2
    if (message == "ONN") {
      digitalWrite(relayPin1, HIGH); // Turn relay 2 on
      Serial.println("RELAY 2 is ON");
    } else if (message == "OFFF") {
      digitalWrite(relayPin1, LOW); // Turn relay 2 off
      Serial.println("RELAY 2 is OFF");
    }
  }
}

void setup_wifi() {
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(device_id, mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(relay1_topic);
      client.subscribe(relay2_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Select NTC Thermistor (IN0)
  digitalWrite(multiplexerPin1, LOW);
  digitalWrite(multiplexerPin2, LOW);
  digitalWrite(multiplexerPin3, LOW);
  delay(10);
  int ntcValue = analogRead(analogPin);
  double ntcTemp = Thermistor(ntcValue);

  // Select MQ-135 Gas Sensor (IN1)
  digitalWrite(multiplexerPin1, HIGH);
  digitalWrite(multiplexerPin2, LOW);
  digitalWrite(multiplexerPin3, LOW);
  delay(10);
  int gasValue = analogRead(analogPin);
  Serial.print("MQ-135 Gas Sensor: ");
  Serial.println(gasValue);
  delay(1000); // Delay for 1 second

  if (ntcTemp > thresholdTemperature) {
    digitalWrite(RELAY_PIN, LOW);
  } else {
    digitalWrite(RELAY_PIN, HIGH);
  }

  if (gasValue > gas_level_threshold) {
     if (myServo.read() != 0) { // Only write if not already in position
      myServo.write(0); // Rotate servo back to 0 degrees
    }
    digitalWrite(buzzer, HIGH);
    sendSMS(phoneNumber, "GAS LEAKAGE OCCURED.");  // Send an SMS
    makeCall(phoneNumber); // Make a call
  } else {
      if (myServo.read() != 130) { // Only write if not already in position
      myServo.write(130); // Rotate servo to 120 degrees and keep it there
    }
    digitalWrite(buzzer, LOW);
  }

  publish_sensor_data(ntcTemp, gasValue);
}

void publish_sensor_data(double ntcTemp, int gasValue) {
  if (client.publish(temperature_celsius_topic1, String(ntcTemp).c_str(), true)) {
    Serial.println("Temperature published");
  } else {
    Serial.println("Failed to publish temperature");
  }

  if (client.publish(gas_level_topic, String(gasValue).c_str(), true)) {
    Serial.println("Gas level published");
  } else {
    Serial.println("Failed to publish gas level");
  }
}

// Function to send an SMS
void sendSMS(String number, String message) {
  mySerial.print("AT+CMGF=1"); // Set SMS mode to text
  mySerial.print("\r");
  delay(1000);
  mySerial.print("AT+CMGS=\"");
  mySerial.print(number);
  mySerial.print("\"\r");
  delay(1000);
  mySerial.print(message);
  mySerial.print("\r");
  delay(1000);
  mySerial.write(26); // ASCII code for Ctrl+Z to send SMS
  delay(1000);
  Serial.println("SMS sent to " + number + ": " + message);
}

// Function to make a call
void makeCall(String number) {
  mySerial.print("ATD");
  mySerial.print(number);
  mySerial.print(";");
  mySerial.print("\r");
  Serial.println("Calling " + number + "...");
}

/**
 * Basic Write Example code for InfluxDBClient library for Arduino
 * Data can be immediately seen in a InfluxDB UI: wifi_status measurement
 * Enter WiFi and InfluxDB parameters below
 *
 * Measures signal level of the actually connected WiFi network
 * This example supports only InfluxDB running from unsecure (http://...)
 * For secure (https://...) or Influx Cloud 2 use SecureWrite example
 **/

#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>

//WiFi.begin("true_home2G_dfb", "NMcFfdxM");
// WiFi AP SSID
//#define WIFI_SSID "true_home2G_dfb"
// WiFi password
//#define WIFI_PASSWORD "NMcFfdxM"

#define WIFI_SSID "ASUS_60"
#define WIFI_PASSWORD "RoadRunner"


// For InfluxDB 1 e.g. http://192.168.1.48:8086
#define INFLUXDB_URL "http://172.16.102.140:8086"

// InfluxDB v1 database name 
#define INFLUXDB_DB_NAME "banpot_db"

#include "DHT.h"

DHT dht;

// InfluxDB client instance
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_DB_NAME);

// Data point this is table name
Point sensor("evergrow_sensor_test");
float count = 0;

void setup() {
  Serial.begin(115200);
  dht.setup(D0); // data pin D0
  // Connect WiFi
  Serial.println("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();


  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void loop() {

// delay(dht.getMinimumSamplingPeriod());
// float humidity = dht.getHumidity();
// float temperature = dht.getTemperature();
/*
    sensor.clearFields();
                      field name, field value
    sensor.addField("SensorType", "SHT15");
    sensor.addField("Tenp_Value", 25.0);
    sensor.addField("Humi_Value", 50.0);
    sensor.addField("SensorType", "dht22");
    sensor.addField("Tenp_Value", 23.4);
    sensor.addField("Humi_Value", 56.7);
 */
 /*
 // if(dht.getStatusString() == "OK"){
    sensor.clearFields();
    sensor.addField("SensorType", "SHT15");
    sensor.addField("Tenp_Value", 25.0);
    sensor.addField("Humi_Value", 50.0);
    sensor.addField("SensorType", "dht22");
    sensor.addField("Tenp_Value", 23.4);
    sensor.addField("Humi_Value", 56.7);
//  }

  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());
  // If no Wifi signal, try to reconnect it
  if ((WiFi.RSSI() == 0) && (wifiMulti.run() != WL_CONNECTED))
    Serial.println("Wifi connection lost");
  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
  */
  write_db(1, 12.3, 45.6);
  Serial.println("Wait 1s");
  delay(1000);
  write_db(2, 78.9, 10.1);
  Serial.println("Wait 1s");
  delay(1000);
}

void write_db(unsigned char sensor_type, float temp_value, float humi_value){
  sensor.clearFields();
  if(sensor_type == 1){
    sensor.addField("SensorType", "SHT15");
    sensor.addField("Tenp_Value", temp_value);
    sensor.addField("Humi_Value", humi_value);
    db_writePoint();
  }else if(sensor_type == 2){
    sensor.addField("SensorType", "dht22");
    sensor.addField("Tenp_Value", temp_value);
    sensor.addField("Humi_Value", humi_value);
    db_writePoint();
  }
}

void db_writePoint(){
  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());
  if ((WiFi.RSSI() == 0) && (wifiMulti.run() != WL_CONNECTED)){
    Serial.println("Wifi connection lost");
  }
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

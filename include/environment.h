//
// Created by thorgan on 3/15/25.
//
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

// #################################
// #  Specific device's constants  #
// #################################
/*
 * UUIDs for Boards:
 *
 * 00a4-43bb-8b77 -> ESP32 Light device 2
 * af6f-43b1-a20e -> ESP32 Light device 1 (HERE)
 * db87-47b7-998d
 * af66-4ec1-b57a
 * 52db-49fa-ads7
 * 3e04-4d88-9130
 * 9c6d-4001-945b
 * ee30-4e17-be44
 * b51e-43ac-8603
 * b406-4eaa-8776
 * a80a-4057-9ef0
 * c8ae-462b-8952
 * 0158-4d61-85c2
 * b817-488c-9b5d
 * 808e-4c62-9c4f
 */
#define DEVICE_ID "ESP32-af6f-43b1-a20e"  // UNIQUE IDENTIFIER -> DO NOT GIVE IT TO ANY OTHER BOARD!
#define SYSTEM_NAME "home"
#define DEVICE_TYPE "light"
#define DEFAULT_LOCATION "room"
#define DEFAULT_LOCATION_ID 1
#define BAUD_RATE 1000000

// PINS FOR ACTUATORS / SENSORS
// Light
#define LIGHT 22
#define LIGHT_VOLTAGE 23

// Presence detector
#define PRESENCE_DETECTOR_PIN 18
#define PRESENCE_DETECTOR_VOLTAGE 19

// Temperature/Humidity sensor
#define TEMPERATURE_SENSOR_PIN 14
#define TEMPERATURE_SENSOR_VOLTAGE 12

// #################################
// #    Time-related Constants     #
// #################################
static unsigned long SECOND_MILLIS = 1000;
static unsigned long MINUTE_MILLIS = 60 * SECOND_MILLIS;
static unsigned long HOUR_MILLIS = 60 * MINUTE_MILLIS;
static unsigned long DAY_MILLIS = 24 * HOUR_MILLIS;


// #################################
// #     Network / MQTT Broker     #
// #################################
static char WIFI_SSID[] = "@Hermes";
static char WIFI_PASSWORD[] = "password";
static char MQTT_BROKER_ADDRESS[] = "10.3.141.1";
static int MQTT_PORT = 1883;
static char MQTT_USERNAME[] = "";  // CHANGE IT IF REQUIRED
static char MQTT_PASSWORD[] = "";  // CHANGE IT IF REQUIRED

#define QOS 2
#define RETAIN false

// DEBUG MODE (comment the define DEBUG_MODE 1 to disable debug mode)
#define DEBUG_MODE 1

#ifdef DEBUG_MODE
    #define DEBUG_MODE_PRINT(x)     Serial.print (x)
    #define DEBUG_MODE_PRINTDEC(x)  Serial.print (x, DEC)
    #define DEBUG_MODE_PRINTHEX(x)  Serial.print (x, HEX)
    #define DEBUG_MODE_PRINTLN(x)   Serial.println (x)
    #define DEBUG_MODE_PRINTF(...)    Serial.printf (__VA_ARGS__)
#else
    #define DEBUG_MODE_PRINT(x)
    #define DEBUG_MODE_PRINTDEC(x)
    #define DEBUG_MODE_PRINTHEX(x)
    #define DEBUG_MODE_PRINTLN(x)
    #define DEBUG_MODE_PRINTF(...)
#endif

// Standard MQTT topics that this device should publish/subscribe to
// PUBLISH topics
#define STARTUP_TOPIC "startup"

// SUBSCRIBE topics
#define SETUP_TOPIC "setup"
#define RESET_TOPIC "reset"

// #################################
// #   Modules (Name / Variable)   #
// #################################
// Actuators
#define LIGHT_CONTROLLER "lightController"

// Sensors
#define LIGHT_SENSOR "lightSensor"
#define LUMINOSITY_SENSOR "luminositySensor"
#define PRESENCE_DETECTOR "presenceDetector"
#define TEMPERATURE_SENSOR "temperatureSensor"
#define CONSUMPTION_SENSOR "consumptionSensor"

#endif //ENVIRONMENT_H
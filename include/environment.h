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
#define BAUD_RATE 9600

// PINS FOR ACTUATORS / SENSORS
#define LIGHT 5

// #################################
// #    Time-related Constants     #
// #################################
static unsigned long SECOND_MILLIS = 1000;
static unsigned long MINUTE_MILLIS = 60 * SECOND_MILLIS;
static unsigned long HOUR_MILLIS = 60 * MINUTE_MILLIS;
static unsigned long DAY_MILLIS = 24 * HOUR_MILLIS;

// #define SECOND_MILLIS 1000UL;
// #define MINUTE_MILLIS 60000UL;
// #define HOUR_MILLIS 3600000UL;
// #define DAY_MILLIS 86400000UL;


// #################################
// #     Network / MQTT Broker     #
// #################################
static char WIFI_SSID[] = "@Hermes";
static char WIFI_PASSWORD[] = "2kW%s@Vwqqy3ROkeU9&U%Lgi3e6U%u2OnwgNn@Gtrx$n30mhdy";
static char MQTT_BROKER_ADDRESS[] = "10.3.141.1";
static int MQTT_PORT = 1883;
static char MQTT_USERNAME[] = "";  // CHANGE IT IF REQUIRED
static char MQTT_PASSWORD[] = "";  // CHANGE IT IF REQUIRED

#define QOS 2
#define RETAIN true

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
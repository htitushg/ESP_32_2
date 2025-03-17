//
// Created by thorgan on 3/16/25.
//
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <environment.h>
#include <utils.h>
#include "Application.h"

#include <LightController.h>
#include <ModuleFactory.h>

void Application::initialize(const WiFiClient& wifi) {

  // DEBUG
  Serial.println("Initializing application...");

  this->network = wifi;
  this->broker = Broker::newBroker(this->network, Application::messageHandler);
  this->setRootTopic();
  this->lightSensor = ModuleFactory::newModule(this->broker, LIGHT_SENSOR);
  this->luminositySensor = ModuleFactory::newModule(this->broker, LUMINOSITY_SENSOR);
  this->presenceDetector = ModuleFactory::newModule(this->broker, PRESENCE_DETECTOR);
  this->temperatureSensor = ModuleFactory::newModule(this->broker, TEMPERATURE_SENSOR);
  this->consumptionSensor = ModuleFactory::newModule(this->broker, CONSUMPTION_SENSOR);
  this->lightController = ModuleFactory::newModule(this->broker, LIGHT_CONTROLLER);

  // Add references to lightSensor and luminositySensor values to lightController
  const auto* light_controller = (LightController *)(this->lightController);
  light_controller->addReference(new ValueReference(LIGHT_SENSOR, this->lightSensor->getValueReference()));
  light_controller->addReference(new ValueReference(LUMINOSITY_SENSOR, this->luminositySensor->getValueReference()));

  this->presenceDetector->Attach(this->lightController);
}

void Application::setRootTopic() {

  // DEBUG
  Serial.println("Defining root topic...");

  // Define the root_topic
  this->root_topic = String(SYSTEM_NAME) + "/" + String(this->location) + "/" + String(this->locationID) + "/" + String(DEVICE_TYPE) + "/" + String(DEVICE_ID) + "/";

  // Define the broker's root_topic
  this->broker->setRootTopic(this->root_topic);

  // DEBUG
  Serial.printf("Root topic set at: %s\n", this->root_topic.c_str());
}

Application::Application() = default;

void Application::messageHandler(MQTTClient *client, char topic[], char payload[], int length) {

    Application *app = Application::getInstance();

    // Extract the module name from the topic
    const String channel = getChannelModule(topic);

    // DEBUG
    Serial.println("|<<<-=================================");
    Serial.printf("| %s - received from MQTT:\n", DEVICE_ID);
    Serial.printf("|====-> topic: %s\n", topic);
    Serial.printf("|====-> channel: %s\n", channel.c_str());
    Serial.printf("|====-> payload: %s\n", payload);
    Serial.printf("|====-> length: %d\n\n", length);

    if (strCaseSensitiveCompare(channel, LIGHT_CONTROLLER)) {
        app->lightController->setValue(payload);

    } else if (strCaseSensitiveCompare(channel, LIGHT_SENSOR)) {
        app->lightSensor->setValue(payload);

    } else if (strCaseSensitiveCompare(channel, LUMINOSITY_SENSOR)) {
        app->luminositySensor->setValue(payload);

    } else if (strCaseSensitiveCompare(channel, PRESENCE_DETECTOR)) {
        app->presenceDetector->setValue(payload);

    } else if (strCaseSensitiveCompare(channel, TEMPERATURE_SENSOR)) {
        app->temperatureSensor->setValue(payload);

    } else if (strCaseSensitiveCompare(channel, CONSUMPTION_SENSOR)) {
        app->consumptionSensor->setValue(payload);

    } else if (strCaseSensitiveCompare(channel, SETUP_TOPIC)) {
        app->onSetupMessage(payload);

    } else if (strCaseSensitiveCompare(channel, RESET_TOPIC)) {
            const bool position = parseBool(payload);
            if (position) { app->reset(); }
    }
}

void Application::onSetupMessage(char payload[]) {

  // DEBUG
  Serial.println("Recieved setup message");

  // Parse setup message
  if (this->isWaitingForSetup()) {

    // DEBUG
    Serial.println("Setup mode: deserializing message...");

    JsonDocument doc = JsonDocument();
    int length;

    DeserializationError error = deserializeJson(doc, payload, length);

    if (error) {

      // DEBUG
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    // const char* id = doc["id"]; // "ESP32-00a4-43bb-8b77"
    // const char* type = doc["type"]; // "light"
    const char* loc_id = doc["location_id"]; // 1
    this->locationID = parseInt(loc_id);
    const char* loc_type = doc["location_type"]; // "room"
    this->location = String(loc_type);
    // const char* loc_name = doc["location_name"]; // "John's bedroom"

    for (JsonObject module : doc["modules"].as<JsonArray>()) {

      const char* module_name = module["name"]; // "lightController", "lightSensor", "luminositySensor", ...
      const char* module_value = module["value"]; // "True", "True", "500", "True", "22.7", "25.46"

      this->setupModule(module_name, module_value);
    }

  	// DEBUG
  	Serial.println("Unsubscribing temporary setup topic...");

    this->broker->unsub(SETUP_TOPIC);

    // Set the root topic with new values
    this->setRootTopic();

    // DEBUG
    Serial.println("Completing setup...");

    this->wait_for_setup = false;
  }
}

void Application::setupModule(const char* name, const char* value) const {

  // DEBUG
  Serial.printf("Setting up module %s...\n", name);

    if (name == nullptr || value == nullptr) return;
    if (strCaseSensitiveCompare(name, LIGHT_CONTROLLER)) this->lightController->setValue(value);
    else if (strCaseSensitiveCompare(name, LIGHT_SENSOR)) this->lightSensor->setValue(value);
    else if (strCaseSensitiveCompare(name, LUMINOSITY_SENSOR)) this->luminositySensor->setValue(value);
    else if (strCaseSensitiveCompare(name, PRESENCE_DETECTOR)) this->presenceDetector->setValue(value);
    else if (strCaseSensitiveCompare(name, TEMPERATURE_SENSOR)) this->temperatureSensor->setValue(value);
    else if (strCaseSensitiveCompare(name, CONSUMPTION_SENSOR)) this->consumptionSensor->setValue(value);
}

void Application::startup() {

  // DEBUG
  Serial.println("Starting up...");
  Serial.println("Preparing startup message...");

  // Prepare startup message
  JsonDocument message = JsonDocument();
  message["id"] = DEVICE_ID;
  message["type"] = DEVICE_TYPE;
  message["locationID"] = this->locationID;
  message["location_type"] = this->location;
  message["location_name"] = String(this->location) + " " + String(this->locationID);

  JsonDocument modules = JsonDocument();
  JsonArray modules_array = modules.to<JsonArray>();

  JsonDocument light_controller_document = JsonDocument();
  JsonObject light_controller_object = light_controller_document.to<JsonObject>();
  light_controller_object["name"] = LIGHT_CONTROLLER;
  light_controller_object["value"] = this->lightController->getValue();
  modules_array.add(light_controller_document);

  JsonDocument light_sensor_document = JsonDocument();
  JsonObject light_sensor_object = light_sensor_document.to<JsonObject>();
  light_sensor_object["name"] = LIGHT_SENSOR;
  light_sensor_object["value"] = this->lightSensor->getValue();
  modules_array.add(light_sensor_document);

  JsonDocument luminosity_sensor_document = JsonDocument();
  JsonObject luminosity_sensor_object = luminosity_sensor_document.to<JsonObject>();
  luminosity_sensor_object["name"] = LUMINOSITY_SENSOR;
  luminosity_sensor_object["value"] = this->luminositySensor->getValue();
  modules_array.add(luminosity_sensor_document);

  JsonDocument presence_detector_document = JsonDocument();
  JsonObject presence_detector_object = presence_detector_document.to<JsonObject>();
  presence_detector_object["name"] = PRESENCE_DETECTOR;
  presence_detector_object["value"] = this->presenceDetector->getValue();
  modules_array.add(presence_detector_document);

  JsonDocument temperature_sensor_document = JsonDocument();
  JsonObject temperature_sensor_object = temperature_sensor_document.to<JsonObject>();
  temperature_sensor_object["name"] = TEMPERATURE_SENSOR;
  temperature_sensor_object["value"] = this->temperatureSensor->getValue();
  modules_array.add(temperature_sensor_document);

  JsonDocument consumption_sensor_document = JsonDocument();
  JsonObject consumption_sensor_object = consumption_sensor_document.to<JsonObject>();
  consumption_sensor_object["name"] = CONSUMPTION_SENSOR;
  consumption_sensor_object["value"] = this->consumptionSensor->getValue();
  modules_array.add(consumption_sensor_document);

  message["modules"] = modules_array;

  // DEBUG
  Serial.println("Serializing startup message...");

  char messageBuffer[512];
  serializeJson(message, messageBuffer);

  // DEBUG
  Serial.printf("JSON payload:\n%s\n", messageBuffer);

  // DEBUG
  Serial.println("Subscribing to temporary setup channel...");

  this->broker->sub(SETUP_TOPIC);
  this->wait_for_setup = true;

  // DEBUG
  Serial.println("Publishing to startup topic...");

  this->broker->pub(STARTUP_TOPIC, messageBuffer);

  // DEBUG
  Serial.println("Waiting for setup message...");

  // Wait for MQTT Broker response
  while (this->wait_for_setup) { this->broker->loop(); }

  // DEBUG
  Serial.println("Subscribing to topics...");

  // Subscribe to MQTT topics
  this->broker->sub(RESET_TOPIC);
  this->broker->sub(LIGHT_CONTROLLER);
  this->broker->sub(LIGHT_SENSOR);
  this->broker->sub(LUMINOSITY_SENSOR);
  this->broker->sub(PRESENCE_DETECTOR);
  this->broker->sub(TEMPERATURE_SENSOR);
  this->broker->sub(CONSUMPTION_SENSOR);
}

void Application::unsubscribeAllTopics() const {

  // TODO -> maybe return to setup here? Is it necessary?...
  // if (this->broker == nullptr) return;

  // DEBUG
  Serial.println("Unsubscribing all topics...");

  this->broker->unsub(RESET_TOPIC);
  this->broker->unsub(LIGHT_CONTROLLER);
  this->broker->unsub(LIGHT_SENSOR);
  this->broker->unsub(LUMINOSITY_SENSOR);
  this->broker->unsub(PRESENCE_DETECTOR);
  this->broker->unsub(TEMPERATURE_SENSOR);
  this->broker->unsub(CONSUMPTION_SENSOR);
}

void Application::reset() {

  // DEBUG
  Serial.println("Resetting...");

  // Unsubscribe to MQTT channels
  this->unsubscribeAllTopics();

  // Call the startup() function:
  this->startup();
}

Application *Application::app = nullptr;

Application * Application::getInstance() {
    if (app == nullptr) return new Application();
    return app;
}

void Application::brokerLoop() const {
  this->broker->loop();
}

void Application::sensorLoop() {

  // if (millis() - lastPublishTime > PUBLISH_INTERVAL) {
  //   // TODO -> use this kind of condition for sensor check
  //   lastPublishTime = millis();
  // }

  delay(500);
}

// FIXME/TODO -> remove this function (was only to debug connections)
//void Application::MQTTLoop( void * parameter) {
//  for(;;) {
//    this->broker->loop();
//
//     if (millis() - this->lastPublishTime > this->publish_interval) {
//       Serial.printf("WiFi status: %s\n", wl_status_to_string(WiFi.status()));
//       this->lastPublishTime = millis();
//     }
//
//
//    if (!this->mqtt.connected()) {
//
//      // DEBUG
//      Serial.println("MQTT disconnected");
//      Serial.printf("MQTT status: %s\n", this->mqtt.sessionPresent() ? "on" : "off");
//      Serial.println("Attempting MQTT connection...");
//
//      connectToMQTT();
//    }
//
//    delay(1000);
//  }
//}
//
// Created by thorgan on 3/16/25.
//

#include <Arduino.h>
#include <ArduinoJson.h>
#include <environment.h>
#include <utils.h>
#include "Application.h"

#include <LightController.h>
#include <ModuleFactory.h>

void Application::initialize(const WiFiClient &wifi) {
    // DEBUG
    Serial.println("Initializing application...");

    this->a_network = wifi;
    this->a_broker = Broker::newBroker(& this->a_network, Application::messageHandler);
    this->setRootTopic();

    this->a_modules[light_controller].reset(ModuleFactory::newModule(this->a_broker.get(), LIGHT_CONTROLLER));
    this->a_modules[light_sensor].reset(ModuleFactory::newModule(this->a_broker.get(), LIGHT_SENSOR));
    this->a_modules[temperature_sensor].reset(ModuleFactory::newModule(this->a_broker.get(), TEMPERATURE_SENSOR));
    this->a_modules[presence_detector].reset(ModuleFactory::newModule(this->a_broker.get(), PRESENCE_DETECTOR));
    this->a_modules[consumption_sensor].reset(ModuleFactory::newModule(this->a_broker.get(), CONSUMPTION_SENSOR));
    this->a_modules[luminosity_sensor].reset(ModuleFactory::newModule(this->a_broker.get(), LUMINOSITY_SENSOR));

    // Add references to lightSensor and luminositySensor values to lightController
    this->a_modules[light_controller]->addReference(
        this->a_modules[light_sensor]->getValueReference(),
        String(LIGHT_SENSOR));

    this->a_modules[light_controller]->addReference(
        this->a_modules[luminosity_sensor]->getValueReference(),
        String(LUMINOSITY_SENSOR));

    this->a_modules[presence_detector]->Attach(this->a_modules[light_controller].get());
}

void Application::setRootTopic() {
    // DEBUG
    Serial.println("Defining root topic...");

    // Define the root_topic
    this->a_root_topic = String(SYSTEM_NAME) + "/" + String(this->a_location) + "/" +
                         String(this->a_locationID) + "/" + String(DEVICE_TYPE) + "/" + String(
                             DEVICE_ID) + "/";

    // Define the broker's root_topic
    this->a_broker->setRootTopic(this->a_root_topic);

    // DEBUG
    Serial.printf("Root topic set at: %s\n", this->a_root_topic.c_str());
}

Application::Application() = default;

void Application::messageHandler(MQTTClient *client, char topic[], char payload[], int length) {

    // DEBUG
    Serial.printf("Message arrived on topic: %s with length %d\n", topic, length);
    Serial.printf("payload:\n%s\n", payload);
    Serial.println("Retrieving application instance...");

    std::shared_ptr<Application> app = Application::getInstance();

    // DEBUG
    Serial.println("Retrieving module name from the topic...");

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
        app->a_modules[light_controller]->setValue(payload);
    } else if (strCaseSensitiveCompare(channel, LIGHT_SENSOR)) {
        app->a_modules[light_sensor]->setValue(payload);
    } else if (strCaseSensitiveCompare(channel, LUMINOSITY_SENSOR)) {
        app->a_modules[luminosity_sensor]->setValue(payload);
    } else if (strCaseSensitiveCompare(channel, PRESENCE_DETECTOR)) {
        app->a_modules[presence_detector]->setValue(payload);
    } else if (strCaseSensitiveCompare(channel, TEMPERATURE_SENSOR)) {
        app->a_modules[temperature_sensor]->setValue(payload);
    } else if (strCaseSensitiveCompare(channel, CONSUMPTION_SENSOR)) {
        app->a_modules[consumption_sensor]->setValue(payload);
    } else if (strCaseSensitiveCompare(channel, SETUP_TOPIC)) {
        app->onSetupMessage(std::string(payload));
    } else if (strCaseSensitiveCompare(channel, RESET_TOPIC)) {
        const bool position = parseBool(payload);
        if (position) { app->reset(); }
    }
}

void Application::onSetupMessage(std::string & payload) {
    // DEBUG
    Serial.println("Received setup message");

    // Parse setup message
    if (this->isWaitingForSetup()) {
        // DEBUG
        Serial.println("Setup mode: deserializing message...");

        auto doc = JsonDocument();

        const DeserializationError error = deserializeJson(doc, payload);

        if (error) {
            // DEBUG
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }

        // const char* id = doc["id"]; // "ESP32-00a4-43bb-8b77"
        // const char* type = doc["type"]; // "light"
        const char *loc_id = doc["location_id"]; // 1
        this->a_locationID = parseInt(loc_id);

        // DEBUG
        Serial.printf("Updated location ID: %u\n", this->a_locationID);

        const char *loc_type = doc["location_type"]; // "room"
        this->a_location = String(loc_type);

        // DEBUG
        Serial.printf("Updated location type: %s\n", this->a_location.c_str());

        // const char* loc_name = doc["location_name"]; // "John's bedroom"

        for (JsonObject module: doc["modules"].as<JsonArray>()) {
            const char *module_name = module["name"];
            // "lightController", "lightSensor", "luminositySensor", ...
            const char *module_value = module["value"];
            // "True", "True", "500", "True", "22.7", "25.46"

            this->setupModule(module_name, module_value);
        }

        // DEBUG
        Serial.println("Unsubscribing temporary setup topic...");

        this->a_broker->unsub(SETUP_TOPIC);

        // Set the root topic with new values
        this->setRootTopic();

        // DEBUG
        Serial.println("Completing setup...");

        this->a_wait_for_setup = false;
    }
}

void Application::setupModule(const char * name, const char * value) const {
    // DEBUG
    Serial.printf("Setting up module %s with value '%s'...\n", name, value);

    if (name == nullptr || value == nullptr) return;

    if (strCaseSensitiveCompare(name, LIGHT_CONTROLLER))
        this->a_modules[light_controller]->setValue(value);

    else if (strCaseSensitiveCompare(name, LIGHT_SENSOR))
        this->a_modules[light_sensor]->setValue(value);

    else if (strCaseSensitiveCompare(name, LUMINOSITY_SENSOR))
        this->a_modules[luminosity_sensor]->setValue(value);

    else if (strCaseSensitiveCompare(name, PRESENCE_DETECTOR))
        this->a_modules[presence_detector]->setValue(value);

    else if (strCaseSensitiveCompare(name, TEMPERATURE_SENSOR))
        this->a_modules[temperature_sensor]->setValue(value);

    else if (strCaseSensitiveCompare(name, CONSUMPTION_SENSOR))
        this->a_modules[consumption_sensor]->setValue(value);
}

void Application::startup() {
    // DEBUG
    Serial.println("Starting up...");
    Serial.println("Preparing startup message...");

    // Prepare startup message
    JsonDocument message = JsonDocument();
    message["id"] = DEVICE_ID;
    message["type"] = DEVICE_TYPE;
    message["locationID"] = this->a_locationID;
    message["location_type"] = this->a_location;
    message["location_name"] = String(this->a_location) + " " + String(this->a_locationID);

    JsonDocument modules = JsonDocument();
    JsonArray modules_array = modules.to<JsonArray>();

    JsonDocument light_controller_document = JsonDocument();
    JsonObject light_controller_object = light_controller_document.to<JsonObject>();
    light_controller_object["name"] = LIGHT_CONTROLLER;
    light_controller_object["value"] = this->a_modules[light_controller]->getValue();
    modules_array.add(light_controller_document);

    JsonDocument light_sensor_document = JsonDocument();
    JsonObject light_sensor_object = light_sensor_document.to<JsonObject>();
    light_sensor_object["name"] = LIGHT_SENSOR;
    light_sensor_object["value"] = this->a_modules[light_sensor]->getValue();
    modules_array.add(light_sensor_document);

    JsonDocument luminosity_sensor_document = JsonDocument();
    JsonObject luminosity_sensor_object = luminosity_sensor_document.to<JsonObject>();
    luminosity_sensor_object["name"] = LUMINOSITY_SENSOR;
    luminosity_sensor_object["value"] = this->a_modules[luminosity_sensor]->getValue();
    modules_array.add(luminosity_sensor_document);

    JsonDocument presence_detector_document = JsonDocument();
    JsonObject presence_detector_object = presence_detector_document.to<JsonObject>();
    presence_detector_object["name"] = PRESENCE_DETECTOR;
    presence_detector_object["value"] = this->a_modules[presence_detector]->getValue();
    modules_array.add(presence_detector_document);

    JsonDocument temperature_sensor_document = JsonDocument();
    JsonObject temperature_sensor_object = temperature_sensor_document.to<JsonObject>();
    temperature_sensor_object["name"] = TEMPERATURE_SENSOR;
    temperature_sensor_object["value"] = this->a_modules[temperature_sensor]->getValue();
    modules_array.add(temperature_sensor_document);

    JsonDocument consumption_sensor_document = JsonDocument();
    JsonObject consumption_sensor_object = consumption_sensor_document.to<JsonObject>();
    consumption_sensor_object["name"] = CONSUMPTION_SENSOR;
    consumption_sensor_object["value"] = this->a_modules[consumption_sensor]->getValue();
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

    this->a_broker->sub(SETUP_TOPIC);
    this->a_wait_for_setup = true;

    // DEBUG
    Serial.println("Publishing to startup topic...");

    this->a_broker->pub(STARTUP_TOPIC, messageBuffer);

    // DEBUG
    Serial.println("Waiting for setup message...");

    // Wait for MQTT Broker response
    while (this->a_wait_for_setup) { this->a_broker->loop(); }

    // DEBUG
    Serial.println("Subscribing to topics...");

    // Subscribe to MQTT topics
    this->a_broker->sub(RESET_TOPIC);
    this->a_broker->sub(LIGHT_CONTROLLER);
    this->a_broker->sub(LIGHT_SENSOR);
    this->a_broker->sub(LUMINOSITY_SENSOR);
    this->a_broker->sub(PRESENCE_DETECTOR);
    this->a_broker->sub(TEMPERATURE_SENSOR);
    this->a_broker->sub(CONSUMPTION_SENSOR);
}

void Application::unsubscribeAllTopics() const {
    // TODO -> maybe return to setup here? Is it necessary?...
    // if (this->a_broker == nullptr) return;

    // DEBUG
    Serial.println("Unsubscribing all topics...");

    this->a_broker->unsub(RESET_TOPIC);
    this->a_broker->unsub(LIGHT_CONTROLLER);
    this->a_broker->unsub(LIGHT_SENSOR);
    this->a_broker->unsub(LUMINOSITY_SENSOR);
    this->a_broker->unsub(PRESENCE_DETECTOR);
    this->a_broker->unsub(TEMPERATURE_SENSOR);
    this->a_broker->unsub(CONSUMPTION_SENSOR);
}

void Application::reset() {
    // DEBUG
    Serial.println("Resetting...");

    // Unsubscribe to MQTT channels
    this->unsubscribeAllTopics();

    // Call the startup() function:
    this->startup();
}

std::shared_ptr<Application> Application::app;

std::shared_ptr<Application> Application::getInstance() {
    if (app == nullptr) app.reset(new Application());
    return app;
}

void Application::brokerLoop() const {
    this->a_broker->loop();
}

[[noreturn]] void Application::sensorLoop() {

    std::shared_ptr<Application> app = Application::getInstance();

    while (true) {
        if (millis() - app->a_lastPublishTime > app->a_publish_interval) {

            // DEBUG
            Serial.println("Running sensor loop...");

            // TODO -> use this kind of condition for sensor check
            app->a_lastPublishTime = millis();
        }

        delay(500);
    }
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

//
// Created by thorgan on 3/16/25.
//

#include <Arduino.h>
#include <nlohmann/json.hpp>
#include <environment.h>
#include <utils.h>
#include "Application.h"
#include <LightController.h>
#include <ModuleFactory.h>

std::shared_ptr<Application> Application::app = Application::getInstance();

std::shared_ptr<Application> Application::getInstance() {
    if (app == nullptr) app.reset(new Application());
    return app;
}

void Application::initialize(const WiFiClient & wifi, void callback(MQTTClient *client, char topic[], char bytes[], int length)) {

    // DEBUG
    DEBUG_MODE_PRINTLN("Initializing application...");

    this->a_network = wifi;
    this->a_broker = Broker::newBroker(& this->a_network, callback);
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
        std::string(LIGHT_SENSOR));

    this->a_modules[light_controller]->addReference(
        this->a_modules[luminosity_sensor]->getValueReference(),
        std::string(LUMINOSITY_SENSOR));

    this->a_modules[presence_detector]->Attach(this->a_modules[light_controller].get());
}

void Application::setRootTopic() {

    // DEBUG
    DEBUG_MODE_PRINTLN("Defining root topic...");

    // Define the root_topic
    this->a_root_topic = std::string(SYSTEM_NAME) + "/" + this->a_location + "/" +
                         toString(this->a_locationID) + "/" + std::string(DEVICE_TYPE) + "/" + std::string(
                             DEVICE_ID) + "/";

    // Define the broker's root_topic
    this->a_broker->setRootTopic(this->a_root_topic);

    // DEBUG
    DEBUG_MODE_PRINTF("Root topic set at: %s\n", this->a_root_topic.c_str());
}

Application::Application() = default;

void Application::messageHandler(char topic[], char payload[], int length) {

    // DEBUG
    DEBUG_MODE_PRINTF("Message arrived on topic: %s with length %d\n", topic, length);
    DEBUG_MODE_PRINTF("payload:\n%s\n", payload);
    DEBUG_MODE_PRINTLN("Retrieving module name from the topic...");

    // Extract the module name from the topic
    const std::string channel = getChannelModule(topic);

    // DEBUG
    DEBUG_MODE_PRINTLN("|<<<-=================================");
    DEBUG_MODE_PRINTF("| %s - received from MQTT:\n", DEVICE_ID);
    DEBUG_MODE_PRINTF("|====-> topic: %s\n", topic);
    DEBUG_MODE_PRINTF("|====-> channel: %s\n", channel.c_str());
    DEBUG_MODE_PRINTF("|====-> payload: %s\n", payload);
    DEBUG_MODE_PRINTF("|====-> length: %d\n\n", length);

    if (strCaseSensitiveCompare(channel, LIGHT_CONTROLLER)) {
        this->a_modules[light_controller]->setValue(std::string(payload));

    } else if (strCaseSensitiveCompare(channel, LIGHT_SENSOR)) {
        this->a_modules[light_sensor]->setValue(std::string(payload));

    } else if (strCaseSensitiveCompare(channel, LUMINOSITY_SENSOR)) {
        this->a_modules[luminosity_sensor]->setValue(std::string(payload));

    } else if (strCaseSensitiveCompare(channel, PRESENCE_DETECTOR)) {
        this->a_modules[presence_detector]->setValue(std::string(payload));

    } else if (strCaseSensitiveCompare(channel, TEMPERATURE_SENSOR)) {
        this->a_modules[temperature_sensor]->setValue(std::string(payload));

    } else if (strCaseSensitiveCompare(channel, CONSUMPTION_SENSOR)) {
        this->a_modules[consumption_sensor]->setValue(std::string(payload));

    } else if (strCaseSensitiveCompare(channel, SETUP_TOPIC)) {
        this->onSetupMessage(std::string(payload));

    } else if (strCaseSensitiveCompare(channel, RESET_TOPIC)) {
        const bool position = parseBool(payload);
        if (position) {
            this->reset();
        }
    }
}

void Application::onSetupMessage(const std::string & payload) {

    // DEBUG
    DEBUG_MODE_PRINTLN("Received setup message");

    // Parse setup message
    if (this->isWaitingForSetup()) {

        // DEBUG
        DEBUG_MODE_PRINTF("Payload received:\n%s\n\n", payload.c_str());
        DEBUG_MODE_PRINTLN("Setup mode: deserializing message...");

        auto doc = nlohmann::json::parse(payload);

        // const char* id = doc["id"]; // "ESP32-00a4-43bb-8b77"
        // const char* type = doc["type"]; // "light"
        this->a_locationID = doc["location_id"].get<int>(); // 1

        // DEBUG
        DEBUG_MODE_PRINTF("Updated location ID: %u\n", this->a_locationID);

        this->a_location = doc["location_type"].get<std::string>(); // "room"

        // DEBUG
        DEBUG_MODE_PRINTF("Updated location type: %s\n", this->a_location.c_str());
        DEBUG_MODE_PRINTF("New location name: %s\n", doc["location_name"].get<std::string>().c_str());

        // const char* loc_name = doc["location_name"]; // "John's bedroom"

        if (!doc["modules"].empty()) {
            if (doc["modules"].is_array()) {
                for (auto & module: doc["modules"]) {

                    // DEBUG
                    DEBUG_MODE_PRINTF("Loop: module %s\n", module["name"].get<std::string>().c_str());

                    this->setupModule(module);
                }
            } else { DEBUG_MODE_PRINTLN("Error: modules is not an array!"); }
        } else { DEBUG_MODE_PRINTLN("Error: no modules found!"); }

        // DEBUG
        DEBUG_MODE_PRINTLN("Unsubscribing temporary setup topic...");

        this->a_broker->unsub(SETUP_TOPIC);

        // Set the root topic with new values
        this->setRootTopic();

        // DEBUG
        DEBUG_MODE_PRINTLN("Completing setup...");

        this->a_wait_for_setup = false;
    }
}

void Application::setupModule(nlohmann::json & module) const {

    const std::string name = module["name"].get<std::string>();

    // DEBUG
    DEBUG_MODE_PRINTF("Setting up module %s...\n", name.c_str());
    DEBUG_MODE_PRINTF("Value exists: %s\n", toString(!module["value"].empty()).c_str());
    DEBUG_MODE_PRINTF("Value type: '%s'...\n", module["value"].type_name());

    const std::string value = module["value"].get<std::string>();

    if (name.empty()) return;

    if (strCaseSensitiveCompare(name, LIGHT_CONTROLLER)) {
        this->a_modules[light_controller]->setValue(value);
    }

    else if (strCaseSensitiveCompare(name, LIGHT_SENSOR)) {
        this->a_modules[light_sensor]->setValue(value);
    }

    else if (strCaseSensitiveCompare(name, LUMINOSITY_SENSOR)) {
        this->a_modules[luminosity_sensor]->setValue(value);
    }

    else if (strCaseSensitiveCompare(name, PRESENCE_DETECTOR)) {
        this->a_modules[presence_detector]->setValue(value);
    }

    else if (strCaseSensitiveCompare(name, TEMPERATURE_SENSOR)) {
        this->a_modules[temperature_sensor]->setValue(value);
    }

    else if (strCaseSensitiveCompare(name, CONSUMPTION_SENSOR)) {
        this->a_modules[consumption_sensor]->setValue(value);
    }
}

void Application::startup() {

    // DEBUG
    DEBUG_MODE_PRINTLN("Starting up...");
    DEBUG_MODE_PRINTLN("Preparing startup message...");

    // Prepare startup message

    const nlohmann::json message = {
        {"id", DEVICE_ID},
        {"type", DEVICE_TYPE},
        {"location_id", this->a_locationID},
        {"location_type", this->a_location},
        {"location_name", this->a_location + " " + toString(this->a_locationID)},
        {
            "modules", {
                {
                    {"name", LIGHT_CONTROLLER},
                    {"value", this->a_modules[light_controller]->getValue()},
                },
                {
                    {"name", LIGHT_SENSOR},
                    {"value", this->a_modules[light_sensor]->getValue()},
                },
                {
                    {"name", LUMINOSITY_SENSOR},
                    {"value", this->a_modules[luminosity_sensor]->getValue()},
                },
                {
                    {"name", PRESENCE_DETECTOR},
                    {"value", this->a_modules[presence_detector]->getValue()},
                },
                {
                    {"name", TEMPERATURE_SENSOR},
                    {"value", this->a_modules[temperature_sensor]->getValue()},
                },
                {
                    {"name", CONSUMPTION_SENSOR},
                    {"value", this->a_modules[consumption_sensor]->getValue()},
                },
            }
        }
    };

    // DEBUG
    DEBUG_MODE_PRINTLN("Serializing startup message...");
    DEBUG_MODE_PRINTF("JSON payload:\n%s\n", message.dump().c_str());
    DEBUG_MODE_PRINTLN("Subscribing to temporary setup channel...");

    this->a_broker->sub(SETUP_TOPIC);
    this->a_wait_for_setup = true;

    bool retain = false;

    // DEBUG
    DEBUG_MODE_PRINTF("Publishing to startup topic with retain to %s...\n", toString(retain).c_str());

    this->a_broker->pub(STARTUP_TOPIC, message.dump(), retain);

    // DEBUG
    DEBUG_MODE_PRINTLN("Waiting for setup message...");

    // Wait for MQTT Broker response
    while (this->isWaitingForSetup()) {
        if (!this->a_broker->loop()) DEBUG_MODE_PRINTLN("Broker disconnected while waiting for setup message...");
        delay(200);
    }

    this->subscribeAllTopics();
}

void Application::subscribeAllTopics() const {

    // DEBUG
    DEBUG_MODE_PRINTLN("Subscribing to topics...");

    // Subscribe to MQTT topics
    this->a_broker->sub(RESET_TOPIC);
    this->a_broker->sub(LIGHT_CONTROLLER);
    this->a_broker->sub(LIGHT_SENSOR);
    this->a_broker->sub(LUMINOSITY_SENSOR);
    // this->a_broker->sub(PRESENCE_DETECTOR);
    // this->a_broker->sub(TEMPERATURE_SENSOR);
    this->a_broker->sub(CONSUMPTION_SENSOR);
}

void Application::unsubscribeAllTopics() const {
    // TODO -> maybe return to setup here? Is it necessary?...
    // if (this->a_broker == nullptr) return;

    // DEBUG
    DEBUG_MODE_PRINTLN("Unsubscribing all topics...");

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
    DEBUG_MODE_PRINTLN("Resetting...");

    // Unsubscribe to MQTT channels
    this->unsubscribeAllTopics();

    // Call the startup() function:
    this->startup();
}

void Application::brokerLoop() const {
    this->a_broker->loop();
}

[[noreturn]] void Application::sensorLoop() {
    while (true) {
        if (!this->isWaitingForSetup() && millis() - this->a_lastPublishTime > this->a_publish_interval) {

            // DEBUG
            DEBUG_MODE_PRINTLN("Running sensor loop...");

            // TODO -> use this kind of condition for sensor check
            this->a_lastPublishTime = millis();
        }

        delay(500);
    }
}

void Application::readInputSensor() {
    for (auto module : this->a_modules) {
        module->readInput();
    }
}


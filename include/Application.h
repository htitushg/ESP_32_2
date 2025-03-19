//
// Created by thorgan on 3/16/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <Arduino.h>
#include <nlohmann/json.hpp>
#include <WiFi.h>
#include <environment.h>
#include <Broker.h>
#include <IModule.h>

class Application {
  protected:
    static std::shared_ptr<Application> app;
    std::string  a_location = DEFAULT_LOCATION;
    unsigned int a_locationID = DEFAULT_LOCATION_ID;
    std::string  a_root_topic;

	// Setup variable (used to synchronize the main loop with the MQTT message handler at setup)
    bool a_wait_for_setup = false;

    unsigned int a_publish_interval = 10000;
    unsigned long a_lastPublishTime = 0;

    WiFiClient a_network;
    std::shared_ptr<Broker> a_broker;
    std::shared_ptr<IModule> a_modules[6];

    enum ModuleTypes: int {
        light_controller = 0,
        light_sensor = 1,
        luminosity_sensor =2,
        presence_detector = 3,
        temperature_sensor = 4,
        consumption_sensor = 5
    };

    bool isWaitingForSetup() const { return this->a_wait_for_setup; }
    void onSetupMessage(const std::string & payload);
    void reset();
    void setupModule(nlohmann::json & module) const;
	static void messageHandler(MQTTClient * client, char topic[], char payload[], int length);
    void unsubscribeAllTopics() const;
    void setRootTopic();
    Application();

  public:
    static std::shared_ptr<Application> getInstance();

    const unsigned long getLastPublishedTime() const { return a_lastPublishTime; }
    const unsigned int getPublishInterval() const { return a_publish_interval; }

    void sensorLoop();
    void brokerLoop() const;
    void startup();
    void initialize(const WiFiClient& wifi);
};

#endif //APPLICATION_H

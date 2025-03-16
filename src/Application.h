//
// Created by thorgan on 3/16/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <Arduino.h>
#include <WiFi.h>
#include <environment.h>
#include <Broker.h>
#include <Module.h>

class Application {
  protected:
    String location = DEFAULT_LOCATION;
    unsigned int locationID = DEFAULT_LOCATION_ID;
    String root_topic;

	// Setup variable (used to synchronize the main loop with the MQTT message handler at setup)
    bool wait_for_setup = false;

    unsigned int publish_interval = 10000;
    unsigned long lastPublishTime = 0;

	// Complete channel names
    String startup_channel;
	String setup_channel;
    String reset_channel;
    String light_controller_channel;
    String light_sensor_channel;
    String luminosity_sensor_channel;
    String presence_detector_channel;
    String temperature_sensor_channel;
    String consumption_sensor_channel;

    WiFiClient network;
    Broker *broker;
    Module *lightController;
    Module *lightSensor;
    Module *luminositySensor;
    Module *presenceDetector;
    Module *temperatureSensor;
    Module *consumptionSensor;

    bool isWaitingForSetup() { return this->wait_for_setup; }
    void onSetupMessage(char payload[]);
    void reset();
    void setupModule(const char* name, const char* value);
	static void messageHandler(MQTTClient *client, char topic[], char payload[], int length);
    void unsubscribeAllTopics();

  public:
    Application();
    void brokerLoop();
    void startup();
    void init(WiFiClient wifi);
    static void sensorLoop();
};

#endif //APPLICATION_H

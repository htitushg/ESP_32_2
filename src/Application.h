//
// Created by thorgan on 3/16/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <environment.h>
#include <Broker.h>
#include <IModule.h>

class Application {
  protected:
    static Application *app;
    String location = DEFAULT_LOCATION;
    unsigned int locationID = DEFAULT_LOCATION_ID;
    String root_topic;

	// Setup variable (used to synchronize the main loop with the MQTT message handler at setup)
    bool wait_for_setup = false;

    unsigned int publish_interval = 10000;
    unsigned long lastPublishTime = 0;

    WiFiClient network;
    Broker *broker = nullptr;
    IModule *lightController = nullptr;
    IModule *lightSensor = nullptr;
    IModule *luminositySensor = nullptr;
    IModule *presenceDetector = nullptr;
    IModule *temperatureSensor = nullptr;
    IModule *consumptionSensor = nullptr;

    bool isWaitingForSetup() const { return this->wait_for_setup; }
    void onSetupMessage(char payload[]);
    void reset();
    void setupModule(const char* name, const char* value) const;
	static void messageHandler(MQTTClient *client, char topic[], char payload[], int length);
    void unsubscribeAllTopics() const;
    void setRootTopic();
    Application();

  public:
    static Application *getInstance();
    void brokerLoop() const;
    void startup();
    void initialize(const WiFiClient& wifi);
    static void sensorLoop();
};

#endif //APPLICATION_H

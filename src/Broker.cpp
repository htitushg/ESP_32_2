//
// Created by thorgan on 3/15/25.
//
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <environment.h>
#include <Broker.h>
#include <utils.h>

Broker::Broker(const WiFiClient& network) {
    this->wifi = network;
    this->mqtt = MQTTClient(512);
}

void Broker::setRootTopic(const String &topic) {
  	this->root_topic = topic;
}

Broker* Broker::newBroker(const WiFiClient& network, void cb(MQTTClient *client, char topic[], char bytes[], int length)) {
    Broker* broker = new Broker(network);

    // DEBUG
    Serial.println("Starting MQTT connection...");

    // Connect to the MQTT broker
    broker->mqtt.begin(MQTT_BROKER_ADDRESS, MQTT_PORT, broker->wifi);

    // Create a handler for incoming messages
    broker->mqtt.onMessageAdvanced(cb);

    // DEBUG
    Serial.printf("%s - Connecting to MQTT broker\n", DEVICE_ID);

    while (!broker->mqtt.connect(DEVICE_ID, MQTT_USERNAME, MQTT_PASSWORD, false)) {

        // DEBUG
        Serial.print(".");
        delay(100);
    }

    // DEBUG
    Serial.println();

    if (!broker->mqtt.connected()) {

        // DEBUG
        Serial.printf("%s - MQTT broker Timeout!\n", DEVICE_ID);
        // TODO -> reset the device here?
    }

    // DEBUG
    Serial.printf("%s  - MQTT broker Connected!\n", DEVICE_ID);

    return broker;
}

void Broker::sub(const String &module_name) {

  	const String topic = this->root_topic + module_name;

    if (this->mqtt.subscribe(topic, QOS)) {

        // DEBUG
        Serial.printf("%s - Subscribed to the topic: ", DEVICE_ID);
    } else {

        // DEBUG
        Serial.printf("%s - Failed to subscribe to the topic: ", DEVICE_ID);
    }

    // DEBUG
    Serial.println(topic);
}

void Broker::pub(const String &module_name, const String &value) {

  	const String topic = this->root_topic + module_name;

    // DEBUG
    Serial.printf("Connection to MQTT: %hhd\n", this->mqtt.connected());
    Serial.println("|=================================->>>");
    Serial.printf("| %s - published to MQTT:\n", DEVICE_ID);
    Serial.printf("|====-> topic: %s\n", topic.c_str());
    Serial.printf("|====-> payload: %s\n\n", value.c_str());

    this->mqtt.publish(topic.c_str(), value.c_str(), RETAIN, QOS);
}

void Broker::unsub(const String &module_name) {

  const String topic = this->root_topic + module_name;

  // DEBUG
  Serial.printf("Unsubscribing from topic: %s\n", topic.c_str());

  this->mqtt.unsubscribe(topic);
}

void Broker::loop() {
  this->mqtt.loop();
}
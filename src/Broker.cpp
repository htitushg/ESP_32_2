//
// Created by thorgan on 3/15/25.
//

#include <Arduino.h>
#include <ArduinoJson.h>
#include <environment.h>
#include <Broker.h>
#include <utils.h>

#include <memory>

Broker::Broker(WiFiClient * network) {
    this->a_wifi.reset(network);
    this->a_mqtt = std::make_shared<MQTTClient>(512);
}

void Broker::setRootTopic(const String & topic) {
  	this->a_root_topic = topic;
}

std::shared_ptr<Broker> Broker::newBroker(WiFiClient * network, void cb(MQTTClient *client, char topic[], char bytes[], int length)) {
    std::shared_ptr<Broker> broker;
    broker.reset(new Broker(network));

    // DEBUG
    Serial.println("Starting MQTT connection...");

    // Connect to the MQTT broker
    broker->a_mqtt->begin(MQTT_BROKER_ADDRESS, MQTT_PORT, * broker->a_wifi);

    // Create a handler for incoming messages
    broker->a_mqtt->onMessageAdvanced(cb);

    // DEBUG
    Serial.printf("%s - Connecting to MQTT broker\n", DEVICE_ID);

    while (!broker->a_mqtt->connect(DEVICE_ID, MQTT_USERNAME, MQTT_PASSWORD, false)) {

        // DEBUG
        Serial.print(".");
        delay(100);
    }

    // DEBUG
    Serial.println();

    if (!broker->a_mqtt->connected()) {

        // DEBUG
        Serial.printf("%s - MQTT broker Timeout!\n", DEVICE_ID);
        // TODO -> reset the device here?
    }

    // DEBUG
    Serial.printf("%s  - MQTT broker Connected!\n", DEVICE_ID);

    return broker;
}

void Broker::sub(const String & module_name) {

  	const String topic = this->a_root_topic + module_name;

    // DEBUG
    Serial.printf("Subscribing to topic %s with QoS %d\n", topic.c_str(), QOS);

    if (this->a_mqtt->subscribe(topic, QOS)) {

        // DEBUG
        Serial.printf("%s - Subscribed to the topic: ", DEVICE_ID);
    } else {

        // DEBUG
        Serial.printf("%s - Failed to subscribe to the topic: ", DEVICE_ID);
    }

    // DEBUG
    Serial.println(topic);
}

void Broker::pub(const String & module_name, const String & value) {

  	const String topic = this->a_root_topic + module_name;

    // DEBUG
    Serial.printf("Connection to MQTT: %hhd\n", this->a_mqtt->connected());
    Serial.println("|=================================->>>");
    Serial.printf("| %s - published to MQTT:\n", DEVICE_ID);
    Serial.printf("|====-> topic: %s\n", topic.c_str());
    Serial.printf("|====-> payload: %s\n\n", value.c_str());
    Serial.printf("|====-> retain: %hhd\n", RETAIN);
    Serial.printf("|====-> QoS: %d\n\n", QOS);

    this->a_mqtt->publish(topic.c_str(), value.c_str(), RETAIN, QOS);
}

void Broker::unsub(const String & module_name) {

  const String topic = this->a_root_topic + module_name;

  // DEBUG
  Serial.printf("Unsubscribing from topic: %s\n", topic.c_str());

  this->a_mqtt->unsubscribe(topic);
}

void Broker::loop() {
  this->a_mqtt->loop();
}
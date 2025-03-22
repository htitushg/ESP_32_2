//
// Created by thorgan on 3/15/25.
//

#include <Arduino.h>
#include <environment.h>
#include <Broker.h>
#include <utils.h>

#include <memory>

Broker::Broker(WiFiClient * network) {
    this->a_wifi.reset(network);
    this->a_mqtt = std::make_shared<MQTTClient>(512);
}

void Broker::setRootTopic(const std::string & topic) {
  	this->a_root_topic = topic;
}

void Broker::connect(void callback(MQTTClient *client, char topic[], char bytes[], int length)) {
	// Connect to the MQTT broker
	this->a_mqtt->begin(MQTT_BROKER_ADDRESS, MQTT_PORT, * this->a_wifi);

	// Create a handler for incoming messages
	this->a_mqtt->onMessageAdvanced(callback);

    // DEBUG
    DEBUG_MODE_PRINTF("%s - Connecting to MQTT broker\n", DEVICE_ID);

	while (!this->a_mqtt->connect(DEVICE_ID, MQTT_USERNAME, MQTT_PASSWORD, false)) {

        // DEBUG
        DEBUG_MODE_PRINT(".");
        delay(100);
	}

    // DEBUG
    DEBUG_MODE_PRINTLN();

	if (!this->a_mqtt->connected()) {

        // DEBUG
        DEBUG_MODE_PRINTF("%s - MQTT broker Timeout!\n", DEVICE_ID);

		// TODO -> reset the device here?

		return;
	}

    // DEBUG
    DEBUG_MODE_PRINTF("%s  - MQTT broker Connected!\n", DEVICE_ID);
}

std::shared_ptr<Broker> Broker::newBroker(WiFiClient * network, void callback(MQTTClient *client, char topic[], char bytes[], int length)) {
    std::shared_ptr<Broker> broker;
    broker.reset(new Broker(network));

    // DEBUG
    DEBUG_MODE_PRINTLN("Starting MQTT connection...");

	broker->connect(callback);

    return broker;
}

void Broker::sub(const std::string & module_name) {

  	const std::string topic = this->a_root_topic + module_name;

    // DEBUG
    DEBUG_MODE_PRINTF("Subscribing to topic %s with QoS %d\n", topic.c_str(), QOS);

    if (this->a_mqtt->subscribe(String(topic.c_str()), QOS)) {

        // DEBUG
        DEBUG_MODE_PRINTF("%s - Subscribed to the topic: %s\n", DEVICE_ID, topic.c_str());
    } else {

        // DEBUG
        DEBUG_MODE_PRINTF("%s - Failed to subscribe to the topic: %s\n", DEVICE_ID, topic.c_str());
    }
}

void Broker::pub(const std::string & module_name, const std::string & value) {
    this->pub(module_name, value, RETAIN);
}

void Broker::pub(const std::string &module_name, const std::string &value, const bool retain) {

    std::string topic = this->a_root_topic;
    topic.append(module_name);

    // DEBUG
    DEBUG_MODE_PRINTF("Connection to MQTT: %s\n", this->a_mqtt->connected() ? "true": "false");
    DEBUG_MODE_PRINTLN("|=================================->>>");
    DEBUG_MODE_PRINTF("| %s - published to MQTT:\n", DEVICE_ID);
    DEBUG_MODE_PRINTF("|====-> topic: %s\n", topic.c_str());
    DEBUG_MODE_PRINTF("|====-> payload: %s\n", value.c_str());
    DEBUG_MODE_PRINTF("|====-> retain: %s\n", toString(retain).c_str());
    DEBUG_MODE_PRINTF("|====-> QoS: %d\n\n", QOS);

    this->a_mqtt->publish(topic.c_str(), value.c_str(), retain, QOS);
}

void Broker::unsub(const std::string & module_name) {

  const std::string topic = this->a_root_topic + module_name;

  // DEBUG
  DEBUG_MODE_PRINTF("Unsubscribing from topic: %s\n", topic.c_str());

  this->a_mqtt->unsubscribe(String(topic.c_str()));
}

bool Broker::loop() {
  return this->a_mqtt->loop();
}
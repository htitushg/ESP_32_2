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

std::shared_ptr<Broker> Broker::newBroker(WiFiClient * network, void cb(MQTTClient *client, char topic[], char bytes[], int length)) {
    std::shared_ptr<Broker> broker;
    broker.reset(new Broker(network));

	if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.println("Starting MQTT connection...");
	}

    // Connect to the MQTT broker
    broker->a_mqtt->begin(MQTT_BROKER_ADDRESS, MQTT_PORT, * broker->a_wifi);

    // Create a handler for incoming messages
    broker->a_mqtt->onMessageAdvanced(cb);

	if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("%s - Connecting to MQTT broker\n", DEVICE_ID);
	}

    while (!broker->a_mqtt->connect(DEVICE_ID, MQTT_USERNAME, MQTT_PASSWORD, false)) {

	    if (IS_DEBUG_MODE) {
            // DEBUG
            Serial.print(".");
            delay(100);
	    }
    }

	if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.println();
	}

    if (!broker->a_mqtt->connected()) {

	    if (IS_DEBUG_MODE) {
            // DEBUG
            Serial.printf("%s - MQTT broker Timeout!\n", DEVICE_ID);
	    }
        // TODO -> reset the device here?
    }

	if (IS_DEBUG_MODE) {
		// DEBUG
		Serial.printf("%s  - MQTT broker Connected!\n", DEVICE_ID);
	}

    return broker;
}

void Broker::sub(const std::string & module_name) {

  	const std::string topic = this->a_root_topic + module_name;

	if (IS_DEBUG_MODE) {
	    // DEBUG
	    Serial.printf("Subscribing to topic %s with QoS %d\n", topic.c_str(), QOS);
	}

    if (this->a_mqtt->subscribe(String(topic.c_str()), QOS)) {

		if (IS_DEBUG_MODE) {
	        // DEBUG
	        Serial.printf("%s - Subscribed to the topic: %s\n", DEVICE_ID, topic.c_str());
		}
    } else {

		if (IS_DEBUG_MODE) {
	        // DEBUG
	        Serial.printf("%s - Failed to subscribe to the topic: %s\n", DEVICE_ID, topic.c_str());
		}
    }
}

void Broker::pub(const std::string & module_name, const std::string & value) {
    this->pub(module_name, value, RETAIN);
}

void Broker::pub(const std::string &module_name, const std::string &value, const bool retain) {

    std::string topic = this->a_root_topic;
    topic.append(module_name);

	if (IS_DEBUG_MODE) {
	    // DEBUG
	    Serial.printf("Connection to MQTT: %s\n", this->a_mqtt->connected() ? "true": "false");
	    Serial.println("|=================================->>>");
	    Serial.printf("| %s - published to MQTT:\n", DEVICE_ID);
	    Serial.printf("|====-> topic: %s\n", topic.c_str());
	    Serial.printf("|====-> payload: %s\n", value.c_str());
	    Serial.printf("|====-> retain: %s\n", toString(retain).c_str());
	    Serial.printf("|====-> QoS: %d\n\n", QOS);
	}

    this->a_mqtt->publish(topic.c_str(), value.c_str(), retain, QOS);
}

void Broker::unsub(const std::string & module_name) {

  const std::string topic = this->a_root_topic + module_name;

	if (IS_DEBUG_MODE) {
	  // DEBUG
	  Serial.printf("Unsubscribing from topic: %s\n", topic.c_str());
	}

  this->a_mqtt->unsubscribe(String(topic.c_str()));
}

void Broker::loop() {
  this->a_mqtt->loop();
}
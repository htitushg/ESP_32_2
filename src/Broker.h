//
// Created by thorgan on 3/15/25.
//

#ifndef BROKER_H
#define BROKER_H

#pragma once

#include <MQTTClient.h>
#include <WiFi.h>

class Broker {
  protected:
    MQTTClient mqtt;
    WiFiClient wifi;
    String root_topic;

    explicit Broker(const WiFiClient& network);

  public:
    static Broker* newBroker(const WiFiClient &network, void cb(MQTTClient *client, char topic[], char bytes[], int length));
    void sub(const String &module_name);
    void pub(const String &module_name, const String &value);
    void unsub(const String &module_name);
    void setRootTopic(const String &topic);
    void loop();
};

#endif //BROKER_H

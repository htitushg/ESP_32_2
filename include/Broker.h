//
// Created by thorgan on 3/15/25.
//

#ifndef BROKER_H
#define BROKER_H

#include <MQTTClient.h>
#include <WiFi.h>

class Broker {
  protected:
    std::shared_ptr<MQTTClient> a_mqtt;
    std::shared_ptr<WiFiClient> a_wifi;
    String a_root_topic;

    explicit Broker(WiFiClient * network);

  public:
    static std::shared_ptr<Broker> newBroker(WiFiClient * network, void cb(MQTTClient *client, char topic[], char bytes[], int length));
    void sub(const String &module_name);
    void pub(const String &module_name, const String &value);
    void unsub(const String &module_name);
    void setRootTopic(const String &topic);
    void loop();
};

#endif //BROKER_H

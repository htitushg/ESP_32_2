//
// Created by thorgan on 3/15/25.
//

#ifndef BROKER_H
#define BROKER_H

#include <MQTTClient.h>
#include <WiFi.h>

class Broker {
  protected:
    MQTTClient mqtt;
    WiFiClient wifi;

    Broker(WiFiClient network);

  public:
    static Broker* newBroker(WiFiClient network, std::function<void(MQTTClient *client, char topic[], char bytes[], int length)> cb);
    void sub(const String &topic);
    void pub(const String &topic, const String &value);
    void unsub(const String &topic);
    void loop();
};
//void setupModule(const char* name, const char* value);
//void messageHandler(MQTTClient *client, char topic[], char payload[], int length);

#endif //BROKER_H

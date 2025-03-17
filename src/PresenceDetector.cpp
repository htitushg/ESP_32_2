//
// Created by thorgan on 3/17/25.
//

#pragma once

#include "PresenceDetector.h"
#include <environment.h>
#include <utils.h>

PresenceDetector::PresenceDetector(Broker *broker, const bool value){
    this->broker = broker;
    this->value = value;
}

void PresenceDetector::setValue(const char * value) {
    if (isBool(value)) {
        const bool position = parseBool(value);

        // Ignore repeated values
        if (this->value == position) return;

        this->Notify();

        // TODO -> implement lightSensor logic when changing position

        this->broker->pub(this->name, String(position ? "True" : "False"));
        return;
    }

    // DEBUG
    Serial.printf("invalid value: %s\n", value);
}

const String PresenceDetector::getValue(){
    return toString(this->value);
}

const void *PresenceDetector::getValueReference() {
  return &this->value;
}

void PresenceDetector::Update(const String &module_name, const String &value) {
}

void PresenceDetector::Attach(IObserver *observer) {
  this->i_observers.push_back(observer);
}

void PresenceDetector::Detach(IObserver *observer) {
    for (auto itr = this->i_observers.begin(); itr != this->i_observers.end();)
    {
        if (*itr == observer)
            itr = this->i_observers.erase(itr);
        else
            ++itr;
    }
}

void PresenceDetector::Notify() {
    for (IObserver* observer : this->i_observers) {
        observer->Update(PRESENCE_DETECTOR, this->getValue());
    }
}

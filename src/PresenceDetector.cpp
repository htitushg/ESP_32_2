//
// Created by thorgan on 3/17/25.
//

#include "PresenceDetector.h"
#include <environment.h>
#include <utils.h>

PresenceDetector::PresenceDetector(Broker *broker, const bool value) {

    // DEBUG
    Serial.printf("Creating PresenceDetector with value: %hhd\n", value);

    this->a_broker = broker;
    this->a_value = value;
}

void PresenceDetector::setValue(const char * value) {
    if (isBool(value)) {
        const bool position = parseBool(value);

        // Ignore repeated values
        if (this->a_value == position) return;

        this->Notify();

        // TODO -> implement lightSensor logic when changing position

        this->a_broker->pub(this->a_name, String(position ? "True" : "False"));
        return;
    }

    // DEBUG
    Serial.printf("invalid value: %s\n", value);
}

const String PresenceDetector::getValue() const {
    return toString(this->a_value);
}

const MyAny PresenceDetector::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "bool");
}

void PresenceDetector::Update(const String &module_name, const String &value) {
}

void PresenceDetector::Attach(IObserver *observer) {
  this->a_observers.push_back(observer);
}

void PresenceDetector::Detach(IObserver *observer) {
    for (auto itr = this->a_observers.begin(); itr != this->a_observers.end();)
    {
        if (*itr == observer)
            itr = this->a_observers.erase(itr);
        else
            ++itr;
    }
}

void PresenceDetector::Notify() {
    for (IObserver* observer : this->a_observers) {
        observer->Update(PRESENCE_DETECTOR, this->getValue());
    }
}

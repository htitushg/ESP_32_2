//
// Created by thorgan on 3/17/25.
//

#include "PresenceDetector.h"
#include <MyAny.h>
#include <environment.h>
#include <utils.h>

PresenceDetector::PresenceDetector(Broker *broker, const bool value) {

    // DEBUG
    DEBUG_MODE_PRINTF("Creating PresenceDetector with value: %s\n", toString(value).c_str());

    this->a_name = PRESENCE_DETECTOR;
    this->a_broker = broker;
    this->a_value = value;
}

void PresenceDetector::setValue(const std::string & value) {

    // DEBUG
    DEBUG_MODE_PRINTF("Setting %s value %s...\n", this->getName().c_str(), value.c_str());

    if (isBool(value)) {
        const bool position = parseBool(value);

        // DEBUG
        DEBUG_MODE_PRINTF("current value: %s/ new value: %s\n", toString(this->a_value).c_str(), toString(position).c_str());

        // Ignore repeated values
        if (this->a_value == position) return;

        this->a_value = position;

        this->Notify();

        // TODO -> implement lightSensor logic when changing position

        this->a_broker->pub(this->getName(), toString(position));
        return;
    }

    // DEBUG
    DEBUG_MODE_PRINTF("invalid value: %s\n", value.c_str());
}

const std::string PresenceDetector::getValue() const {
    return toString(this->a_value);
}

const MyAny PresenceDetector::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "bool");
}

void PresenceDetector::Update(const std::string & module_name, const std::string & value) {
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

    // DEBUG
    DEBUG_MODE_PRINTLN("PresenceDetector notifying observers of an update...");

    int i = 0;
    for (IObserver* observer : this->a_observers) {

        // DEBUG
        DEBUG_MODE_PRINTF("PresenceDetector notifying observer #%d of an update...\n", i);

        ++i;

        observer->Update(PRESENCE_DETECTOR, this->getValue());
    }
}

void PresenceDetector::readInput() {
    const bool is_presence = digitalRead(PRESENCE_DETECTOR_PIN);
    this->setValue(toString(is_presence));
}


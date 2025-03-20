//
// Created by thorgan on 3/17/25.
//

#include "LightSensor.h"
#include <MyAny.h>
#include <environment.h>
#include <utils.h>

LightSensor::LightSensor(Broker *broker, const bool value) {

	if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("Creating LightSensor with value: %s\n", toString(value).c_str());
	}


    this->a_name = LIGHT_SENSOR;
    this->a_broker = broker;
    this->a_value = value;
}

void LightSensor::setValue(const std::string &value) {

	if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("Setting %s value %s...\n", this->getName().c_str(), value.c_str());
	}

    if (isBool(value)) {
        const bool position = parseBool(value);

        // Ignore repeated values
        if (this->a_value == position) return;

        this->a_value = position;

        this->Notify();

        // TODO -> implement lightSensor logic when changing position

        this->a_broker->pub(this->getName(), toString(position));
        return;
    }

	if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("invalid value: %s\n", value.c_str());
	}
}

const std::string LightSensor::getValue() const {
    return toString(this->a_value);
}

const MyAny LightSensor::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "bool");
}

void LightSensor::Update(const std::string &module_name, const std::string &value) {

}

void LightSensor::Attach(IObserver *observer) {
    this->a_observers.push_back(observer);
}

void LightSensor::Detach(IObserver *observer) {
    for (auto itr = this->a_observers.begin(); itr != this->a_observers.end();)
    {
        if (*itr == observer)
            itr = this->a_observers.erase(itr);
        else
            ++itr;
    }
}

void LightSensor::Notify() {
    for (IObserver* observer : this->a_observers) {
        observer->Update(LIGHT_SENSOR, this->getValue());
    }
}

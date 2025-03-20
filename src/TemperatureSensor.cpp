//
// Created by thorgan on 3/17/25.
//

#include "TemperatureSensor.h"
#include <MyAny.h>
#include <environment.h>
#include <utils.h>

TemperatureSensor::TemperatureSensor(Broker *broker, const float value) {

	if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("Creating TemperatureSensor with value: %f\n", value);
	}


    this->a_name = TEMPERATURE_SENSOR;
    this->a_broker = broker;
    this->a_value = value;
}

void TemperatureSensor::setValue(const std::string &value) {

	if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("Setting %s value %s...\n", this->getName().c_str(), value.c_str());
	}

    const float position = parseFloat(value);

    // Ignore repeated values
    if (this->a_value == position) return;

    this->a_value = position;

    this->Notify();

    // TODO -> implement lightSensor logic when changing position

    this->a_broker->pub(this->getName(), toString(position));
}

const std::string TemperatureSensor::getValue() const {
    return toString(this->a_value, 2);
}

const MyAny TemperatureSensor::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "float");
}

void TemperatureSensor::Update(const std::string & module_name, const std::string & value) {
}

void TemperatureSensor::Attach(IObserver *observer) {
    this->a_observers.push_back(observer);
}

void TemperatureSensor::Detach(IObserver *observer) {
    for (auto itr = this->a_observers.begin(); itr != this->a_observers.end();)
    {
        if (*itr == observer)
            itr = this->a_observers.erase(itr);
        else
            ++itr;
    }
}

void TemperatureSensor::Notify() {
    for (IObserver* observer : this->a_observers) {
        observer->Update(TEMPERATURE_SENSOR, this->getValue());
    }
}

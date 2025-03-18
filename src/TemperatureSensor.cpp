//
// Created by thorgan on 3/17/25.
//

#include "TemperatureSensor.h"
#include <environment.h>
#include <utils.h>

TemperatureSensor::TemperatureSensor(Broker *broker, const float value) {

    // DEBUG
    Serial.printf("Creating TemperatureSensor with value: %f\n", value);

    this->a_broker = broker;
    this->a_value = value;
}

void TemperatureSensor::setValue(const char * value) {
    const float position = parseFloat(value);

    // Ignore repeated values
    if (this->a_value == position) return;

    this->Notify();

    // TODO -> implement lightSensor logic when changing position

    this->a_broker->pub(this->a_name, toString(position));
    return;

    // DEBUG
    Serial.printf("invalid value: %s\n", value);
}

const String TemperatureSensor::getValue() const {
    return toString(this->a_value);
}

const MyAny TemperatureSensor::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "float");
}

void TemperatureSensor::Update(const String &module_name, const String &value) {
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

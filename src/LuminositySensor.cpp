//
// Created by thorgan on 3/17/25.
//

#include "LuminositySensor.h"
#include <MyAny.h>
#include <environment.h>
#include <utils.h>

LuminositySensor::LuminositySensor(Broker *broker, const float value) {

    // DEBUG
    DEBUG_MODE_PRINTF("Creating LuminositySensor with value: %f\n", value);

    this->a_name = LUMINOSITY_SENSOR;
    this->a_broker = broker;
    this->a_value = value;
}

void LuminositySensor::setValue(const std::string &value) {

    // DEBUG
    DEBUG_MODE_PRINTF("Setting %s value %s...\n", this->getName().c_str(), value.c_str());

    const float position = parseFloat(value);

    // Ignore repeated values
    if (this->a_value == position) return;

    this->a_value = position;

    this->Notify();

    // TODO -> implement lightSensor logic when changing position

    this->a_broker->pub(this->getName(), toString(position));
}

const std::string LuminositySensor::getValue() const {
    return toString(this->a_value, 2);
}

const MyAny LuminositySensor::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "float");
}

void LuminositySensor::Update(const std::string &module_name, const std::string &value) {
}

void LuminositySensor::Attach(IObserver *observer) {
    this->a_observers.push_back(observer);
}

void LuminositySensor::Detach(IObserver *observer) {
    for (auto itr = this->a_observers.begin(); itr != this->a_observers.end();)
    {
        if (*itr == observer)
            itr = this->a_observers.erase(itr);
        else
            ++itr;
    }
}

void LuminositySensor::Notify() {
    for (IObserver* observer : this->a_observers) {
        observer->Update(LUMINOSITY_SENSOR, this->getValue());
    }
}

//
// Created by thorgan on 3/17/25.
//

#include "LuminositySensor.h"
#include <environment.h>
#include <utils.h>

LuminositySensor::LuminositySensor(Broker *broker, const float value) {

    // DEBUG
    Serial.printf("Creating LuminositySensor with value: %f\n", value);

    this->a_broker = broker;
    this->a_value = value;
}

void LuminositySensor::setValue(const char * value) {
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

const String LuminositySensor::getValue() const {
    return toString(this->a_value);
}

const MyAny LuminositySensor::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "float");
}

void LuminositySensor::Update(const String &module_name, const String &value) {
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

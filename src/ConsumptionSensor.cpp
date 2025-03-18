//
// Created by thorgan on 3/17/25.
//

#include "ConsumptionSensor.h"
#include <environment.h>
#include <utils.h>

ConsumptionSensor::~ConsumptionSensor() {
    Serial.println("ConsumptionSensor::~ConsumptionSensor()");
}

ConsumptionSensor::ConsumptionSensor(Broker *broker, const float value){

    // DEBUG
    Serial.printf("Creating ConsumptionSensor with value: %f\n", value);

    this->a_broker = broker;
    this->a_value = value;
}

void ConsumptionSensor::setValue(const char * value) {
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

const String ConsumptionSensor::getValue() const {
    return toString(this->a_value);
}

const MyAny ConsumptionSensor::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "float");
}

void ConsumptionSensor::Update(const String &module_name, const String &value) {
}

void ConsumptionSensor::Attach(IObserver *observer) {
    this->a_observers.push_back(observer);
}

void ConsumptionSensor::Detach(IObserver *observer) {
    for (auto itr = this->a_observers.begin(); itr != this->a_observers.end();)
    {
        if (*itr == observer)
            itr = this->a_observers.erase(itr);
        else
            ++itr;
    }
}

void ConsumptionSensor::Notify() {
    for (IObserver* observer : this->a_observers) {
        observer->Update(CONSUMPTION_SENSOR, this->getValue());
    }
}

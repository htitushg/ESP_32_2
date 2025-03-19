//
// Created by thorgan on 3/17/25.
//

#include "ConsumptionSensor.h"
#include <MyAny.h>
#include <environment.h>
#include <utils.h>

ConsumptionSensor::~ConsumptionSensor() {
    Serial.println("ConsumptionSensor::~ConsumptionSensor()");
}

ConsumptionSensor::ConsumptionSensor(Broker *broker, const float value){

	if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("Creating ConsumptionSensor with value: %f\n", value);
	}


    this->a_name = CONSUMPTION_SENSOR;
    this->a_broker = broker;
    this->a_value = value;
}

void ConsumptionSensor::setValue(const std::string &value) {

	if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("Setting %s value %s...", this->getName().c_str(), value.c_str());
	}

    const float position = parseFloat(value);

    // Ignore repeated values
    if (this->a_value == position) return;

    this->Notify();

    // TODO -> implement lightSensor logic when changing position

    this->a_broker->pub(this->getName(), toString(position));
}

const std::string ConsumptionSensor::getValue() const {
    return toString(this->a_value);
}

const MyAny ConsumptionSensor::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "float");
}

void ConsumptionSensor::Update(const std::string &module_name, const std::string &value) {
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

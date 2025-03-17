//
// Created by thorgan on 3/17/25.
//

#pragma once

#include "ConsumptionSensor.h"
#include <environment.h>
#include <utils.h>

ConsumptionSensor::ConsumptionSensor(Broker *broker, const float value){
    this->broker = broker;
    this->value = value;
}

void ConsumptionSensor::setValue(const char * value) {
    const float position = parseFloat(value);

    // Ignore repeated values
    if (this->value == position) return;

    this->Notify();

    // TODO -> implement lightSensor logic when changing position

    this->broker->pub(this->name, toString(position));
    return;

    // DEBUG
    Serial.printf("invalid value: %s\n", value);
}

const String ConsumptionSensor::getValue(){
    return toString(this->value);
}

const void *ConsumptionSensor::getValueReference() {
  return &this->value;
}

void ConsumptionSensor::Update(const String &module_name, const String &value) {
}

void ConsumptionSensor::Attach(IObserver *observer) {
    this->i_observers.push_back(observer);
}

void ConsumptionSensor::Detach(IObserver *observer) {
    for (auto itr = this->i_observers.begin(); itr != this->i_observers.end();)
    {
        if (*itr == observer)
            itr = this->i_observers.erase(itr);
        else
            ++itr;
    }
}

void ConsumptionSensor::Notify() {
    for (IObserver* observer : this->i_observers) {
        observer->Update(CONSUMPTION_SENSOR, this->getValue());
    }
}

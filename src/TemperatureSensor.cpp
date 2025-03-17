//
// Created by thorgan on 3/17/25.
//

#pragma once

#include "TemperatureSensor.h"
#include <environment.h>
#include <utils.h>

TemperatureSensor::TemperatureSensor(Broker *broker, const float value){
    this->broker = broker;
    this->value = value;
}

void TemperatureSensor::setValue(const char * value) {
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

const String TemperatureSensor::getValue(){
    return toString(this->value);
}

const void *TemperatureSensor::getValueReference() {
  return &this->value;
}

void TemperatureSensor::Update(const String &module_name, const String &value) {
}

void TemperatureSensor::Attach(IObserver *observer) {
    this->i_observers.push_back(observer);
}

void TemperatureSensor::Detach(IObserver *observer) {
    for (auto itr = this->i_observers.begin(); itr != this->i_observers.end();)
    {
        if (*itr == observer)
            itr = this->i_observers.erase(itr);
        else
            ++itr;
    }
}

void TemperatureSensor::Notify() {
    for (IObserver* observer : this->i_observers) {
        observer->Update(TEMPERATURE_SENSOR, this->getValue());
    }
}

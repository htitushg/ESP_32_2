//
// Created by thorgan on 3/17/25.
//

#pragma once

#include "LuminositySensor.h"
#include <environment.h>
#include <utils.h>

LuminositySensor::LuminositySensor(Broker *broker, const float value){
    this->broker = broker;
    this->value = value;
}

void LuminositySensor::setValue(const char * value) {
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

const String LuminositySensor::getValue(){
    return toString(this->value);
}

const void *LuminositySensor::getValueReference() {
  return &this->value;
}

void LuminositySensor::Update(const String &module_name, const String &value) {
}

void LuminositySensor::Attach(IObserver *observer) {
    this->i_observers.push_back(observer);
}

void LuminositySensor::Detach(IObserver *observer) {
    for (auto itr = this->i_observers.begin(); itr != this->i_observers.end();)
    {
        if (*itr == observer)
            itr = this->i_observers.erase(itr);
        else
            ++itr;
    }
}

void LuminositySensor::Notify() {
    for (IObserver* observer : this->i_observers) {
        observer->Update(LUMINOSITY_SENSOR, this->getValue());
    }
}

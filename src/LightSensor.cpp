//
// Created by thorgan on 3/17/25.
//

#include "LightSensor.h"

#include <environment.h>
#include <utils.h>

#pragma once

LightSensor::LightSensor(Broker *broker, const bool value){
    this->broker = broker;
    this->value = value;
}

void LightSensor::setValue(const char * value) {
    if (isBool(value)) {
        const bool position = parseBool(value);

        // Ignore repeated values
        if (this->value == position) return;

        this->Notify();

        // TODO -> implement lightSensor logic when changing position

        this->broker->pub(this->name, String(position ? "True" : "False"));
        return;
    }

    // DEBUG
    Serial.printf("invalid value: %s\n", value);
}

const String LightSensor::getValue(){
    return toString(this->value);
}

const void *LightSensor::getValueReference() {
    return &this->value;
}

void LightSensor::Update(const String &module_name, const String &value) {

}

void LightSensor::Attach(IObserver *observer) {
    this->i_observers.push_back(observer);
}

void LightSensor::Detach(IObserver *observer) {
    for (auto itr = this->i_observers.begin(); itr != this->i_observers.end();)
    {
        if (*itr == observer)
            itr = this->i_observers.erase(itr);
        else
            ++itr;
    }
}

void LightSensor::Notify() {
    for (IObserver* observer : this->i_observers) {
        observer->Update(LIGHT_SENSOR, this->getValue());
    }
}

//
// Created by thorgan on 3/17/25.
//

#include "LightSensor.h"
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

void LightSensor::Update(const String &value) {
}

void LightSensor::Attach(IObserver *observer) {
}

void LightSensor::Detach(IObserver *observer) {
}

void LightSensor::Notify() {
}

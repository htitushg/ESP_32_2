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

void LuminositySensor::Update(const String &value) {
}

void LuminositySensor::Attach(IObserver *observer) {
}

void LuminositySensor::Detach(IObserver *observer) {
}

void LuminositySensor::Notify() {
}

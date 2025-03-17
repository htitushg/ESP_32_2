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

void TemperatureSensor::Update(const String &value) {
}

void TemperatureSensor::Attach(IObserver *observer) {
}

void TemperatureSensor::Detach(IObserver *observer) {
}

void TemperatureSensor::Notify() {
}

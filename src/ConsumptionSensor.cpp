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

void ConsumptionSensor::Update(const String &value) {
}

void ConsumptionSensor::Attach(IObserver *observer) {
}

void ConsumptionSensor::Detach(IObserver *observer) {
}

void ConsumptionSensor::Notify() {
}

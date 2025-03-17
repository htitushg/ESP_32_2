//
// Created by thorgan on 3/17/25.
//

#pragma once

#include "PresenceDetector.h"
#include <environment.h>
#include <utils.h>

PresenceDetector::PresenceDetector(Broker *broker, const bool value){
    this->broker = broker;
    this->value = value;
}

void PresenceDetector::setValue(const char * value) {
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

const String PresenceDetector::getValue(){
    return toString(this->value);
}

const void *PresenceDetector::getValueReference() {
  return &this->value;
}

void PresenceDetector::Update(const String &value) {
}

void PresenceDetector::Attach(IObserver *observer) {
}

void PresenceDetector::Detach(IObserver *observer) {
}

void PresenceDetector::Notify() {
}

//
// Created by thorgan on 3/16/25.
//

#pragma once

#include "LightController.h"
#include <environment.h>
#include <utils.h>

LightController::LightController(Broker *broker, const void *isPresence, bool value){
	this->is_presence = (bool *)isPresence;
  	this->broker = broker;
	this->value = value;
}

void LightController::setValue(const char * value) {
	if (isBool(value)) {
      	const bool position = parseBool(value);

    	// Ignore repeated values
  		if (this->value == position) return;

  		this->value = position;
  		if (this->value) {
    		digitalWrite(LIGHT, HIGH);
  		} else {
    		digitalWrite(LIGHT, LOW);
  		}

  		this->broker->pub(this->name, String(position ? "True" : "False"));
      	return;
	}

    // DEBUG
    Serial.printf("invalid value: %s\n", value);
}

const String LightController::getValue() {
  return toString(this->value);
}

const void *LightController::getValueReference() {
	return &this->value;
}

void LightController::Update(const String &value) {
}

void LightController::Attach(IObserver *observer) {
}

void LightController::Detach(IObserver *observer) {
}

void LightController::Notify() {
}

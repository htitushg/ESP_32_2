//
// Created by thorgan on 3/16/25.
//

#pragma once

#include "LightController.h"
#include <environment.h>
#include <utils.h>

LightController::LightController(Broker *broker, const bool value) {
	this->ref_values = new ValueReferenceMap();
  	this->broker = broker;
	this->value = value;
}

void LightController::addReference(ValueReference *reference) const {
	this->ref_values->add(reference->getKey(), reference->getValue());
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

		this->Notify();

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

float LightController::getLuminosity() const {

	// Return nullptr if not found
	const auto val = this->ref_values->getValue(LUMINOSITY_SENSOR);

	// Return false if val == nullptr
	return fromFloatPtr(val);
}

bool LightController::getLightState() const {

	// Return nullptr if not found
	const auto val = this->ref_values->getValue(LIGHT_SENSOR);

	// Return 0.0f if val == nullptr
	return fromBoolPtr(val);
}

void LightController::Update(const String &module_name, const String &value) {
	if (strCaseSensitiveCompare(module_name, PRESENCE_DETECTOR)) {
		if (!isBool(value)) return;
		const bool is_presence = parseBool(value);

		// Turn light on when detecting presence, the light is off, and the luminosity is lower than 100
		if (is_presence && !this->getLightState() && this->getLuminosity() < 100.0f) this->setValue(toString(true).c_str());
	}
}

void LightController::Attach(IObserver *observer) {
	this->i_observers.push_back(observer);
}

void LightController::Detach(IObserver *observer) {
	for (auto itr = this->i_observers.begin(); itr != this->i_observers.end();)
	{
		if (*itr == observer)
			itr = this->i_observers.erase(itr);
		else
			++itr;
	}
}

void LightController::Notify() {
	for (IObserver* observer : this->i_observers) {
		observer->Update(LIGHT_CONTROLLER, this->getValue());
	}
}

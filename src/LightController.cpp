//
// Created by thorgan on 3/16/25.
//

#include "LightController.h"
#include <environment.h>
#include <utils.h>

LightController::LightController(Broker *broker, const bool value) {

	// DEBUG
	Serial.printf("Creating LightController with value: %hhd\n", value);

	this->a_ref_values = std::map<std::string, MyAny>();
  	this->a_broker = broker;
	this->a_value = value;
}

void LightController::addReference(MyAny value, String module_name) {
	this->a_ref_values[std::string(module_name.c_str())] = value;
}

void LightController::setValue(const char * value) {
	if (isBool(value)) {
      	const bool position = parseBool(value);

    	// Ignore repeated values
  		if (this->a_value == position) return;

  		this->a_value = position;
  		if (this->a_value) {
    		digitalWrite(LIGHT, HIGH);
  		} else {
    		digitalWrite(LIGHT, LOW);
  		}

		this->Notify();

  		this->a_broker->pub(this->a_name, String(position ? "True" : "False"));
      	return;
	}

    // DEBUG
    Serial.printf("invalid value: %s\n", value);
}

const String LightController::getValue() const {
  return toString(this->a_value);
}

const MyAny LightController::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "bool");
}

const float LightController::getLuminosity() const {

	// Return nullptr if not found
	const MyAny val = this->a_ref_values.at(std::string(LUMINOSITY_SENSOR));

	const float * value = (const float *) val;
	if (value == nullptr) return 0.0f;

	return * value;
}

const bool LightController::getLightState() const {

	// Return nullptr if not found
	const MyAny val = this->a_ref_values.at(std::string(LIGHT_SENSOR));

	const bool * value = (const bool *) val;
	if (value == nullptr) return false;

	return * value;
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
	this->a_observers.push_back(observer);
}

void LightController::Detach(IObserver *observer) {
	for (auto itr = this->a_observers.begin(); itr != this->a_observers.end();)
	{
		if (*itr == observer)
			itr = this->a_observers.erase(itr);
		else
			++itr;
	}
}

void LightController::Notify() {
	for (IObserver* observer : this->a_observers) {
		observer->Update(LIGHT_CONTROLLER, this->getValue());
	}
}

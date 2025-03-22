//
// Created by thorgan on 3/17/25.
//

#include "TemperatureSensor.h"
#include <MyAny.h>
#include <environment.h>
#include <utils.h>

TemperatureSensor::TemperatureSensor(Broker *broker, const float value) {

    // DEBUG
    DEBUG_MODE_PRINTF("Creating TemperatureSensor with value: %f\n", value);

    this->a_name = TEMPERATURE_SENSOR;
    this->a_broker = broker;
    this->a_value = value;
}

void TemperatureSensor::setValue(const std::string &value) {

    // DEBUG
    DEBUG_MODE_PRINTF("Setting %s value %s...\n", this->getName().c_str(), value.c_str());

    const float position = parseFloat(value);

    // Ignore repeated values
    if (this->a_value == position) return;

    this->a_value = position;

    this->Notify();

    // TODO -> implement lightSensor logic when changing position

    this->a_broker->pub(this->getName(), toString(position));
}

const std::string TemperatureSensor::getValue() const {
    return toString(this->a_value, 2);
}

const MyAny TemperatureSensor::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "float");
}

void TemperatureSensor::Update(const std::string & module_name, const std::string & value) {
}

void TemperatureSensor::Attach(IObserver *observer) {
    this->a_observers.push_back(observer);
}

void TemperatureSensor::Detach(IObserver *observer) {
    for (auto itr = this->a_observers.begin(); itr != this->a_observers.end();)
    {
        if (*itr == observer)
            itr = this->a_observers.erase(itr);
        else
            ++itr;
    }
}

void TemperatureSensor::Notify() {
    for (IObserver* observer : this->a_observers) {
        observer->Update(TEMPERATURE_SENSOR, this->getValue());
    }
}

void TemperatureSensor::readInput() {
    unsigned char dht[4] = {0, 0, 0, 0};

    // Check if sensor works
    if (this->a_sensor.receive(dht)) {
        DEBUG_MODE_PRINTF("Humidity: %d.%d%%\n", dht[0], dht[1]);

        float temperature = (float) dht[2];
        if (dht[3] < 10) {
            Serial.println("Decimal part < 10");
            temperature += (float) dht[3] / 10.0f;
        }

        this->setValue(toString(temperature));
    }
    else {
        // DEBUG -> read sensor error
        DEBUG_MODE_PRINTLN("Temperature sensor error");
    }
}


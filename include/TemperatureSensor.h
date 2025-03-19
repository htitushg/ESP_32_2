//
// Created by thorgan on 3/17/25.
//

#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include <Arduino.h>
#include <IModule.h>
#include <IObservable.h>
#include <vector>

class TemperatureSensor final: public IModule {
protected:
    float a_value;
    std::vector<IObserver*> a_observers = std::vector<IObserver*>();

public:
    ~TemperatureSensor() override {
        Serial.println("TemperatureSensor::~TemperatureSensor()");
    };
    TemperatureSensor(Broker *broker, float value);
    void setValue(const char * value) override;
    const String getValue() const override;

    const MyAny getValueReference() const override;

    void addReference(MyAny value, String module_name) override {};

    void Update(const String &module_name, const String &value) override;

    void Attach(IObserver *observer) override;

    void Detach(IObserver *observer) override;

    void Notify() override;
};


#endif //TEMPERATURESENSOR_H

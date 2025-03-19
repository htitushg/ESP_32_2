//
// Created by thorgan on 3/17/25.
//

#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <Arduino.h>
#include <IModule.h>
#include <vector>

class LightSensor final : public IModule {
protected:
    bool a_value;
    std::vector<IObserver*> a_observers = std::vector<IObserver*>();

public:
    ~LightSensor() override {
        Serial.println("LightSensor::~LightSensor()");
    };
    LightSensor(Broker *broker, bool value);
    void setValue(const char * value) override;

    const String getValue() const override;

    const MyAny getValueReference() const override;

    void addReference(MyAny value, String module_name) override {};

    void Update(const String &module_name, const String &value) override;

    void Attach(::IObserver *observer) override;

    void Detach(::IObserver *observer) override;

    void Notify() override;
};

#endif //LIGHTSENSOR_H

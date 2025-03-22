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
    ~LightSensor() override {};
    LightSensor(Broker *broker, bool value);
    void setValue(const std::string &value) override;

    const std::string getValue() const override;

    const MyAny getValueReference() const override;

    void addReference(MyAny value, std::string  module_name) override {};

    void Update(const std::string  &module_name, const std::string  &value) override;

    void Attach(::IObserver *observer) override;

    void Detach(::IObserver *observer) override;

    void Notify() override;

    void readInput() override {};
};

#endif //LIGHTSENSOR_H

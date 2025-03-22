//
// Created by thorgan on 3/17/25.
//

#ifndef LUMINOSITYSENSOR_H
#define LUMINOSITYSENSOR_H

#include <Arduino.h>
#include <IModule.h>
#include <IObservable.h>
#include <vector>

class LuminositySensor final: public IModule {
protected:
    float a_value;
    std::vector<IObserver*> a_observers = std::vector<IObserver*>();

public:
    ~LuminositySensor() override {};
    LuminositySensor(Broker * broker, float value);
    void setValue(const std::string &value) override;
    const std::string getValue() const override;

    const MyAny getValueReference() const override;

    void addReference(MyAny value, std::string  module_name) override {};

    void Update(const std::string  & module_name, const std::string  & value) override;

    void Attach(IObserver *observer) override;

    void Detach(IObserver *observer) override;

    void Notify() override;

    void readInput() override {};
};


#endif //LUMINOSITYSENSOR_H

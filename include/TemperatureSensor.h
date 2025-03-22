//
// Created by thorgan on 3/17/25.
//

#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include <Arduino.h>
#include <environment.h>
#include <IModule.h>
#include <IObservable.h>
#include <vector>
#include <xht11.h>

class TemperatureSensor final: public IModule {
protected:
    float a_value;
    std::vector<IObserver*> a_observers = std::vector<IObserver*>();
    xht11 a_sensor = xht11(TEMPERATURE_SENSOR_PIN);

public:
    ~TemperatureSensor() override {};
    TemperatureSensor(Broker * broker, float value);
    void setValue(const std::string &value) override;
    const std::string getValue() const override;

    const MyAny getValueReference() const override;

    void addReference(MyAny value, std::string  module_name) override {};

    void Update(const std::string  & module_name, const std::string  & value) override;

    void Attach(IObserver * observer) override;

    void Detach(IObserver * observer) override;

    void Notify() override;

    void readInput() override;
};


#endif //TEMPERATURESENSOR_H

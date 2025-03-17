//
// Created by thorgan on 3/17/25.
//

#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#pragma once

#include <Arduino.h>
#include <IModule.h>
#include <IObservable.h>

class TemperatureSensor final: public IModule {
protected:
    float value;

public:
    ~TemperatureSensor() override = default;
    TemperatureSensor(Broker *broker, float value);
    void setValue(const char * value) override;
    const String getValue() override;

    const void *getValueReference() override;

    void Update(const ::String &value) override;

    void Attach(::IObserver *observer) override;

    void Detach(::IObserver *observer) override;

    void Notify() override;
};


#endif //TEMPERATURESENSOR_H

//
// Created by thorgan on 3/17/25.
//

#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#pragma once

#include <Arduino.h>
#include <IModule.h>

class LightSensor final : public IModule {
protected:
    bool value;

public:
    ~LightSensor() override = default;
    LightSensor(Broker *broker, bool value);
    void setValue(const char * value) override;

    const String getValue() override;

    const void *getValueReference() override;

    void Update(const ::String &value) override;

    void Attach(::IObserver *observer) override;

    void Detach(::IObserver *observer) override;

    void Notify() override;
};

#endif //LIGHTSENSOR_H

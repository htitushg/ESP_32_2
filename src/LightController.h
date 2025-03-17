//
// Created by thorgan on 3/16/25.
//

#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#pragma once

#include <Arduino.h>
#include <LittleVector.h>
#include <IModule.h>
#include <ValueReferenceMap.h>

class LightController final : public IModule {
  protected:
    bool value;
    ValueReferenceMap *ref_values;
    LittleVector<IObserver*> i_observers = LittleVector<IObserver*>();

  public:
    ~LightController() override = default;

    LightController(Broker *broker, bool value);

    void addReference(ValueReference *reference) const;

    void setValue(const char * value) override;

    const String getValue() override;

    const void *getValueReference() override;

    float getLuminosity() const;

    bool getLightState() const;

    void Update(const String &module_name, const String &value) override;

    void Attach(IObserver *observer) override;

    void Detach(IObserver *observer) override;

    void Notify() override;
};

#endif //LIGHTCONTROLLER_H

//
// Created by thorgan on 3/17/25.
//

#ifndef PRESENCEDETECTOR_H
#define PRESENCEDETECTOR_H

#pragma once

#include <Arduino.h>
#include <IModule.h>
#include <IObservable.h>

class PresenceDetector final: public IModule {
protected:
    bool value;

public:
    ~PresenceDetector() override = default;
    PresenceDetector(Broker *broker, bool value);
    void setValue(const char * value) override;
    const String getValue() override;

    const void *getValueReference() override;

    void Update(const ::String &value) override;

    void Attach(::IObserver *observer) override;

    void Detach(::IObserver *observer) override;

    void Notify() override;
};


#endif //PRESENCEDETECTOR_H

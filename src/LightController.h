//
// Created by thorgan on 3/16/25.
//

#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#pragma once

#include <Arduino.h>
#include <IModule.h>

class LightController final : public IModule {
  protected:
    const bool *is_presence;
    bool value;

  public:
    ~LightController() override = default;
    LightController(Broker *broker, const void *isPresence, bool value);
    void setValue(const char * value) override;

    const String getValue() override;

    const void *getValueReference() override;

    void Update(const String &value) override;

    void Attach(IObserver *observer) override;

    void Detach(IObserver *observer) override;

    void Notify() override;
};

#endif //LIGHTCONTROLLER_H

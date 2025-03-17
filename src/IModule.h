//
// Created by thorgan on 3/16/25.
//

#ifndef IMODULE_H
#define IMODULE_H

#pragma once

#include <Arduino.h>
#include <Broker.h>
#include <IObservable.h>

class IModule: public IObservable, public IObserver {
  protected:
    Broker *broker = nullptr;
    String name;

  public:
    ~IModule() override = default;
    virtual void setValue(const char * value) = 0;
    virtual const String getValue() = 0;
    virtual const void *getValueReference() = 0;
    String getName() { return name; }
};


#endif //IMODULE_H

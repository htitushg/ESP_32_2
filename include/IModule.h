//
// Created by thorgan on 3/16/25.
//

#ifndef IMODULE_H
#define IMODULE_H

#include <Arduino.h>
#include <Broker.h>
#include <IObservable.h>
#include <IObserver.h>
#include <MyAny.h>

class IModule: public IObservable, public IObserver {
  protected:
    Broker *a_broker = nullptr;
    String a_name;

  public:
    virtual ~IModule() override = default;
    virtual void setValue(const char * value) = 0;
    virtual const String getValue() const = 0;
    virtual const MyAny getValueReference() const = 0;

    const String getName() const { return a_name; }

    virtual void addReference(MyAny value, String module_name) = 0;
};


#endif //IMODULE_H

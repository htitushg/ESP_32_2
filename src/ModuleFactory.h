//
// Created by thorgan on 3/17/25.
//

#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#pragma once

#include <Arduino.h>
#include <IModule.h>

class ModuleFactory {
    public:
        static IModule* newModule(Broker *broker, const void *ptr, const String& type);
};


#endif //MODULEFACTORY_H

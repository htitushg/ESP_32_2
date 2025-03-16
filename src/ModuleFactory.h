//
// Created by thorgan on 3/16/25.
//

#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include <Arduino.h>
#include <Module.h>

class ModuleFactory {
  public:
    static Module* newModule(String type);
};



#endif //MODULEFACTORY_H

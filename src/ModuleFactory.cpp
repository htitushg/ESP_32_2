//
// Created by thorgan on 3/16/25.
//

#include <ModuleFactory.h>
#include <utils.h>
#include <environment.h>
#include <LightController.h>

Module* ModuleFactory::newModule(String type) {
    if (strCaseSensitiveCompare(type, LIGHT_CONTROLLER)) return new LightController(false);
    else if (strCaseSensitiveCompare(type, LIGHT_SENSOR)) return new LightController(false);
    else if (strCaseSensitiveCompare(type, LIGHT_SENSOR)) return new LightController(false);
    else if (strCaseSensitiveCompare(type, LIGHT_SENSOR)) return new LightController(false);
    else if (strCaseSensitiveCompare(type, LIGHT_SENSOR)) return new LightController(false);
    else if (strCaseSensitiveCompare(type, LIGHT_SENSOR)) return new LightController(false);
    else return nullptr;
}
//
// Created by thorgan on 3/17/25.
//

#pragma once

#include <ModuleFactory.h>
#include <utils.h>
#include <environment.h>
#include <LightController.h>
#include <LightSensor.h>
#include <PresenceDetector.h>
#include <LuminositySensor.h>
#include <TemperatureSensor.h>
#include <ConsumptionSensor.h>

IModule* ModuleFactory::newModule(Broker *broker, const void *ptr, const String& type) {
    if (strCaseSensitiveCompare(type, LIGHT_CONTROLLER)) return new LightController(broker, ptr, false);
    if (strCaseSensitiveCompare(type, LIGHT_SENSOR)) return new LightSensor(broker, false);
    if (strCaseSensitiveCompare(type, PRESENCE_DETECTOR)) return new PresenceDetector(broker, false);
    if (strCaseSensitiveCompare(type, LUMINOSITY_SENSOR)) return new LuminositySensor(broker, 0.0f);
    if (strCaseSensitiveCompare(type, TEMPERATURE_SENSOR)) return new TemperatureSensor(broker, 0.0f);
    if (strCaseSensitiveCompare(type, CONSUMPTION_SENSOR)) return new ConsumptionSensor(broker, 0.0f);
    return nullptr;
}
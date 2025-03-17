//
// Created by thorgan on 3/17/25.
//

#ifndef IOBSERVER_H
#define IOBSERVER_H

#pragma once

#include <Arduino.h>

class IObserver {
public:
    virtual ~IObserver() = default;

    virtual void Update(const String &module_name, const String &value) = 0;
};

#endif //IOBSERVER_H

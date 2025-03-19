//
// Created by thorgan on 3/17/25.
//

#ifndef IOBSERVER_H
#define IOBSERVER_H

#include <Arduino.h>

class IObserver {
public:
    virtual ~IObserver() = default;

    virtual void Update(const std::string & module_name, const std::string & value) = 0;
};

#endif //IOBSERVER_H

//
// Created by thorgan on 3/17/25.
//

#ifndef VALUEREFERENCEMAP_H
#define VALUEREFERENCEMAP_H

#include <Arduino.h>
#include <LittleVector.h>
#include <ValueReference.h>

class ValueReferenceMap {
    public:
        LittleVector<ValueReference*> values = LittleVector<ValueReference*>();

        ValueReferenceMap() = default;
        ~ValueReferenceMap() = default;

        const void* getValue(String key_name);
        void add(String key_name, const void * value);
        void remove(String key_name);
};



#endif //VALUEREFERENCEMAP_H

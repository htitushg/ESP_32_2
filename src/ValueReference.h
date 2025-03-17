//
// Created by thorgan on 3/17/25.
//

#ifndef VALUEREFERENCE_H
#define VALUEREFERENCE_H

#include <Arduino.h>

#include <utility>

class ValueReference {
    protected:
      String key;
      const void *value = nullptr;

    public:
      ValueReference(String  key, const void* value) : key(std::move(key)), value(value) {}

      ~ValueReference() = default;

      String getKey() { return key; }
      const void *getValue() const { return value; }
};



#endif //VALUEREFERENCE_H

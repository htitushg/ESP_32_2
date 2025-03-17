//
// Created by thorgan on 3/17/25.
//

#include "ValueReferenceMap.h"


const void* ValueReferenceMap::getValue(String key_name) {
  for (ValueReference* value_reference : this->values) {
    if (key_name == value_reference->getKey()) {
      return value_reference->getValue();
    }
  }
  return nullptr;
}

void ValueReferenceMap::add(String key_name, const void * value) {
  this->values.push_back(new ValueReference(key_name, value));
}

void ValueReferenceMap::remove(String key_name) {
  for (LittleVector<ValueReference*>::iterator it = this->values.begin(); it != this->values.end(); it++) {
    if (key_name == (*it)->getKey()) {
      this->values.erase(it);
    }
  }
}

//
// Created by thorgan on 3/16/25.
//

#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>

class Module {
  protected:
    String name;

  public:
    virtual void setValue(const char * value);
    virtual String getValue();
    String getName() { return name; }
};



#endif //MODULE_H

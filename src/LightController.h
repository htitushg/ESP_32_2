//
// Created by thorgan on 3/16/25.
//

#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#include <Arduino.h>
#include <Module.h>

class LightController: public Module {
  protected:
    bool value;

  public:
    LightController(bool value);
    void setValue(const char * value);
	String getValue();
};

#endif //LIGHTCONTROLLER_H

//
// Created by thorgan on 3/16/25.
//

#include "LightController.h"
#include <utils.h>

LightController::LightController(bool value){
	this->value = value;
}

void LightController::setValue(const char * newValue) {
	if (isBool(newValue)) {
          this->value = parseBool(newValue);
          return;
	}

    // DEBUG
    Serial.printf("invalid value: %s\n", newValue);
}

String LightController::getValue(){
  return toString(this->value);
}

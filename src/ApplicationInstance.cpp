//
// Created by thorgan on 3/16/25.
//

#include "ApplicationInstance.h"

Application* ApplicationInstance::getInstance() {
  if (ApplicationInstance::app == nullptr) ApplicationInstance::app = new Application();
  return ApplicationInstance::app;
}
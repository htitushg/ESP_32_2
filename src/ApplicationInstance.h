//
// Created by thorgan on 3/16/25.
//

#ifndef APPLICATIONINSTANCE_H
#define APPLICATIONINSTANCE_H

#include <Application.h>

class ApplicationInstance {
  protected:
    static Application *app;

  public:
    static Application* getInstance();
};



#endif //APPLICATIONINSTANCE_H

//
// Created by thorgan on 3/16/25.
//

#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#include <Arduino.h>
#include <vector>
#include <map>
#include <IModule.h>
#include <MyAny.h>


class LightController final : public IModule {
  protected:
    bool a_value;
    std::map<std::string, MyAny> a_ref_values;
    std::vector<IObserver *> a_observers = std::vector<IObserver *>();

  public:
    ~LightController() override {
      Serial.println("LightController::~LightController()");
    };

    LightController(Broker * broker, bool value);

    void addReference(MyAny value, String module_name) override;

    void setValue(const char * value) override;

    const String getValue() const override;

    const MyAny getValueReference() const override;

    // FIXME/TODO -> remove at the end if not used
    // If other references are needed, might be useful
    // MyAny Get(const String & att) {
    //   if (att == "value") {
    //     return MyAny((void *) & a_value, "bool");
    //   }
    // };

    const float getLuminosity() const;

    const bool getLightState() const;

    void Update(const String & module_name, const String & value) override;

    void Attach(IObserver * observer) override;

    void Detach(IObserver * observer) override;

    void Notify() override;
};

#endif //LIGHTCONTROLLER_H

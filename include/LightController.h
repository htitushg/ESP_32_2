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
    ~LightController() override {};

    LightController(Broker * broker, bool value);

    void addReference(MyAny value, std::string module_name) override;

    void setValue(const std::string &value) override;

    const std::string getValue() const override;

    const MyAny getValueReference() const override;

    const float getLuminosity() const;

    const bool getLightState() const;

    void Update(const std::string & module_name, const std::string & value) override;

    void Attach(IObserver * observer) override;

    void Detach(IObserver * observer) override;

    void Notify() override;

    void readInput() override {};
};

#endif //LIGHTCONTROLLER_H

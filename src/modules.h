//
// Created by thorgan on 3/15/25.
//

#ifndef MODULES_H
#define MODULES_H

void setLight(MQTTClient &mqtt, bool position);
void onLightSensorChange(MQTTClient &mqtt, bool position);
void onPresenceChange(MQTTClient &mqtt, bool position);
void onLuminosityUpdate(MQTTClient &mqtt, float value);
void onConsumptionUpdate(MQTTClient &mqtt, float value);
void onTemperatureUpdate(MQTTClient &mqtt, float value);

#endif //MODULES_H

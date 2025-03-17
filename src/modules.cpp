//
// Created by thorgan on 3/15/QOS5.
//
// #pragma once

// #include <Arduino.h>
// #include <environment.h>

// void setLight(const bool position) {
//
//   // Ignore repeated values
//   if (turn_light == position) return;
//
//   turn_light = position;
//   if (turn_light) {
//     digitalWrite(LIGHT, HIGH);
//   } else {
//     digitalWrite(LIGHT, LOW);
//   }
//
//   publishValue(LIGHT_CONTROLLER, String(position ? "True" : "False"));
// }
//
// void onLightSensorChange(const bool position) {
//   // Ignore known changes
//   if (is_light == position) return;
//
//   // Change light value
//   is_light = position;
//
//   publishValue(LIGHT_SENSOR, String(position ? "True" : "False"));
// }
//
// void onPresenceChange(const bool position) {
//   // Ignore known changes
//   if (is_presence == position) return;
//
//   // change presence value
//   is_presence = position;
//
//   publishValue(PRESENCE_DETECTOR, String(position ? "True" : "False"));
// }
//
// void onLuminosityUpdate(const float value) {
//   // TODO -> Remove equality check when using real sensor with no subscription to luminositySensor channel
//   // Ignore known changes (prevents infinite loop when subscribing to luminositySensor channel)
//   if (luminosity == value) return;
//
//   // change luminosity value
//   luminosity = value;
//
//   publishValue(LUMINOSITY_SENSOR, String(luminosity));
// }
//
// void onConsumptionUpdate(const float value) {
//   // TODO -> Remove equality check when using real sensor with no subscription to consumptionSensor channel
//   // Ignore known changes (prevents infinite loop when subscribing to consumptionSensor channel)
//   if (consumption == value) return;
//
//   // change consumption value
//   consumption = value;
//
//   publishValue(CONSUMPTION_SENSOR, String(consumption));
// }
//
// void onTemperatureUpdate(const float value) {
//   // TODO -> Remove equality check when using real sensor with no subscription to temperatureSensor channel
//   // Ignore known changes (prevents infinite loop when subscribing to temperatureSensor channel)
//   if (temperature == value) return;
//
//   // change temperature value
//   temperature = value;
//
//   publishValue(TEMPERATURE_SENSOR, String(temperature));
// }
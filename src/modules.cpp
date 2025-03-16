//
// Created by thorgan on 3/15/QOS5.
//
#pragma once

#include <Arduino.h>
#include <environment.h>
#include <pub_sub.h>

void setLight(MQTTClient &mqtt, const bool position) {

  // Ignore repeated values
  if (turn_light == position) return;

  turn_light = position;
  if (turn_light) {
    digitalWrite(LIGHT, HIGH);
  } else {
    digitalWrite(LIGHT, LOW);
  }

  publishValue(mqtt, light_controller_channel, String(position ? "True" : "False"));
}

void onLightSensorChange(MQTTClient &mqtt, const bool position) {
  // Ignore known changes
  if (is_light == position) return;

  // Change light value
  is_light = position;

  publishValue(mqtt, light_sensor_channel, String(position ? "True" : "False"));
}

void onPresenceChange(MQTTClient &mqtt, const bool position) {
  // Ignore known changes
  if (is_presence == position) return;

  // change presence value
  is_presence = position;

  publishValue(mqtt, presence_detector_channel, String(position ? "True" : "False"));
}

void onLuminosityUpdate(MQTTClient &mqtt, const float value) {
  // TODO -> Remove equality check when using real sensor with no subscription to luminositySensor channel
  // Ignore known changes (prevents infinite loop when subscribing to luminositySensor channel)
  if (luminosity == value) return;

  // change luminosity value
  luminosity = value;

  publishValue(mqtt, luminosity_sensor_channel, String(luminosity));
}

void onConsumptionUpdate(MQTTClient &mqtt, const float value) {
  // TODO -> Remove equality check when using real sensor with no subscription to consumptionSensor channel
  // Ignore known changes (prevents infinite loop when subscribing to consumptionSensor channel)
  if (consumption == value) return;

  // change consumption value
  consumption = value;

  publishValue(mqtt, consumption_sensor_channel, String(consumption));
}

void onTemperatureUpdate(MQTTClient &mqtt, const float value) {
  // TODO -> Remove equality check when using real sensor with no subscription to temperatureSensor channel
  // Ignore known changes (prevents infinite loop when subscribing to temperatureSensor channel)
  if (temperature == value) return;

  // change temperature value
  temperature = value;

  publishValue(mqtt, temperature_sensor_channel, String(temperature));
}
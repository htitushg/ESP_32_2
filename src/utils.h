//
// Created by thorgan on 3/15/25.
//

#ifndef UTILS_H
#define UTILS_H

#pragma once

#include <Arduino.h>
#include <WiFi.h>


String getChannelModule(char *topic);
bool strCaseInsensitiveCompare(const String& str, const char chars[]);
bool strCaseSensitiveCompare(const String& str, const char chars[]);
int parseInt(const String& str);
float parseFloat(const String& str);
bool parseBool(const String& str);
String toString(bool b);
String toString(int i);
String toString(long l);
String toString(unsigned long ul);
String toString(float f);
String toString(double d);
bool isBool(const String& str);
const char* wl_status_to_string(const wl_status_t status);
bool fromBoolPtr(const void * reference);
float fromFloatPtr(const void * reference);
String fromStringPtr(const void * reference);
int fromIntPtr(const void * reference);

#endif //UTILS_H

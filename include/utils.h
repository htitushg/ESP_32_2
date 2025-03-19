//
// Created by thorgan on 3/15/25.
//

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <WiFi.h>

std::string getChannelModule(char *topic);
bool strCaseInsensitiveCompare(const std::string & str, const char chars[]);
bool strCaseSensitiveCompare(const std::string & str, const char chars[]);
int parseInt(const std::string & str);
float parseFloat(const std::string & str);
bool parseBool(const std::string & str);
std::string toString(bool b);
std::string toString(int i);
std::string toString(unsigned int ui);
std::string toString(long l);
std::string toString(unsigned long ul);
std::string toString(float f);
std::string toString(float f, int precision);
std::string toString(double d);
bool isBool(const std::string & str);
const char* wl_status_to_string(wl_status_t status);

#endif //UTILS_H

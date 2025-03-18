//
// Created by thorgan on 3/15/25.
//

#include <Arduino.h>
#include <utils.h>
#include <environment.h>

// TODO -> remove if not necessary in the end
String millisToDate() {
    const unsigned long now = millis();
    const unsigned long days = now / DAY_MILLIS;
    const unsigned long hours = (now % DAY_MILLIS) / HOUR_MILLIS;
    const unsigned long minutes = ((now % DAY_MILLIS) % HOUR_MILLIS) / MINUTE_MILLIS;
    const unsigned long seconds = (((now % DAY_MILLIS) % HOUR_MILLIS) % MINUTE_MILLIS) / SECOND_MILLIS;

    // 999_999 days 23:59:59 max
    char buffer[20];
    String daysMsg = "";
    if (days > 0 ) {
        if (days == 1) {
            daysMsg = String(days) + " day ";
        } else {
            daysMsg = String(days) + " days ";
        }
    }
    sprintf(buffer, "%s%02d:%02d:%02d", daysMsg.c_str(), hours, minutes, seconds);
    return {buffer};
}

String getChannelModule(char *topic) {
    char * elements[6];
    char * p;
    int i = 0;
    p = strtok(topic, "/");
    while(p != nullptr)
    {
        elements[i] = p;
        p = strtok(nullptr, ",");
        ++i;
    }
    if (elements[5] == nullptr) return "";

    return elements[5];
}

bool strCaseInsensitiveCompare(const String& str, const char chars[]) {
    constexpr size_t str_len = sizeof(str) / sizeof(str[0]);
    const size_t chars_len = strlen(chars);
    if (chars_len != str_len) return false;

    for (int i = 0; i < str_len; i++) {
        if (toUpperCase(str[i]) != toUpperCase(chars[i])) return false;
    }

    return true;
}

bool strCaseSensitiveCompare(const String& str, const char chars[]) {
    constexpr size_t str_len = sizeof(str) / sizeof(str[0]);
    const size_t chars_len = strlen(chars);
    if (chars_len != str_len) return false;

    return str == chars;
}

int parseInt(const String& str) {
    return atoi(str.c_str());
}

float parseFloat(const String& str) {
    return atof(str.c_str());
}

bool parseBool(const String& str) {
    if (strCaseInsensitiveCompare(str, "True")) return true;
    if (strCaseInsensitiveCompare(str, "1")) return true;
    if (strCaseInsensitiveCompare(str, "False")) return false;
    if (strCaseInsensitiveCompare(str, "0")) return false;
    throw std::invalid_argument( "received non-boolean value to parse" );
}

String toString(const bool b) {
    return b ? "True" : "False";
}
String toString(const int i) {
    return String(i);
}
String toString(const long l) {
    return String(l);
}
String toString(const unsigned long ul) {
    return String(ul);
}
String toString(const float f) {
    return String(f);
}
String toString(const double d) {
    return String(d);
}
bool isBool(const String& str) {
    if (strCaseInsensitiveCompare(str, "True")) return true;
    if (strCaseInsensitiveCompare(str, "False")) return true;
    if (strCaseInsensitiveCompare(str, "0")) return true;
    if (strCaseInsensitiveCompare(str, "1")) return true;
    return false;
}

const char* wl_status_to_string(const wl_status_t status) {
    switch (status) {
        case WL_NO_SHIELD: return "WL_NO_SHIELD";
        case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
        case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
        case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
        case WL_CONNECTED: return "WL_CONNECTED";
        case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
        case WL_DISCONNECTED: return "WL_DISCONNECTED";
    }
    return "Invalid WL_STATUS";
}

bool fromBoolPtr(const void * reference) {
    if (reference == nullptr) return false;
    const auto value = static_cast<const bool*>(reference);
    return *value;
}

float fromFloatPtr(const void * reference) {
    if (reference == nullptr) return 0.0f;
    const auto value = static_cast<const float*>(reference);
    return *value;
}

String fromStringPtr(const void * reference) {
    if (reference == nullptr) return "";
    const auto value = static_cast<const char*>(reference);
    return {value};
}

int fromIntPtr(const void * reference) {
    if (reference == nullptr) return 0;
    const auto value = static_cast<const int*>(reference);
    return *value;
}

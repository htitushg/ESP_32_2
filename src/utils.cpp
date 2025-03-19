//
// Created by thorgan on 3/15/25.
//

#include <Arduino.h>
#include <utils.h>
#include <environment.h>

extern std::unique_ptr<bool> IS_DEBUG_MODE;

// TODO -> remove if not necessary in the end
std::string millisToDate() {
    const unsigned long now = millis();
    const unsigned long days = now / DAY_MILLIS;
    const unsigned long hours = (now % DAY_MILLIS) / HOUR_MILLIS;
    const unsigned long minutes = ((now % DAY_MILLIS) % HOUR_MILLIS) / MINUTE_MILLIS;
    const unsigned long seconds = (((now % DAY_MILLIS) % HOUR_MILLIS) % MINUTE_MILLIS) / SECOND_MILLIS;

    // 999_999 days 23:59:59 max
    char buffer[20];
    std::string daysMsg = "";
    if (days > 0 ) {
        if (days == 1) {
            daysMsg = toString(days) + " day ";
        } else {
            daysMsg = toString(days) + " days ";
        }
    }
    sprintf(buffer, "%s%02d:%02d:%02d", daysMsg.c_str(), hours, minutes, seconds);
    return {buffer};
}

std::string getChannelModule(char * topic) {

	if (*IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("Splitting topic %s into bits...\n", topic);
	}

    char * elements[6];
    int i = 0;
    char * p = strtok(topic, "/");
    while(p != nullptr)
    {
        elements[i] = p;

	    if (*IS_DEBUG_MODE) {
            // DEBUG
            Serial.printf("%d. Got: %s\n", i, p);
	    }

        p = strtok(nullptr, "/");
        ++i;
    }
    if (elements[5] == nullptr) return "";

	if (*IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("Returning %s\n", elements[5]);
	}

    return elements[5];
}

bool strCaseInsensitiveCompare(const std::string & str, const char chars[]) {

	if (*IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("strCaseInsensitiveCompare '%s' and '%s'\n", str.c_str(), chars);
	}

    const auto str2 = std::string(chars);

    if (str.length() != str2.length()) return false;

    for (int i = 0; i < str.length(); i++) {
        if (toupper(str[i]) != toupper(str2[i])) return false;
    }

    return true;
}

bool strCaseSensitiveCompare(const std::string& str, const char chars[]) {

	if (*IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("strCaseSensitiveCompare '%s' and '%s'\n", str.c_str(), chars);
	}

    return strcmp(str.c_str(), chars) == 0;
}

int parseInt(const std::string & str) {
    return atoi(str.c_str());
}

float parseFloat(const std::string & str) {
    return atof(str.c_str());
}

bool parseBool(const std::string & str) {
    if (strCaseInsensitiveCompare(str, "True")) return true;
    if (strCaseInsensitiveCompare(str, "1")) return true;
    if (strCaseInsensitiveCompare(str, "False")) return false;
    if (strCaseInsensitiveCompare(str, "0")) return false;
    throw std::invalid_argument( "received non-boolean value to parse" );
}

std::string toString(const bool b) {
    return b ? "True" : "False";
}
std::string toString(const int i) {
    return std::to_string(i);
}
std::string toString(const unsigned int ui) {
    return std::to_string(ui);
}
std::string toString(const long l) {
    return std::to_string(l);
}
std::string toString(const unsigned long ul) {
    return std::to_string(ul);
}
std::string toString(const float f) {
    return std::to_string(f);
}
std::string toString(const float f, const int precision) {
    char result;
    sprintf(&result, "%.*f", precision, f);
    return std::to_string(result);
}
std::string toString(const double d) {
    return std::to_string(d);
}
bool isBool(const std::string & str) {
    if (strCaseInsensitiveCompare(str, "True")) return true;
    if (strCaseInsensitiveCompare(str, "False")) return true;
    if (strCaseInsensitiveCompare(str, "0")) return true;
    if (strCaseInsensitiveCompare(str, "1")) return true;
    return false;
}

const char* wl_status_to_string(const wl_status_t status) {
    switch (status) {
        case WL_NO_SHIELD: return "NO_SHIELD";
        case WL_IDLE_STATUS: return "IDLE_STATUS";
        case WL_NO_SSID_AVAIL: return "NO_SSID_AVAIL";
        case WL_SCAN_COMPLETED: return "SCAN_COMPLETED";
        case WL_CONNECTED: return "CONNECTED";
        case WL_CONNECT_FAILED: return "CONNECT_FAILED";
        case WL_CONNECTION_LOST: return "CONNECTION_LOST";
        case WL_DISCONNECTED: return "DISCONNECTED";
    }
    return "Invalid STATUS";
}

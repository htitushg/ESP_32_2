//
// Created by thorgan on 18/03/25.
//

#ifndef MYANY_H
#define MYANY_H

#include <Arduino.h>

class MyAny {

private:
    void * a_value;
    String a_type;

public:
    operator int * () {
        if (a_type == "int") {
            return (int *)a_value;
        }
        return nullptr;
    };
    operator const int * () const {
        if (a_type == "int") {
            return (int *)a_value;
        }
        return nullptr;
    };
    operator bool * () {
        if (a_type == "bool") {
            return (bool *)a_value;
        }
        return nullptr;
    };
    operator const bool * () const {
        if (a_type == "bool") {
            return (bool *)a_value;
        }
        return nullptr;
    };
    operator float * () {
        if (a_type == "float") {
            return (float *)a_value;
        }
        return nullptr;
    };
    operator const float * () const {
        if (a_type == "float") {
            return (float *)a_value;
        }
        return nullptr;
    };
    operator String * () {
        if (a_type == "String") {
            return (String *)a_value;
        }
        return nullptr;
    };
    operator const String * () const {
        if (a_type == "String") {
            return (String *)a_value;
        }
        return nullptr;
    };

    MyAny() = default;
    MyAny(const MyAny &other) = default;

    MyAny(void * value, const char * str): a_value(value), a_type(str) {};
};

#endif //MYANY_H

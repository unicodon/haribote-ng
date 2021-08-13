#pragma once

#include "Value.h"

Value parseJson(const char* data, size_t size);
Value parseJson(const std::string& str);
std::string stringify(const Value& value);

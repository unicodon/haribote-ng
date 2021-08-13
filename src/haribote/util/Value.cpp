#include "Value.h"

#include "Json.h"

void* enabler = nullptr;

std::ostream& operator<<(std::ostream& os, const Value& value)
{
	os << stringify(value);
	return os;
}

const char* Value::kTypeNames[7] = {
	"Null",
	"Bool",
	"Int",
	"Double",
	"String",
	"Array",
	"Object"
};

#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>

extern void* enabler;

class Value {
public:
	typedef bool Bool;
	typedef int64_t Int;
	typedef double Double;
	typedef std::string String;
	typedef std::vector<Value> Array;
	typedef std::map<std::string, Value> Object;

	enum class Type {
		kNull,
		kBool,
		kInt,
		kDouble,
		kString,
		kArray,
		kObject
	};

	static const char* kTypeNames[7];

	struct UnexpectedTypeException : public std::exception {
	public:
		UnexpectedTypeException(Type actual, Type expected)
			: actual(actual)
			, expected(expected)
		{
			char buf[64];
			snprintf(buf, sizeof buf, "UnexpectedTypeException: actual=%s expected=%s", kTypeNames[(int)actual], kTypeNames[(int)expected]);
			message = buf;
		}

		virtual const char* what() const override
		{
			return message.c_str();
		}

		Type actual;
		Type expected;
		std::string message;
	};

	~Value() { reset(); }

	//---------------------------------
	explicit Value(void)  noexcept : m_type(Type::kNull) {}
	explicit Value(nullptr_t)  noexcept : m_type(Type::kNull) {}
	explicit Value(Bool b)  noexcept : m_type(Type::kBool), m_bool(b) {}
	explicit Value(Int n)  noexcept : m_type(Type::kInt), m_int(n) {}
	explicit Value(Double n)  noexcept : m_type(Type::kDouble), m_double(n) {}
	Value(String&& s) : m_type(Type::kString) { assignString(std::move(s)); }
	Value(Array&& a) : m_type(Type::kArray) { assignArray(std::move(a)); }
	Value(Object&& o) : m_type(Type::kObject) { assignObject(std::move(o)); }

	explicit Value(std::initializer_list<Value> l) : m_type(Type::kArray) { assignArray(std::move(l)); }
	explicit Value(std::initializer_list<std::pair<std::string const, Value>> l) : m_type(Type::kObject) { assignObject(std::move(l)); }

	Value(Value&& rhs) { assign(std::move(rhs)); }

	explicit Value(const char* s) : m_type(Type::kString) { assignString(s); }
	explicit Value(std::exception) : m_type(Type::kNull) {}

	template <typename T, typename std::enable_if<std::is_integral<T>::value>::type *& = enabler>
	Value(T t) noexcept : m_type(Type::kInt), m_int(t) {}

	template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type *& = enabler>
	Value(T t) noexcept : m_type(Type::kDouble), m_double(t) {}

	template <typename T, typename std::enable_if<std::is_rvalue_reference<T>::value>::type *& = enabler>
	Value(T&& t) : Value(move(t)) {}

	explicit Value(const String& s) = delete;
	explicit Value(const Array& a) = delete;
	explicit Value(const Object& o) = delete;
	Value& operator=(const Value& rhs) = delete;
	Value(const Value& rhs)
	{
		assign(rhs);
	}

	//---------------------------------
	Value& operator=(Value&& rhs) { reset(); assign(std::move(rhs)); return *this; }
	Value& operator=(const char* s) { reset();  m_type = Type::kString; assignString(s); return *this; }
	Value& operator=(String&& s) { reset();  m_type = Type::kString; assignString(move(s)); return *this; }

	//---------------------------------
	Type type() const noexcept { return m_type; }

	bool isNull() const noexcept { return m_type == Type::kNull; }
	bool isBool() const noexcept { return m_type == Type::kBool; }
	bool isInt() const noexcept { return m_type == Type::kInt; }
	bool isDouble() const noexcept { return m_type == Type::kDouble; }
	bool isString() const noexcept { return m_type == Type::kString; }
	bool isArray() const noexcept { return m_type == Type::kArray; }
	bool isObject() const noexcept { return m_type == Type::kObject; }
	bool isNumber() const noexcept { return isDouble() || isInt(); }

	//---------------------------------
	Bool asBool() const { ensureType<Type::kBool>(); return m_bool; }
	Int asInt() const { ensureType<Type::kInt>(); return m_int; }
	Double asDouble() const { ensureType<Type::kDouble>(); return m_double; }

	Double asNumber() const
	{
		if (m_type == Type::kDouble) return m_double;
		if (m_type == Type::kInt) return (Double)m_int;
		throw UnexpectedTypeException{ Type::kDouble, m_type };
	}

	String& asString() { ensureType<Type::kString>(); return m_string; }
	Array& asArray() { ensureType<Type::kArray>(); return m_array; }
	Object& asObject() { ensureType<Type::kObject>(); return m_object; }

	const String& asString() const { ensureType<Type::kString>(); return m_string; }
	const Array& asArray() const { ensureType<Type::kArray>(); return m_array; }
	const Object& asObject() const { ensureType<Type::kObject>(); return m_object; }

	//---------------------------------
	size_t arraySize() const { return asArray().size(); }
	Value& operator[](size_t i) { return asArray()[i]; }
	const Value& operator[](size_t i) const { return asArray()[i]; }

	Array::iterator begin() { return asArray().begin(); }
	Array::const_iterator begin() const { return asArray().begin(); }
	Array::iterator end() { return asArray().end(); }
	Array::const_iterator end() const { return asArray().end(); }

	void push_back(Value&& v) { asArray().push_back(std::move(v)); }

	//---------------------------------
	size_t objectSize() const { return asObject().size(); }

	Value& operator[](const std::string& key) { return asObject()[key]; }

	//---------------------------------
	const char* typeName() const { return kTypeNames[(int)m_type]; }
	size_t size() const
	{
		switch (m_type) {
		case Type::kArray:
			return arraySize();
		case Type::kObject:
			return objectSize();
		default:
			return 1;
		}
	}

private:
	// new/delete prohibited
	void* operator new(size_t, void* p) = delete;
	void* operator new[](size_t, void* p) = delete;
	void* operator new(size_t size) = delete;
	void* operator new[](size_t size) = delete;
	void operator delete[](void* p) = delete;
	void assign(const Value& rhs)
	{
		m_type = rhs.m_type;
		switch (m_type) {
		case Type::kNull:
			break;
		case Type::kBool:
			m_bool = rhs.m_bool;
			break;
		case Type::kInt:
			m_int = rhs.m_int;
			break;
		case Type::kDouble:
			m_double = rhs.m_double;
			break;
		case Type::kString:
			assignString(rhs.m_string);
			break;
		case Type::kArray:
			assignArray(rhs.m_array);
			break;
		case Type::kObject:
			assignObject(rhs.m_object);
			break;
		}
	}

	void assign(Value&& rhs)
	{
		m_type = rhs.m_type;
		switch (m_type) {
		case Type::kNull:
			break;
		case Type::kBool:
			m_bool = rhs.m_bool;
			break;
		case Type::kInt:
			m_int = rhs.m_int;
			break;
		case Type::kDouble:
			m_double = rhs.m_double;
			break;
		case Type::kString:
			assignString(std::move(rhs.m_string));
			break;
		case Type::kArray:
			assignArray(std::move(rhs.m_array));
			break;
		case Type::kObject:
			assignObject(std::move(rhs.m_object));
			break;
		}
	}

	template<typename Arg>
	void assignString(Arg&& a) { new (&m_string) String(std::forward<Arg>(a)); }
	template<typename Arg>
	void assignArray(Arg&& a) { new (&m_array) Array(std::forward<Arg>(a)); }
	template<typename Arg>
	void assignObject(Arg&& a) { new (&m_object) Object(std::forward<Arg>(a)); }

	void reset()
	{
		// Non-trivial member destruction.
		switch (m_type) {
		case Type::kString:
			m_string.~String();
			break;
		case Type::kArray:
			m_array.~Array();
			break;
		case Type::kObject:
			m_object.~Object();
			break;
		default:
			break;
		}
		// Reset the type to NULL.
		m_type = Type::kNull;
	}

	template<Type type>
	void ensureType() const
	{
		if (m_type != type) throw UnexpectedTypeException{ type, m_type };
	}

	Type m_type;
	union {
		Bool m_bool;
		Int m_int;
		Double m_double;
		String m_string;
		Array m_array;
		Object m_object;
	};
};

std::ostream& operator<<(std::ostream& os, const Value& value);

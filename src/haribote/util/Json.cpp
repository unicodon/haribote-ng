#include "Json.h"

#include <map>
#include <memory>
#include <vector>
#include <string>
#include <cassert>
#include <limits>
#include <stdio.h>

using namespace std;

namespace
{

struct ParserException {
	int position;
	int character;
	int lineno;
};

#define THROW_PARSER_EXCEPTION throw ParserException { r.pos(), r.peek(), __LINE__ }

enum {
	kWS = 1 << 0,
	kBEGIN_ARRAY   = 1 << 1,
	kEND_ARRAY     = 1 << 2,
	kBEGIN_OBJECT  = 1 << 3,
	kEND_OBJECT    = 1 << 4,
	kNAME_SEPARATOR  = 1 << 5,
	kVALUE_SEPARATOR = 1 << 6
};

//
// Parser helper
//

// This function returns the next non-WS character.
template<class R> int skipWS(R& r)
{
	while (true) {
		int c = r.peek();
		if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
			r.read();
			continue;
		}
		return c;
	}
}

template<class R> bool parse4HexDigit(R& r, std::string &s)
{
	int u = 0; // unicode
	// Parse hex digit
	for (int i = 0; i < 4; i++) {
		u <<= 4;
		int c = r.read();
		if (c >= '0' && c <= '9') {
			u |= c - '0';
		}
		else if (c >= 'A' && c <= 'F') {
			u |= c - 'A' + 10;
		}
		else if (c >= 'a' && c <= 'f') {
			u |= c - 'a' + 10;
		}
		else {
			return false;
		}
	}
	// To UTF-8
	if (u < 0x80) {
		s.push_back(u);
		return true;
	}
	else if (u < 0x800) {
		int c1 = (u & 0x3F) | 0x80; u >>= 6;
		int c0 = u | 0xC0;
		s.push_back(c0);
		s.push_back(c1);
		return true;
	}
	else if (u < 0x10000) {
		int c2 = (u & 0x3F) | 0x80; u >>= 6;
		int c1 = (u & 0x3F) | 0x80; u >>= 6;
		int c0 = u | 0xE0;
		s.push_back(c0);
		s.push_back(c1);
		s.push_back(c2);
		return true;
	}
	else if (u < 0x11000) {
		int c3 = (u & 0x3F) | 0x80; u >>= 6;
		int c2 = (u & 0x3F) | 0x80; u >>= 6;
		int c1 = (u & 0x3F) | 0x80; u >>= 6;
		int c0 = u | 0xF0;
		s.push_back(c0);
		s.push_back(c1);
		s.push_back(c2);
		s.push_back(c3);
		return true;
	}
	return false;
}

template<class R> Value parseValue(R& r);
template<class R> Value parseNumber(R& r);
template<class R> Value parseString(R& r);
template<class R> Value parseArray(R& r);
template<class R> Value parseObject(R& r);

template<class R> Value parseValue(R& r)
{
	skipWS<R>(r);
	int c = r.peek();
	if (c == r.kEOS || c == '}' || c == ']' || c == ':' || c == ',') {
		THROW_PARSER_EXCEPTION;
	}

	if (c == '{') { // Object
		return parseObject(r);
	}
	else if (c == '[') { // Array
		return parseArray(r);
	}
	else if (c == '\"') { // String 
		return parseString(r);
	}
	else if (c == 'n') { // null
		r.read();
		if (r.read() == 'u' && r.read() == 'l' && r.read() == 'l') {
			return Value(nullptr);
		}
		THROW_PARSER_EXCEPTION;
	}
	else if (c == 'f') { // false
		r.read();
		if (r.read() == 'a' && r.read() == 'l' && r.read() == 's' && r.read() == 'e') {
			return Value(false);
		}
		THROW_PARSER_EXCEPTION;
	}
	else if (c == 't') { // true
		r.read();
		if (r.read() == 'r' && r.read() == 'u' && r.read() == 'e') {
			return Value(true);
		}
		THROW_PARSER_EXCEPTION;
	}
	else { // Number?
		return parseNumber(r);
	}
	THROW_PARSER_EXCEPTION;
}

template<class R> Value parseNumber(R& r)
{
	skipWS<R>(r);
	bool isInt = true;
	char tmp[128];
	char* ptr = tmp;
	char* end = tmp + sizeof(tmp) / sizeof(tmp[0]) - 1;
	while (true) {
		int c = r.peek();
		if (c == '-' || c == '+' || (c >= '0' && c <= '9')) {
			*ptr++ = r.read();
			if (ptr >= end)
				throw range_error("Too long number string");
			continue;
		}
		if (c == '.' || c == 'E' || c == 'e') {
			isInt = false;
			*ptr++ = r.read();
			if (ptr >= end)
				throw range_error("Too long number string");
			continue;
		}
		break;
	}
	*ptr = '\0';
	size_t length = ptr - tmp;
	if (isInt) {
		int64_t n = strtoll(tmp, &end, 10);
		if (tmp + length == end) {
			return Value(n);
		}
	}
	else {
		double d = strtod(tmp, &end);
		if (tmp + length == end) {
			return Value(d);
		}
	}
	THROW_PARSER_EXCEPTION;
}

template<class R> Value parseString(R& r)
{
	skipWS<R>(r);

	const char* ptr;
	size_t size;
	if (r.tryReadUnescapedString(ptr, size)) {
		Value::String s(ptr, size);
		return Value(std::move(s));
	}

	if (r.read() != '\"') // begin-string
		THROW_PARSER_EXCEPTION;

	Value::String s;
	while (true) {
		int c = r.read();
		if (c == '"') { // end-string
			return Value(std::move(s));
		}

		if (c == r.kEOS)
			THROW_PARSER_EXCEPTION;

		if ((c & 0x80) == 0x00) { // 1 Byte
			if (c < 0x20) // control characters
				THROW_PARSER_EXCEPTION;
			if (c != '\\') { // unescaped
				s.push_back(c);
			}
			else {
				c = r.read();
				if (c == 0x22) { // "
					s.push_back(0x22);
				}
				else if (c == 0x5C) {
					s.push_back(0x5C);
				}
				else if (c == 0x2F) { // /
					s.push_back(0x2F);
				}
				else if (c == 'b') { // backspace
					s.push_back(0x08);
				}
				else if (c == 'f') { // form feed
					s.push_back(0x0C);
				}
				else if (c == 'n') { // line feed
					s.push_back(0x0A);
				}
				else if (c == 'r') { // carriage return
					s.push_back(0x0D);
				}
				else if (c == 't') { // tab
					s.push_back(0x09);
				}
				else if (c == 'u') { // 4HEXDIG
					if (!parse4HexDigit(r, s))
						THROW_PARSER_EXCEPTION;
				}
				else {
					THROW_PARSER_EXCEPTION;
				}
			}
		}
		else if ((c & 0xE0) == 0xC0) { // 2 Bytes
			int c1 = r.read();
			if ((c1 & 0xC0) != 0x80)
				THROW_PARSER_EXCEPTION;
			s.push_back(c);
			s.push_back(c1);
		}
		else if ((c & 0xF0) == 0xE0) { // 3 Bytes
			int c1 = r.read();
			if ((c1 & 0xC0) != 0x80)
				THROW_PARSER_EXCEPTION;
			int c2 = r.read();
			if ((c2 & 0xC0) != 0x80)
				THROW_PARSER_EXCEPTION;
			s.push_back(c);
			s.push_back(c1);
			s.push_back(c2);
		}
		else if ((c & 0xF8) == 0xF0) { // 4 Bytes
			int c1 = r.read();
			if ((c1 & 0xC0) != 0x80)
				THROW_PARSER_EXCEPTION;
			int c2 = r.read();
			if ((c2 & 0xC0) != 0x80)
				THROW_PARSER_EXCEPTION;
			int c3 = r.read();
			if ((c3 & 0xC0) != 0x80)
				THROW_PARSER_EXCEPTION;
			s.push_back(c);
			s.push_back(c1);
			s.push_back(c2);
			s.push_back(c3);
		}
		else {
			THROW_PARSER_EXCEPTION;
		}
	}
	THROW_PARSER_EXCEPTION;
}

template<class R> Value parseArray(R& r)
{
	skipWS<R>(r);
	if (r.read() != '[') // begin-array
		THROW_PARSER_EXCEPTION;
	Value::Array array;

	while (true) {
		skipWS<R>(r);
		int c = r.peek();
		if (c == ']') { // end-array
			r.read();
			return Value(std::move(array));
		}
		if (c == ',') {
			if (array.empty())
				THROW_PARSER_EXCEPTION;
			r.read();
		}
		else if (!array.empty()) {
			THROW_PARSER_EXCEPTION;
		}

		if (array.empty()) {
			array.reserve(4);
		}
		array.emplace_back(parseValue(r));
	}
	THROW_PARSER_EXCEPTION;
}

template<class R> Value parseObject(R& r)
{
	skipWS<R>(r);
	if (r.read() != '{') // begin-object
		THROW_PARSER_EXCEPTION;
	Value::Object object;

	while (true) {
		skipWS<R>(r);
		int c = r.peek();
		if (c == '}') { // end-object
			r.read();
			return Value(std::move(object));
		}
		if (c == ',') {
			if (object.empty())
				THROW_PARSER_EXCEPTION;
			r.read();
		}
		else if (!object.empty()) {
			THROW_PARSER_EXCEPTION;
		}

		Value name = parseString(r);

		skipWS<R>(r);
		if (r.read() != ':')
			THROW_PARSER_EXCEPTION;

		Value value = parseValue(r);

		object[move(name.asString())] = std::move(value);
	}
	THROW_PARSER_EXCEPTION;
}

// Stringifier
template<class W> void stringify4HexDigit(W& w, char c)
{
	char buf[6] = { '\\', 'u', '0', '0', '0', '0' };
	char h1 = c & 0xF; c >>= 4;
	char h0 = c & 0xF;
	if (h0 < 10) {
		buf[4] = '0' + h0;
	}
	else {
		buf[4] = 'A' + h0 - 10;
	}
	if (h1 < 10) {
		buf[5] = '0' + h1;
	}
	else {
		buf[5] = 'A' + h1 - 10;
	}
	w.write(buf, sizeof(buf));
}

template<class W> void stringifyValue(W& w, const Value& in);
template<class W> void stringifyInt(W& w, Value::Int in);
template<class W> void stringifyDouble(W& w, Value::Double in);
template<class W> void stringifyString(W& w, const Value::String& in);
template<class W> void stringifyArray(W& w, const Value::Array& in);
template<class W> void stringifyObject(W& w, const Value::Object& in);

template<class W> void stringifyValue(W& w, const Value& in)
{
	switch (in.type()) {
	case Value::Type::kNull:
		w.write("null", 4);
		break;
	case Value::Type::kBool:
		if (in.asBool())
			w.write("true", 4);
		else
			w.write("false", 5);
		break;
	case Value::Type::kInt:
		stringifyInt(w, in.asInt());
		break;
	case Value::Type::kDouble:
		stringifyDouble(w, in.asDouble());
		break;
	case Value::Type::kString:
		stringifyString(w, in.asString());
		break;
	case Value::Type::kArray:
		stringifyArray(w, in.asArray());
		break;
	case Value::Type::kObject:
		stringifyObject(w, in.asObject());
		break;
	}
}

template<class W> void stringifyInt(W& w, Value::Int in)
{
	char buf[64];
	int n = snprintf(buf, sizeof(buf), "%d", in);
	return w.write(buf, n);
}

template<class W> void stringifyDouble(W& w, Value::Double in)
{
	char buf[64];
	int n = snprintf(buf, sizeof(buf), "%g", in);
	return w.write(buf, n);
}

template<class W> void stringifyString(W& w, const Value::String& in)
{
	w.write('\"');
	const char* str = in.data();
	size_t len = in.length();
	for (size_t i = 0; i < len; i++) {
		char c = *str++;
		if (c == 0x22) {
			w.write("\\\"", 2);
		}
		else if (c == 0x5C) {
			w.write("\\\\", 2);
		}
		else if (c == 0x2F) {
			w.write("\\/", 2);
		}
		else if (c == 0x08) {
			w.write("\\b", 2);
		}
		else if (c == 0x0C) {
			w.write("\\f", 2);
		}
		else if (c == 0x0A) {
			w.write("\\n", 2);
		}
		else if (c == 0x0D) {
			w.write("\\r", 2);
		}
		else if (c == 0x09) {
			w.write("\\t", 2);
		}
		else if (c >= 0 && c < 0x20) { // control character
			stringify4HexDigit(w, c);
		}
		else {
			w.write(c);
		}
	}
	w.write('\"');
}

template<class W> void stringifyArray(W& w, const Value::Array& in)
{
	w.write('[');
	bool first = true;
	for (const auto &value : in) {
		if (!first) {
			w.write(',');
		}
		stringifyValue(w, value);
		first = false;
	}
	w.write(']');
}

template<class W> void stringifyObject(W& w, const Value::Object& in)
{
	w.write('{');
	bool first = true;
	for (const auto &pair : in) {
		if (!first) {
			w.write(',');
		}
		stringifyString(w, pair.first);
		w.write(':');
		stringifyValue(w, pair.second);
		first = false;
	}
	w.write('}');
}

// Reader
class StringReader {
public:
	enum {
		kEOS = -1
	};

	StringReader(const std::string str)
	{
		m_data = str.data();
		m_cur = m_data;
		m_end = m_cur + str.length();
	}

	StringReader(const char* data, size_t size)
	{
		m_data = data;
		m_cur = m_data;
		m_end = m_cur + size;
	}

	int pos()
	{
		return (int)(m_cur - m_data);
	}

	bool isEOS() {
		return !(m_cur < m_end);
	}

	int peek()
	{
		if (isEOS())
			return kEOS;
		return *m_cur;
	}

	int read()
	{
		if (isEOS())
			return kEOS;
		return *m_cur++;
	}

	bool tryReadUnescapedString(const char*& ptr, size_t& size)
	{
		const char* begin;
		const char* cur = m_cur;
		if (*cur++ != '\"')
			return false;

		begin = cur;
		while (cur < m_end) {
			char c = *cur++;
			if (c < 0x20) // Control character
				break;
			if (c & 0x80) // Not ASCII
				break;
			if (c == '\\') // Escape character
				break;

			if (c == '\"') {
				ptr = begin;
				m_cur = cur;
				size = cur - begin - 1;
				return true;
			}
		}

		return false;
	}

private:
	const char* m_data;
	const char* m_cur;
	const char* m_end;
};

class StringWriter {
public:
	StringWriter(std::string& str) : m_str(str) { }
	void write(char c) { m_str.push_back(c); }
	void write(const char* s, size_t n) { m_str.append(s, n); }
private:
	std::string& m_str;
};

}

Value parseJson(const char* data, size_t size)
{
	StringReader reader(data, size);
	try {
		return parseValue(reader);
	}
	catch (ParserException pe) {
		(void)pe;
		return Value();
	}
}

Value parseJson(const std::string& str)
{
	return parseJson(str.data(), str.length());
}

std::string stringify(const Value& value)
{
	std::string str;
	StringWriter writer(str);
	stringifyValue(writer, value);
	return str;
}

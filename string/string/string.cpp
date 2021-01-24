#include <cstdlib>
#include <stdexcept>
#include <cstring>
#include "string.hpp"

const char null_terminator = '\0';

void String::Empty_string_create () 
{
	_data = new char;
	*_data = null_terminator;
	_capacity = 1;
	_size = 0;
	new_ref_count();
}

void String::new_ref_count() 
{
	ref_count = new size_t;
	*ref_count = 1;
}

String::String()
{
	Empty_string_create();
}

String::String(const char* str)
{
	if (!str) {
		Empty_string_create();
	}
	else {
		_size = strlen(str);
		_capacity = _size + 1;
		_data = new char[_capacity];
		memcpy_s(_data, _capacity, str, _capacity);
		new_ref_count();
	}
}

String::String(const char* str, size_t n) 
{
	if (!str) {
		Empty_string_create();
	}
	else {
		size_t str_size = strlen(str);
		if (n >= str_size){
			n = str_size;
		}
		_capacity = n + 1;
		_size = n;
		_data = new char[_capacity];
		memcpy_s(_data, _capacity, str, _size);
		_data[_size] = null_terminator;
		new_ref_count();
	}
}

String::String(size_t n, char c)
{
	_size = n;
	_capacity = n + 1;
	_data = new char[_capacity];
	for (size_t i = 0; i < _capacity - 1; ++i) {
		_data[i] = c;
	}
	_data[_capacity - 1] = null_terminator;
	new_ref_count();
}

String::String(const String& str)
{
	_data = str._data;
	_capacity = str._capacity;
	_size = str._size;
	ref_count = str.ref_count;
	++(*ref_count);
}

String::String(const String& str, size_t pos, size_t len) 
{
	if (pos > str._size) {
		throw std::out_of_range("Out of range\n");
	}
	if (len == npos || len + pos > str._size) {
		len = str._size - pos;
	}
	_capacity = len + 1;
	_size = len;
	_data = new char[_capacity];
	memcpy_s(_data, _capacity, str._data + pos, len);
	_data[_capacity - 1] = null_terminator;
	ref_count = new size_t;
	*ref_count = 1;
}

void String::decrement()
{
	if (*ref_count == 1) {
		delete[] _data;
		_data = nullptr;
		delete ref_count;
		ref_count = nullptr;
	}
	else {
		(*ref_count)--;
	}
}

void String::CloneIfNeeded()
{
	if (*ref_count > 1) {
		(*ref_count)--;
		ref_count = new size_t;
		*ref_count = 1;
		_capacity = _size + 1;
		char* new_data = _data;
		_data = new char[_capacity];
		strncpy_s(_data, _capacity, new_data, _size);
	}
}

String::~String() 
{
	decrement();
}

size_t String::size() const
{
	return _size;
}

size_t String::capacity() const
{
	return _capacity;
}

void String::reserve(size_t n)
{
	if (n > _capacity) {
		CloneIfNeeded();
		_capacity = n;
		char* new_data = new char[n];
		memcpy_s(new_data, _size + 1, _data, _size + 1);
		delete[] _data;
		_data = new_data;
	}
}

void String::clear() 
{
	if (*_data == null_terminator) {
		return;
	}
	CloneIfNeeded();
	*_data = null_terminator;
	_size = 0;
}

bool String::empty() const
{
	return _size == 0;
}

char& String::at(size_t pos) 
{
	CloneIfNeeded();
	if (pos >= _size || !_data) {
		throw std::out_of_range("Out of range\n");
	}
	return _data[pos];
}

const char& String::at(size_t pos) const 
{
	if (pos > _size || !_data) {
		throw std::out_of_range("Out of range\n");
	}
	if (pos == _size) {
		return null_terminator;
	}
	else {
		return _data[pos];
	}
}

char& String::operator[](size_t pos)
{
	CloneIfNeeded();
	if (pos >= _size || !_data) {
		throw std::out_of_range("Out of range\n");
	}
	return _data[pos];
}

const char& String::operator[](size_t pos) const 
{
	if (pos > _size || !_data) {
		throw std::out_of_range("Out of range\n");
	}
	if (pos == _size) {
		return null_terminator;
	}
	else {
		return _data[pos];
	}
}

char& String::back() 
{
	CloneIfNeeded();
	if (!_size || !_data) {
		throw std::out_of_range("Undefined behaviour\n");
	}
	return _data[_size - 1];
}

const char& String::back() const 
{
	if (!_size || !_data) {
		throw std::out_of_range("Undefined behaviour\n");
	}
	return _data[_size - 1];
}

char& String::front() 
{
	CloneIfNeeded();
	if (!_size || !_data) {
		throw std::out_of_range("Undefined behaviour\n");
	}
	return _data[0];
}

const char& String::front() const
{
	if (!_size || !_data) {
		throw std::out_of_range("Undefined behaviour\n");
	}
	return _data[0];
}

String& String::operator+=(const String& str)
{
	if (!str._data || !str._size) {
		return *this;
	}
	CloneIfNeeded();
	if (_capacity > str._size + _size) {
		memcpy_s(_data + _size, _capacity - _size, str._data, str._size + 1);
		return *this;
	}
	_capacity = str._size + _size + 1;
	char* new_data = new char[_capacity];
	memcpy_s(new_data, _capacity, _data, _size);
	memcpy_s(new_data + _size, _capacity, str._data, str._size + 1);
	delete[] _data;
	_size = str._size + _size;
	_data = new_data;
	return *this;
}

String& String::operator+=(const char* str)
{
	String new_str = String(str);
	operator+=(new_str);
	return *this;
}

String& String::operator+=(char c)
{
	String new_str = String(1, c);
	operator+=(new_str);
	return *this;
}

String& String::operator=(const String& str)
{
	if (this == &str || ref_count == str.ref_count) {
		return *this;
	}
	decrement();
	_data = str._data;
	_capacity = str._capacity;
	_size = str._size;
	ref_count = str.ref_count;
	++(*ref_count);
	return *this;
}

String& String::operator=(const char* str)
{
	String new_str = String(str);
	operator=(new_str);
	return *this;
}

String& String::insert(size_t pos, const String& str)
{
	if (pos > _size) {
		throw std::out_of_range("Out of range\n");
	}
	if (!str._size) {
		return *this;
	}
	CloneIfNeeded();
	if (_capacity > _size + str._size) {
		memcpy_s(_data + pos + str._size, _capacity - pos - str._size, _data + pos, _size - pos + 1);
		memcpy_s(_data + pos, _capacity - pos, str._data, str._size);
	}
	else {
		_capacity = _size + str._size + 1;
		char* new_data = new char[_capacity];
		memcpy_s(new_data, _capacity, _data, pos);
		memcpy_s(new_data + pos, _capacity - pos, str._data, str._size);
		memcpy_s(new_data + pos + str._size, _capacity - pos - str._size, _data + pos, _size - pos + 1);
		delete[] _data;
		_data = new_data;
	}
	_size = _size + str._size;
	return *this;
}

String& String::insert(size_t pos, const char* str)
{
	String new_str = String(str);
	insert(pos, new_str);
	return *this;
}

String& String::insert(size_t pos, size_t n, char c)
{ 
	String new_str = String(n, c);
	insert(pos, new_str);
	return *this;
}

String& String::erase(size_t pos, size_t len)
{
	if (pos > _size) {
		throw std::out_of_range("Out of range\n");
	}
	CloneIfNeeded();
	if (pos + len > _size || len == npos || len > _size) {
		len = _size - pos;
		_data[pos] = null_terminator;
	}
	else {
		memcpy_s(_data + pos, _capacity, _data + pos + len, _capacity - len - pos);
	}
	_size = _size - len;
	return *this;
}

String& String::replace(size_t pos, size_t len, const String& str)
{
	erase(pos, len);
	insert(pos, str);
	return *this;
}

String& String::replace(size_t pos, size_t len, const char* str)
{
	String new_str = String(str);
	replace(pos, len, new_str);
	return *this;
}

String& String::replace(size_t pos, size_t len, size_t n, char c)
{
	String new_str = String(n, c);
	replace(pos, len, new_str);
	return *this;
}

void String::swap(String& str)
{
	char* tmp_data = str._data;
	str._data = _data;
	_data = tmp_data;

	size_t tmp_size = str._size;
	str._size = _size;
	_size = tmp_size;

	size_t tmp_capacity = str._capacity;
	str._capacity = _capacity;
	_capacity = tmp_capacity;

	size_t* tmp_ref = str.ref_count;
	str.ref_count = ref_count;
	ref_count = tmp_ref;
}

const char* String::data() const
{ 
	return _data;
}

size_t String::find(const String& str, size_t pos) const
{ 
	char* haystack = _data;
	const char* needle = str._data;
	if (pos >= _size || !_size || !str._size) {
		return npos;
	}
	for (size_t i = pos; haystack[i] != null_terminator;) {
		size_t j = 0;
		for (;haystack[i + j] != null_terminator && needle[j] != null_terminator; ++j) {
			if (haystack[i + j] != needle[j]) {
				break;
			}
		}
		if (j == str._size) {
			return i;
		}
		else {
			i = i + str._size - 1;
		}
	}
	return npos;
}

size_t String::find(const char* str, size_t pos) const
{ 
	String new_str = String(str);
	return find(new_str, pos);
}

size_t String::find(char c, size_t pos) const
{ 
	char* haystack = _data;
	if (pos >= _size) {
		return npos;
	}
	for (size_t i = pos; haystack[i] != null_terminator; ++i) {
		if (haystack[i] == c) {
			return i;
		}
	}
	return npos;
}

String String::substr(size_t pos, size_t len) const
{
	if (pos > _size) {
		throw std::out_of_range("Out of range\n");
	}
	if (len == npos || len + pos > _size) {
		len = _size - pos;
	}
	char* new_data = new char[len + 1];
	memcpy_s(new_data, len + 1, _data + pos, len);
	new_data[len] = null_terminator;
	String new_string = String(new_data);
	delete[] new_data;
	return new_string;
}

int String::compare(const String& str) const
{ 
	if (_size > str._size) {
		return 1;
	}
	if (_size < str._size) {
		return -1;
	}
	for (size_t i = 0; i < _size; ++i) {
		if (_data[i] > str._data[i]) {
			return 1;
		}
		if (_data[i] < str._data[i]) {
			return -1;
		}
	}
	return 0;
}

int String::compare(const char* str) const
{ 
	String new_string = String::String(str);
	int cmp = String::compare(new_string);
	return cmp;
}

size_t String::countRef() const
{
	return *ref_count;
}
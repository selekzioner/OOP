#pragma once
#include <string>

class type {
public:
	type() = delete;
	explicit type(const int& _x, const std::string& _s){
		x = _x;
		s = _s;
	}
	int x;
	bool operator< (const type& t) const {
		return s.length() < t.s.length();
	}
	static bool cmp_first_symbol(const type& t1, const type& t2){
		return t1.s[0] < t2.s[0];
	}
private:
	std::string s;
};
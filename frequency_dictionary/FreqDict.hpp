#pragma once
#include <string>

class FreqDict {
public:
	virtual size_t AddWord(const std::string& word) = 0;
	virtual size_t WordFreq(const std::string& word) const = 0;
	virtual ~FreqDict() = default;
};
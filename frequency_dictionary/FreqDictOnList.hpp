#pragma once
#include <list>
#include "FreqDict.hpp"

class FreqDictOnList final : public FreqDict {
public:
	size_t AddWord(const std::string& word) override;
	size_t WordFreq(const std::string& word) const override;
	~FreqDictOnList() override = default;
private:
	std::list<std::pair<std::string, size_t>> data;
};

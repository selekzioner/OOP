#pragma once
#include <vector>
#include "FreqDict.hpp"

class FreqDictOnVector final : public FreqDict {
public:
	size_t AddWord(const std::string& word) override;
	size_t WordFreq(const std::string& word) const override;
	~FreqDictOnVector() override = default;
private:
	std::vector<std::pair<std::string, size_t>> data;
};
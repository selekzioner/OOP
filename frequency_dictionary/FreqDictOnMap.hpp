#pragma once
#include <map>
#include "FreqDict.hpp"

class FreqDictOnMap final : public FreqDict {
public:
	size_t AddWord(const std::string& word) override;
	size_t WordFreq(const std::string& word) const override;
	~FreqDictOnMap() override = default;
private:
	std::map<std::string, size_t> data;
};
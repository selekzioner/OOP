#include "FreqDictOnMap.hpp"

size_t FreqDictOnMap::AddWord(const std::string& word)
{
	if (word.empty()) {
		return 0;
	}
	return ++data[word];
}

size_t FreqDictOnMap::WordFreq(const std::string& word) const
{
	if (word.empty()) {
		return 0;
	}
	return data.at(word);
}
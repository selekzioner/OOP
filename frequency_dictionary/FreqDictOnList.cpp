#include "FreqDictOnList.hpp"

size_t FreqDictOnList::AddWord(const std::string& word)
{
	if (word.empty()) {
		return 0;
	}
	size_t count = 0;
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (word == it->first) {
			count = ++it->second;
			break;
		}
	}
	if (!count) {
		data.push_back(std::make_pair(word, ++count));
	}
	return count;
}

size_t FreqDictOnList::WordFreq(const std::string& word) const 
{
	if (word.empty()) {
		return 0;
	}
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (word == it->first) {
			return it->second;
		}
	}
	return 0;
}
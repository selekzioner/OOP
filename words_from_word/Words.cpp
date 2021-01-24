#include <iostream>
#include "Words.hpp"

WordsFinder::WordsFinder(std::istream &file, std::string str) 
{
	std::string s;
	while (file >> s) {
		Dictionary.push_back(s);
	}
	BasicWord = str;
}

void WordsFinder::Output(std::ostream &of) const
{
	of << "Finded words:" << std::endl;
	for (auto it : Dictionary) {
		if (FindWords(BasicWord, it)) {
			of << it << std::endl;
		}
	}
}

bool WordsFinder::FindWords(std::string BasicWord, std::string SearchWord) const
{
	if (BasicWord.size() < SearchWord.size()) {
		return false;
	}
	size_t count = 0;
	for (size_t i = 0; i < SearchWord.size(); i++){
		for (size_t j = 0; j < BasicWord.size(); j++){
			if (BasicWord[j] == SearchWord[i]){
				count++;
				BasicWord[j] = ' ';
				break;
			}
		}
	}
	return (size_t)count == SearchWord.size();
}
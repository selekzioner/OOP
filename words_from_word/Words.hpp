#pragma once
#include <fstream>
#include <vector>
#include <string>

class WordsFinder{
public:
	WordsFinder(std::istream &file, std::string str);
	void Output(std::ostream &of) const;
private:
	std::string BasicWord;
	std::vector<std::string> Dictionary;
	bool FindWords(std::string main, std::string find) const;
};
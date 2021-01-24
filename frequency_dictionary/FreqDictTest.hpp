#pragma once
#include <string>
#include <iostream>
#include <list>
#include <time.h>
#include "FreqDict.hpp"

namespace FreqDictTest {
	std::string GetWordFromString(size_t& pos, const std::string& s);

	void PrintWordFrequences(std::ostream& of, const FreqDict& dictionary,
		const std::list<std::string>& ListOfWords, clock_t time);

	void Test(std::istream& InpStream, std::ostream& OutStream, FreqDict& Dictionary);
}
#include "FreqDictTest.hpp"

std::string FreqDictTest::GetWordFromString(size_t& pos, const std::string& s)
{
	const char* forbidden{ " ,.;:&!?\t" };
	pos = s.find_first_not_of(forbidden, pos);
	if (pos == std::string::npos) {
		return std::string();
	}

	size_t NextPos = s.find_first_of(forbidden, pos);
	size_t len = (NextPos == std::string::npos) ? std::string::npos : NextPos - pos;
	std::string word = s.substr(pos, len);

	word.front() = std::tolower(word.front());
	pos = NextPos;

	return word;
}

void FreqDictTest::PrintWordFrequences(std::ostream& OutStream, const FreqDict& Dictionary,
	const std::list<std::string>& ListOfWords, clock_t time)
{
	OutStream << std::endl << "------------------------------------\n" << std::endl;
	for (auto it = ListOfWords.begin(); it != ListOfWords.end(); ++it) {
		clock_t start = clock();
		size_t freq = Dictionary.WordFreq(*it);
		clock_t end = clock();
		OutStream << *it << " " << freq << std::endl;
		time += (end - start);
	}
	OutStream.std::ostream::seekp(0);
	OutStream << "time: " << time * 1.0 / CLOCKS_PER_SEC << " sec" << std::endl;
}

void FreqDictTest::Test(std::istream& InpStream, std::ostream& OutStream, FreqDict& Dictionary)
{
	clock_t time = 0;
	std::string str;
	std::list<std::string> ListOfWords;

	while (InpStream >> str) {
		size_t pos = 0;
		while (pos != std::string::npos) {
			std::string word = FreqDictTest::GetWordFromString(pos, str);
			clock_t start = clock();
			if (Dictionary.AddWord(word) == 1) {
				clock_t end = clock();
				time += (end - start);
				ListOfWords.push_back(word);
			}
		}
	}
	FreqDictTest::PrintWordFrequences(OutStream, Dictionary, ListOfWords, time);
}

#include <fstream>
#include <iostream>
#include "Words.hpp"

int main(int argc, char* argv[]) 
{
	const char* InputFileName;
	if (argc > 1) {
		InputFileName = argv[1];
	}
	else {
		InputFileName = "input.txt";
	}
	std::ifstream inp(InputFileName);
	if (!inp) {
		std::cout << "File not found!\n" << std::endl;
		return 1;
	}
	std::string word;
	std::cout << "Enter the word:" << std::endl;
	std::cin >> word;
	WordsFinder Finder = WordsFinder(inp, word);
	inp.close();
	if (argc > 2) {
		std::ofstream of;
		of.open(argv[2], std::ofstream::out);
		if (!of) {
			std::cout << "Cannot open file for writing!" << std::endl;
			return 1;
		}
		Finder.Output(of);
		of.close();
	}
	else {
		Finder.Output(std::cout);
	}
	return 0;
}
#include <iostream>
#include <sstream>
#include <fstream>
#include "CommandParser.hpp"
#include "Editor.hpp"

int main() {
	{
		std::ifstream in("input.txt");
		std::ifstream commands("commands.txt");
		auto doc = Document(in);
		auto editor = Editor(doc);
		CommandParser::parse(editor, commands);
	}
	std::cout << _CrtDumpMemoryLeaks();
	return 0;
}
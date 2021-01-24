#pragma once
#include <regex>
#include <sstream>
#include "Editor.hpp"


class CommandParser final {
public:
	CommandParser() = delete;
	static void parse(Editor& editor, std::istream& input);
private:
	static inline const std::regex check_commands = std::regex(
		"copy [0-9]+, [0-9]+|paste [0-9]+|insert \"[^ \f\n\r\t\v]+\", [0-9]+|delete [0-9]+, [0-9]+|undo|redo");
};

inline void CommandParser::parse(Editor& editor, std::istream& input) {
	for (std::string buffer; std::getline(input, buffer);) {
		if (!std::regex_match(buffer, check_commands)) {
			throw std::invalid_argument("Invalid Command File");
		}
		std::istringstream stream(buffer);
		std::string s_buffer;
		stream >> buffer;
		
		if (buffer == "copy") {
			stream >> buffer; stream >> s_buffer;
			buffer.erase(buffer.size() - 1, 1);
			editor.Copy(std::stoul(buffer), std::stoul(s_buffer));
		}
		else if (buffer == "paste") {
			stream >> buffer;
			editor.Paste(std::stoul(buffer));
		}
		else if (buffer == "insert") {
			stream >> buffer; stream >> s_buffer;
			buffer.erase(0, 1); buffer.erase(buffer.size() - 2, 2);
			editor.Insert(std::move(buffer), std::stoul(s_buffer));
		}
		else if (buffer == "delete") {
			stream >> buffer; stream >> s_buffer;
			buffer.erase(buffer.size() - 1, 1);
			editor.Delete(std::stoul(buffer), std::stoul(s_buffer));
		}
		else if (buffer == "undo") {
			editor.Undo();
		}
		else if (buffer == "redo") {
			editor.Redo();
		}
	}
}

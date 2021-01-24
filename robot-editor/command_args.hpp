#pragma once
#include <string>

struct command_args {
	std::string recipe_file_name;
	std::string input_file_name;
	std::string output_file_name;
	
	static command_args parse_command_args(const int argc, char* argv[]);
};
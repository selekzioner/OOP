#include <iostream>
#include <fstream>
#include "command_args.hpp"
#include "text_editor.hpp"
#include "recipe_parser.hpp"

int main(int argc, char* argv[])
{
	{
		auto _command_args = command_args::parse_command_args(argc, argv);
		std::ifstream rf(_command_args.recipe_file_name);
		if (!rf) {
			std::cout << "Error: File not found/EmptyFile\n" << std::endl;
			return 1;
		}
		std::ifstream inp(_command_args.input_file_name);
		if (!inp) {
			std::cout << "Error: File not found/EmptyFile\n" << std::endl;
			return 1;
		}
		std::ofstream out(_command_args.output_file_name);
		if (!out) {
			std::cout << "Error: Cannot open file for writing!" << std::endl;
			return 1;
		}
		auto parser = recipe_parser(rf);
		auto editor = text_editor(inp);
		editor.edit(parser.get_commands());
		editor.save(out);
	}
	std::cout << _CrtDumpMemoryLeaks();
	return 0;
}
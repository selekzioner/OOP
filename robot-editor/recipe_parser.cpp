#include "recipe_parser.hpp"
#include "command.hpp"

recipe_parser::recipe_parser(std::istream& in)
{
	data_.clear();
	std::string s;
	while (std::getline(in, s)){
		auto c = command(s);
		data_.push_back(c);
	}
}

const std::vector<command>& recipe_parser::get_commands() const
{
	return data_;
}
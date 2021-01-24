#pragma once
#include "command.hpp"

class recipe_parser
{
public:
	recipe_parser() = delete;
	explicit recipe_parser(std::istream& in);
	
	const std::vector<command>& get_commands() const;
	
	~recipe_parser() = default;
private:
	std::vector<command> data_;
};


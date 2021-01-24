#include "command.hpp"

command::command(const std::string& s)
{
	numbers_.clear();
	strings_.clear();
	auto args = get_args(s);
	if (args.front() == "delete") {
		correct_delete_command(args);
	}
	else if (args.front() == "change") {
		correct_change_command(args);
	}
	else if (args.front() == "insert") {
		correct_insert_command(args);
	}
	else if (args.front() == "replace") {
		correct_replace_command(args);
	}
	else if (args.front() == "undo") {
		strings_.emplace_back("undo");
	}
}

std::vector<std::string> command::get_args(const std::string& s)
{
	std::vector<std::string> args;
	size_t command_begin = 0, command_end = 0;
	auto text_begin = s.find_first_of('"');
	while (command_begin != std::string::npos) {
		command_begin = s.find_first_not_of(' ', command_end);

		if (text_begin == command_begin && command_begin != std::string::npos) {
			auto text_end = s.find('\"', text_begin + 1);
			if (text_end == std::string::npos){
				break;
			}
			
			while (s[text_end - 1] == '\\') {
				const auto prev_pos = text_end;
				text_end = s.find('\"', text_end + 1);
				if (text_end == std::string::npos) {
					text_end = prev_pos;
					break;
				}
			}
			args.push_back(s.substr(text_begin + 1, text_end - text_begin - 1));
			command_begin = s.find_first_not_of(' ', text_end + 1);
			text_begin = s.find_first_of('"', text_end + 1);
		}

		if (command_begin == std::string::npos) {
			break;
		}
		command_end = s.find_first_of(' ', command_begin);
		if (command_end == std::string::npos) {
			command_end = s.size();
		}
		auto arg = s.substr(command_begin, command_end - command_begin);
		args.push_back(arg);
	}
	return args;
}

const std::string& command::get_type() const
{
	return strings_.front();
}

size_t command::get_from() const
{
	return numbers_.front();
}

size_t command::get_to() const
{
	return numbers_.back();
}

const std::string& command::get_text1() const
{
	return strings_[1];
}

const std::string& command::get_text2() const
{
	return strings_.back();
}

void command::correct_delete_command(const std::vector<std::string>& args)
{
	strings_.emplace_back("delete");
	if (args.size() == 5 && args[1] == "from" && args[3] == "to") {
		numbers_.push_back(std::stoul(args[2]));
		numbers_.push_back(std::stoul(args[4]));
	}
	else if (args.size() == 3 && args[1] == "from") {
		numbers_.push_back(std::stoul(args[2]));
		numbers_.push_back(n_pos);
	}
	else if (args.size() == 3 && args[1] == "to") {
		numbers_.push_back(0);
		numbers_.push_back(std::stoul(args[2]));
	}
	else if (args.size() == 1) {
		numbers_.push_back(0);
		numbers_.push_back(n_pos);
	}
}

void command::correct_change_command(const std::vector<std::string>& args)
{
	std::string text;
	strings_.emplace_back("change");
	if (args.size() == 7 && args[1] == "from" && args[3] == "to" && args[5] == "with") {
		numbers_.push_back(std::stoul(args[2]));
		numbers_.push_back(std::stoul(args[4]));
		text = args[6];
	}
	else if (args.size() == 5 && args[3] == "with") {
		auto m = std::stoul(args[2]);
		text = args[4];
		if (args[1] == "from") {
			numbers_.push_back(std::stoul(args[2]));
			numbers_.push_back(n_pos);
		}
		else if (args[1] == "to") {
			numbers_.push_back(0);
			numbers_.push_back(std::stoul(args[2]));
		}
	}
	else if (args.size() == 3 && args[1] == "with") {
		text = args[2];
		numbers_.push_back(0);
		numbers_.push_back(n_pos);
	}
	correct_exeption_symbols(text);
	strings_.push_back(text);
}

void command::correct_insert_command(const std::vector<std::string>& args)
{
	strings_.emplace_back("insert");
	if (args.size() == 4 && args[1] == "after") {
		numbers_.push_back(std::stoul(args[2]));
		auto text = args[3];
		correct_exeption_symbols(text);
		strings_.push_back(text);
	}
}

void command::correct_replace_command(const std::vector<std::string>& args)
{
	strings_.emplace_back("replace");
	std::string text, replacement;
	if (args.size() == 8 && args[1] == "from" && args[3] == "to" && args[6] == "with") {
		numbers_.push_back(std::stoul(args[2]));
		numbers_.push_back(std::stoul(args[4]));
		text = args[5];
		replacement = args[7];
	}
	else if (args.size() == 6 && args[4] == "with") {
		text = args[3];
		replacement = args[5];

		if (args[1] == "from") {
			numbers_.push_back(std::stoul(args[2]));
			numbers_.push_back(n_pos);
		}
		else if (args[1] == "to") {
			numbers_.push_back(0);
			numbers_.push_back(std::stoul(args[2]));
		}
	}
	else if (args.size() == 4 && args[2] == "with") {
		text = args[1];
		replacement = args[3];
		numbers_.push_back(0);
		numbers_.push_back(n_pos);
	}
	correct_exeption_symbols(text);
	strings_.push_back(text);
	correct_exeption_symbols(replacement);
	strings_.push_back(replacement);
}

void command::correct_exeption_symbols(std::string& str)
{
	replace_symbol(str, "\\n", "\n", 2);
	replace_symbol(str, "\\\"", "\"", 2);
	replace_symbol(str, "\\\\", "\\", 3);
}

void command::replace_symbol(std::string& str, const std::string& symbol, const std::string& replacement, const size_t& size)
{
	size_t pos = 0;
	while (pos != std::string::npos) {
		pos = str.find(symbol);
		if (pos == std::string::npos) {
			break;
		}
		str.replace(pos, size, replacement);
	}
}
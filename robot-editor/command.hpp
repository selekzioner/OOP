#pragma once
#include <string>
#include <vector>
#include <iostream>

class command {
public:
	command() = delete;
	explicit command(const std::string& s);

	size_t get_from() const;
	size_t get_to() const;
	const std::string& get_type() const;
	const std::string& get_text1() const;
	const std::string& get_text2() const;
	
	virtual ~command() = default;
private:
	static std::vector<std::string> get_args(const std::string& s);

	static const size_t n_pos = std::numeric_limits<size_t>::max();

	void correct_delete_command(const std::vector<std::string>& args);
	void correct_change_command(const std::vector<std::string>& args);
	void correct_insert_command(const std::vector<std::string>& args);
	void correct_replace_command(const std::vector<std::string>& args);

	static void correct_exeption_symbols(std::string& str);
	static void replace_symbol(std::string& str, 
		const std::string& symbol, const std::string& replacement, const size_t& size);
	
	std::vector<std::string> strings_;
	std::vector<size_t> numbers_;
};
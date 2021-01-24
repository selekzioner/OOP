#pragma once
#include <string>
#include <vector>
#include "command.hpp"

class text_editor
{
public:
	text_editor() = delete;
	explicit text_editor(std::istream& inp);

	void edit(const std::vector<command>& commands);
	void save(std::ostream& out);

	~text_editor() = default;
private:
	std::vector<std::string> data_;
	std::vector<std::string> copy_;

	std::vector<std::string> format_text(const std::string& text) const;

	void delete_(const size_t& n = 0, const size_t& m = n_pos);
	void change(const std::string& text, const size_t& n = 0, const size_t& m = n_pos);
	void insert(const std::string& text, const size_t& n);
	void replace(const std::string& text, const std::string& replacement, const size_t& n = 0, const size_t& m = n_pos);
	void undo();

	static const size_t n_pos = std::numeric_limits<size_t>::max();
	
	static size_t correct_begin_index(const size_t& index);
	size_t correct_end_index(const size_t& index) const;
};
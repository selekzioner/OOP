#include "text_editor.hpp"
#include <sstream>

text_editor::text_editor(std::istream& inp)
{
	std::string s;
	while (std::getline(inp, s)) {
		data_.push_back(s);
	}
}

void text_editor::save(std::ostream& out)
{
	for (auto i= data_.begin(); i != data_.end(); ++i)
	{
		out << *i;
		if (i + 1 != data_.end()){
			out << "\n";
		}
	}
}

std::vector<std::string> text_editor::format_text(const std::string& text) const
{
	std::string s;
	std::vector<std::string> vector;
	std::stringstream ss(text.data());
	while (std::getline(ss, s)){
		vector.push_back(s);
	}
	return vector;
}

void text_editor::edit(const std::vector<command>& commands)
{
	for (const auto& i : commands)
	{
		if (i.get_type() == "delete") {
			delete_(i.get_from(), i.get_to());
		}
		else if (i.get_type() == "change") {
			change(i.get_text1(), i.get_from(), i.get_to());
		}
		else if (i.get_type() == "insert") {
			insert(i.get_text1(), i.get_from());
		}
		else if (i.get_type() == "replace") {
			replace(i.get_text1(), i.get_text2(), i.get_from(), i.get_to());
		}
		else if (i.get_type() == "undo") {
			undo();
		}
	}
}

void text_editor::delete_(const size_t& n, const size_t& m)
{
	if (data_.empty()) {
		return;
	}
	copy_ = data_;
	const auto first = correct_begin_index(n);
	const auto last = correct_end_index(m);
	for (auto i = first; i <= last; ++i) {
		data_.erase(data_.begin() + first);
	}
}

void text_editor::change(const std::string& text, const size_t& n, const size_t& m)
{
	const auto prev_data = data_;
	delete_(n, m);
	if (!n) {
		insert(text, n);
	}
	else {
		insert(text, n - 1);
	}
	copy_ = prev_data;
}

void text_editor::insert(const std::string& text, const size_t& n)
{
	auto formatted_text = format_text(text);
	if (text.empty()) {
		return;
	}
	copy_ = data_;
	auto first = n;
	if (n > data_.size()) {
		first = data_.size();
	}
	data_.insert(data_.begin() + first, formatted_text.begin(), formatted_text.end());
}

void text_editor::replace(const std::string& text, const std::string& replacement, const size_t& n, const size_t& m)
{
	if (data_.empty()) {
		return;
	}
	const auto prev_data = data_;
	const auto first = correct_begin_index(n);
	const auto last = correct_end_index(m);

	std::string haystack;
	for (auto i = data_.begin() + first; i <= data_.begin() + last; ++i) {
		haystack += *i;
		if (i - data_.begin() != last) {
			haystack += "\n";
		}
	}
	size_t pos = 0, counter = 0;
	while (true) {
		pos = haystack.find(text, pos);
		if (pos == std::string::npos) {
			break;
		}
		haystack.replace(pos, text.size(), replacement);
		++counter;
	}
	if (!counter) {
		return;
	}
	change(haystack, first + 1, last + 1);
	copy_ = prev_data;
}

void text_editor::undo()
{
	if (copy_.empty()) {
		return;
	}
	data_ = copy_;
	copy_.clear();
}

size_t text_editor::correct_begin_index(const size_t& index)
{
	if (!index) {
		return index;
	}
	return index - 1;
}

size_t text_editor::correct_end_index(const size_t& index) const
{
	if (!index) {
		return index;
	}
	if (index > data_.size()) {
		return data_.size() - 1;
	}
	return index - 1;
}
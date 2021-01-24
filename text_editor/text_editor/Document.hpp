#pragma once
#include <string>
#include <iostream>


class Document final {
public:
	Document() = default;
	explicit Document(std::istream& input) {
		std::getline(input, str_);
	}

	Document(const Document& another) {
		str_ = another.str_;
	}

	void insert(const std::string& buffer, const size_t idx) {
		if (idx < str_.size()) {
			str_.insert(idx, buffer);
		}
		else {
			str_.insert(str_.size(), buffer);
		}
	}

	void erase(const size_t begin, const size_t end) {
		if (begin < str_.size() && end > begin) {
			str_.erase(begin, end - begin);
		}
	}

	std::string substr(const size_t begin, const size_t end) const {
		if (begin < str_.size()) {
			return str_.substr(begin, end - begin);
		}
		return std::string();
	}

	const std::string& data() const {
		return str_;
	}

	size_t size() const {
		return str_.size();
	}

	void save(std::ostream& out) const {
		out << str_;
	}
private:
	std::string str_{};
};

#pragma once
#include "Document.hpp"


class EditorCommand {
public:
	EditorCommand() = delete;
	explicit EditorCommand(Document& text) : text_(text) {
		text_ = text;
	}

	virtual void execute() = 0;
	virtual void undo() = 0;

	virtual ~EditorCommand() = default;
protected:
	Document& text_;
};


class CopyCommand final : public EditorCommand {
public:
	CopyCommand(Document& text, std::string& buffer, const size_t begin, const size_t end) :
		EditorCommand(text), buffer_(buffer) {
		begin_ = begin, end_ = end;
	}
	
	void execute() override {
		prev_buffer_ = buffer_;
		buffer_ = text_.substr(begin_, end_);
	}
	
	void undo() override {
		buffer_ = prev_buffer_;
	}
private:
	size_t begin_, end_;
	std::string& buffer_;
	std::string prev_buffer_{};
};


class PasteCommand final : public EditorCommand {
public:
	PasteCommand(Document& text, std::string& buffer, const size_t idx) :
		EditorCommand(text), buffer_(buffer) {
		idx_ = idx;
	}

	void execute() override {
		text_.insert(buffer_, idx_);
	}

	void undo() override {
		text_.erase(idx_, idx_ + buffer_.size());
	}
private:
	size_t idx_;
	std::string& buffer_;
};


class InsertCommand final : public EditorCommand {
public:
	InsertCommand(Document& text, std::string&& str, const size_t idx) :
		EditorCommand(text), str_(str) {
		idx_ = idx;
	}

	void execute() override {
		text_.insert(str_, idx_);
	}

	void undo() override {
		if (idx_ >= text_.size()) {
			idx_ = text_.size() - str_.size();
		}
		text_.erase(idx_, idx_ + str_.size());
	}
private:
	size_t idx_;
	const std::string str_;
};


class DeleteCommand final : public EditorCommand {
public:
	DeleteCommand(Document& text, const size_t begin, const size_t end) :
		EditorCommand(text) {
		begin_ = begin, end_ = end;
	}

	void execute() override {
		erased_ = text_.substr(begin_, end_);
		text_.erase(begin_, end_);
	}

	void undo() override {
		text_.insert(erased_, begin_);
	}
private:
	size_t begin_, end_;
	std::string erased_{};
};
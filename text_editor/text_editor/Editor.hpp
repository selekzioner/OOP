#pragma once
#include <vector>
#include "EditorCommands.hpp"
#include "Document.hpp"


class Editor final {
public:
	Editor() = delete;
	explicit Editor(Document& text) : text_(text) {};
	
	void Copy(const size_t begin, const size_t end) {
		AddAndExecuteCommand(std::make_unique<CopyCommand>(text_, buffer_, begin, end));
	}
	
	void Paste(const size_t idx) {
		AddAndExecuteCommand(std::make_unique<PasteCommand>(text_, buffer_, idx));
	}

	void Insert(std::string&& str, const size_t idx) {
		AddAndExecuteCommand(std::make_unique<InsertCommand>(text_, std::move(str), idx));
	}

	void Delete(const size_t begin, const size_t end) {
		AddAndExecuteCommand(std::make_unique<DeleteCommand>(text_, begin, end));
	}

	void Undo() {
		if (executed_commands_.empty()) {
			return;
		}
		executed_commands_.back()->undo();
		undo_commands_.emplace_back(std::move(executed_commands_.back()));
		executed_commands_.pop_back();
	}

	void Redo() {
		if (undo_commands_.empty()) {
			return;
		}
		undo_commands_.back()->execute();
		executed_commands_.emplace_back(std::move(undo_commands_.back()));
		undo_commands_.pop_back();
	}

	void Save(std::ostream& out) const {
		text_.save(out);
	}
	
private:
	std::vector<std::unique_ptr<EditorCommand>> executed_commands_, undo_commands_;
	Document& text_;
	std::string buffer_{};

	void AddAndExecuteCommand(std::unique_ptr<EditorCommand>&& cmd) {
		cmd->execute();
		executed_commands_.emplace_back(std::move(cmd));
		undo_commands_.clear();
	}
};
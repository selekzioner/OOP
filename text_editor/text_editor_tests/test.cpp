#include "pch.h"
#include "CommandParser.hpp"
#include <sstream>
#include <fstream>

TEST(CopyAndPaste, Empty) {
	std::istringstream input;
	auto doc = Document(input);
	auto editor = Editor(doc);
	editor.Copy(0, 10);
	EXPECT_TRUE(doc.data().empty());
	editor.Paste(5);
	EXPECT_TRUE(doc.data().empty());
}

TEST(CopyAndPaste, NonEmpty) {
	std::istringstream input("aaa bb c");
	auto doc = Document(input);
	auto editor = Editor(doc);
	
	editor.Copy(5, 10);
	EXPECT_EQ(doc.data(), "aaa bb c");
	editor.Paste(6);
	EXPECT_EQ(doc.data(), "aaa bbb c c");

	editor.Copy(20, 1);
	EXPECT_EQ(doc.data(), "aaa bbb c c");
	editor.Paste(6);
	EXPECT_EQ(doc.data(), "aaa bbb c c");
	
	editor.Copy(0, 1);
	EXPECT_EQ(doc.data(), "aaa bbb c c");
	editor.Paste(0);
	EXPECT_EQ(doc.data(), "aaaa bbb c c");

	editor.Copy(8, 12);
	EXPECT_EQ(doc.data(), "aaaa bbb c c");
	editor.Paste(20);
	EXPECT_EQ(doc.data(), "aaaa bbb c c c c");
}

TEST(CopyAndPaste, Undo) {
	std::istringstream input("aaa bb c");
	auto doc = Document(input);
	auto editor = Editor(doc);

	editor.Copy(5, 10);
	EXPECT_EQ(doc.data(), "aaa bb c");
	editor.Paste(6);
	EXPECT_EQ(doc.data(), "aaa bbb c c");
	editor.Undo();
	EXPECT_EQ(doc.data(), "aaa bb c");

	editor.Copy(0, 1);
	EXPECT_EQ(doc.data(), "aaa bb c");
	editor.Paste(0);
	EXPECT_EQ(doc.data(), "aaaa bb c");
	

	editor.Copy(5, 7);
	EXPECT_EQ(doc.data(), "aaaa bb c");
	editor.Paste(8);
	EXPECT_EQ(doc.data(), "aaaa bb bbc");
	editor.Undo();
	EXPECT_EQ(doc.data(), "aaaa bb c");
	editor.Undo();
	EXPECT_EQ(doc.data(), "aaaa bb c");
	editor.Undo();
	EXPECT_EQ(doc.data(), "aaa bb c");
}

TEST(CopyAndPaste, Redo) {
	std::istringstream input("aaa bb c");
	auto doc = Document(input);
	auto editor = Editor(doc);

	editor.Copy(5, 10);
	EXPECT_EQ(doc.data(), "aaa bb c");
	editor.Paste(6);
	EXPECT_EQ(doc.data(), "aaa bbb c c");
	editor.Undo();
	EXPECT_EQ(doc.data(), "aaa bb c");
	editor.Redo();
	EXPECT_EQ(doc.data(), "aaa bbb c c");
	editor.Undo();
	EXPECT_EQ(doc.data(), "aaa bb c");

	editor.Copy(0, 1);
	EXPECT_EQ(doc.data(), "aaa bb c");
	editor.Paste(0);
	EXPECT_EQ(doc.data(), "aaaa bb c");


	editor.Copy(5, 7);
	EXPECT_EQ(doc.data(), "aaaa bb c");
	editor.Paste(8);
	EXPECT_EQ(doc.data(), "aaaa bb bbc");
	
	editor.Undo();
	EXPECT_EQ(doc.data(), "aaaa bb c");
	editor.Undo();
	EXPECT_EQ(doc.data(), "aaaa bb c");
	editor.Undo();
	EXPECT_EQ(doc.data(), "aaa bb c");

	editor.Redo();
	EXPECT_EQ(doc.data(), "aaaa bb c");
	editor.Redo();
	EXPECT_EQ(doc.data(), "aaaa bb c");
	editor.Redo();
	EXPECT_EQ(doc.data(), "aaaa bb bbc");
	editor.Redo();
	EXPECT_EQ(doc.data(), "aaaa bb bbc");
}

TEST(Insert, Empty) {
	std::istringstream input;
	auto doc = Document(input);
	auto editor = Editor(doc);
	editor.Insert("", 0);
	EXPECT_TRUE(doc.data().empty());
	editor.Insert("abc", 0);
	EXPECT_EQ(doc.data(), "abc");
}

TEST(Insert, NonEmpty) {
	std::istringstream input("unique_ptr");
	auto doc = Document(input);
	auto editor = Editor(doc);

	editor.Insert("oin", 8);
	EXPECT_EQ(doc.data(), "unique_pointr");
	editor.Insert("e", 12);
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Insert("__", 0);
	EXPECT_EQ(doc.data(), "__unique_pointer");

	editor.Insert("_", 100);
	EXPECT_EQ(doc.data(), "__unique_pointer_");

	editor.Insert("", 100);
	EXPECT_EQ(doc.data(), "__unique_pointer_");
}

TEST(Insert, Undo) {
	std::istringstream input("unique_ptr");
	auto doc = Document(input);
	auto editor = Editor(doc);

	editor.Insert("oin", 8);
	EXPECT_EQ(doc.data(), "unique_pointr");
	editor.Insert("e", 12);
	EXPECT_EQ(doc.data(), "unique_pointer");
	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointr");

	editor.Insert("__", 0);
	EXPECT_EQ(doc.data(), "__unique_pointr");

	editor.Insert("_", 100);
	EXPECT_EQ(doc.data(), "__unique_pointr_");

	editor.Insert("", 100);
	EXPECT_EQ(doc.data(), "__unique_pointr_");

	editor.Undo();
	EXPECT_EQ(doc.data(), "__unique_pointr_");

	editor.Undo();
	EXPECT_EQ(doc.data(), "__unique_pointr");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointr");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_ptr");
}

TEST(Insert, Redo) {
	std::istringstream input("unique_ptr");
	auto doc = Document(input);
	auto editor = Editor(doc);

	editor.Insert("oin", 8);
	EXPECT_EQ(doc.data(), "unique_pointr");
	editor.Insert("e", 12);
	EXPECT_EQ(doc.data(), "unique_pointer");
	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointr");
	editor.Redo();
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Insert("__", 0);
	EXPECT_EQ(doc.data(), "__unique_pointer");

	editor.Insert("_", 100);
	EXPECT_EQ(doc.data(), "__unique_pointer_");

	editor.Insert("", 100);
	EXPECT_EQ(doc.data(), "__unique_pointer_");

	editor.Undo();
	EXPECT_EQ(doc.data(), "__unique_pointer_");

	editor.Undo();
	EXPECT_EQ(doc.data(), "__unique_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointr");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_ptr");

	editor.Redo();
	EXPECT_EQ(doc.data(), "unique_pointr");

	editor.Redo();
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Redo();
	EXPECT_EQ(doc.data(), "__unique_pointer");

	editor.Redo();
	EXPECT_EQ(doc.data(), "__unique_pointer_");
}

TEST(Delete, Empty) {
	std::istringstream input;
	auto doc = Document(input);
	auto editor = Editor(doc);
	editor.Delete(5, 10);
	EXPECT_TRUE(doc.data().empty());
}

TEST(Delete, NonEmpty) {
	std::istringstream input("unique_pointer");
	auto doc = Document(input);
	auto editor = Editor(doc);

	editor.Delete(12, 13);
	EXPECT_EQ(doc.data(), "unique_pointr");
	editor.Delete(8, 11);
	EXPECT_EQ(doc.data(), "unique_ptr");

	editor.Delete(20, 11);
	EXPECT_EQ(doc.data(), "unique_ptr");

	editor.Delete(7, 20);
	EXPECT_EQ(doc.data(), "unique_");

	editor.Delete(6, 7);
	EXPECT_EQ(doc.data(), "unique");

	editor.Delete(3, 3);
	EXPECT_EQ(doc.data(), "unique");

	editor.Delete(1, 0);
	EXPECT_EQ(doc.data(), "u");
}

TEST(Delete, Undo) {
	std::istringstream input("unique_pointer");
	auto doc = Document(input);
	auto editor = Editor(doc);

	editor.Delete(12, 13);
	EXPECT_EQ(doc.data(), "unique_pointr");
	editor.Delete(8, 11);
	EXPECT_EQ(doc.data(), "unique_ptr");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointr");
	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Delete(20, 11);
	EXPECT_EQ(doc.data(), "unique_pointer");
	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Delete(7, 20);
	EXPECT_EQ(doc.data(), "unique_");

	editor.Delete(6, 7);
	EXPECT_EQ(doc.data(), "unique");

	editor.Delete(3, 3);
	EXPECT_EQ(doc.data(), "unique");

	editor.Delete(1, 0);
	EXPECT_EQ(doc.data(), "u");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointer");
}

TEST(Delete, Redo) {
	std::istringstream input("unique_pointer");
	auto doc = Document(input);
	auto editor = Editor(doc);

	editor.Delete(12, 13);
	EXPECT_EQ(doc.data(), "unique_pointr");
	editor.Delete(8, 11);
	EXPECT_EQ(doc.data(), "unique_ptr");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointr");
	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Redo();
	EXPECT_EQ(doc.data(), "unique_pointr");
	editor.Redo();
	EXPECT_EQ(doc.data(), "unique_ptr");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointr");
	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Delete(20, 11);
	EXPECT_EQ(doc.data(), "unique_pointer");
	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Delete(7, 20);
	EXPECT_EQ(doc.data(), "unique_");

	editor.Delete(6, 7);
	EXPECT_EQ(doc.data(), "unique");

	editor.Delete(3, 3);
	EXPECT_EQ(doc.data(), "unique");

	editor.Delete(1, 0);
	EXPECT_EQ(doc.data(), "u");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Redo();
	EXPECT_EQ(doc.data(), "unique_");

	editor.Redo();
	EXPECT_EQ(doc.data(), "unique");

	editor.Redo();
	EXPECT_EQ(doc.data(), "unique");

	editor.Redo();
	EXPECT_EQ(doc.data(), "u");
}

TEST(Undo, SimpleTest) {
	std::istringstream input("unique_pointer");
	auto doc = Document(input);
	auto editor = Editor(doc);

	editor.Delete(0, 6);
	EXPECT_EQ(doc.data(), "_pointer");

	editor.Insert("shared", 0);
	EXPECT_EQ(doc.data(), "shared_pointer");

	editor.Copy(0, 20);
	EXPECT_EQ(doc.data(), "shared_pointer");

	editor.Paste(0);
	EXPECT_EQ(doc.data(), "shared_pointershared_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "shared_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "shared_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointer");
}

TEST(Redo, SimpleTest) {
	std::istringstream input("unique_pointer");
	auto doc = Document(input);
	auto editor = Editor(doc);

	editor.Delete(0, 6);
	EXPECT_EQ(doc.data(), "_pointer");

	editor.Insert("shared", 0);
	EXPECT_EQ(doc.data(), "shared_pointer");

	editor.Copy(0, 20);
	EXPECT_EQ(doc.data(), "shared_pointer");

	editor.Paste(0);
	EXPECT_EQ(doc.data(), "shared_pointershared_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "shared_pointer");

	editor.Redo();
	EXPECT_EQ(doc.data(), "shared_pointershared_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "shared_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "shared_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "_pointer");

	editor.Undo();
	EXPECT_EQ(doc.data(), "unique_pointer");

	editor.Redo();
	EXPECT_EQ(doc.data(), "_pointer");
	
	editor.Redo();
	EXPECT_EQ(doc.data(), "shared_pointer");
	
	editor.Redo();
	EXPECT_EQ(doc.data(), "shared_pointer");
	
	editor.Redo();
	EXPECT_EQ(doc.data(), "shared_pointershared_pointer");
	
	editor.Redo();
	EXPECT_EQ(doc.data(), "shared_pointershared_pointer");
}

TEST(Parser, CopyAndPaste) {
	std::istringstream input("abc def");
	std::istringstream commands("copy 1, 3\npaste 0");
	auto doc = Document(input);
	auto editor = Editor(doc);
	CommandParser::parse(editor, commands);
	EXPECT_EQ(doc.data(), "bcabc def");
}

TEST(Parser, Insert) {
	std::istringstream input("abc def");
	std::istringstream commands("insert \"gh\", 3");
	auto doc = Document(input);
	auto editor = Editor(doc);
	CommandParser::parse(editor, commands);
	EXPECT_EQ(doc.data(), "abcgh def");
}

TEST(Parser, Delete) {
	std::istringstream input("abc def");
	std::istringstream commands("delete 0, 4");
	auto doc = Document(input);
	auto editor = Editor(doc);
	CommandParser::parse(editor, commands);
	EXPECT_EQ(doc.data(), "def");
}

TEST(Parser, Undo) {
	std::istringstream input("abc def");
	std::istringstream del("delete 0, 4");
	auto doc = Document(input);
	auto editor = Editor(doc);
	CommandParser::parse(editor, del);
	EXPECT_EQ(doc.data(), "def");

	std::istringstream un("undo");
	CommandParser::parse(editor, un);
	EXPECT_EQ(doc.data(), "abc def");

	std::istringstream commands("delete 0, 4\nundo");
	CommandParser::parse(editor, commands);
	EXPECT_EQ(doc.data(), "abc def");
}

TEST(Parser, Redo) {
	std::istringstream input("abc def");
	std::istringstream del("delete 0, 4");
	auto doc = Document(input);
	auto editor = Editor(doc);
	CommandParser::parse(editor, del);
	EXPECT_EQ(doc.data(), "def");

	std::istringstream un("undo");
	CommandParser::parse(editor, un);
	EXPECT_EQ(doc.data(), "abc def");

	std::istringstream re("redo");
	CommandParser::parse(editor, re);
	EXPECT_EQ(doc.data(), "def");

	std::istringstream un1("undo");
	CommandParser::parse(editor, un1);
	EXPECT_EQ(doc.data(), "abc def");

	std::istringstream commands("delete 0, 4\nundo\nredo");
	CommandParser::parse(editor, commands);
	EXPECT_EQ(doc.data(), "def");
}

TEST(Parser, InvalidCommands) {
	std::istringstream input("abc def");
	std::istringstream del("delete abc");
	auto doc = Document(input);
	auto editor = Editor(doc);
	EXPECT_THROW(CommandParser::parse(editor, del), std::invalid_argument);

	std::istringstream ins("insert hello, 3");
	EXPECT_THROW(CommandParser::parse(editor, ins), std::invalid_argument);

	std::istringstream rand("abc");
	EXPECT_THROW(CommandParser::parse(editor, rand), std::invalid_argument);

	std::istringstream cpy("copy 1 3");
	EXPECT_THROW(CommandParser::parse(editor, cpy), std::invalid_argument);

	std::istringstream pst("paste 1, 3");
	EXPECT_THROW(CommandParser::parse(editor, pst), std::invalid_argument);
}

TEST(Doc, Save) {
	std::istringstream input("abc def");
	std::istringstream commands("delete 0, 4\nundo\nredo");
	std::ostringstream output1;
	auto doc = Document(input);
	
	auto editor = Editor(doc);
	editor.Save(output1);
	EXPECT_EQ(output1.str(), "abc def");
	
	CommandParser::parse(editor, commands);
	std::ostringstream output2;
	EXPECT_EQ(doc.data(), "def");
	editor.Save(output2);
	EXPECT_EQ(output2.str(), "def");
}
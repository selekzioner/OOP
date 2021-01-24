#include "pch.h"
#include "../2337_String/string.hpp"

TEST(Ñonstructor, Empty)
{
	String str = String();
	ASSERT_STRCASEEQ("\0", str.data());
	ASSERT_EQ(0, str.size());
	ASSERT_EQ(1, str.capacity());
	ASSERT_EQ(1, str.countRef());
	ASSERT_EQ(true, str.empty());
}

TEST(Ñonstructor, CharArrayEmpty)
{
	String str = String(nullptr);
	ASSERT_STRCASEEQ("\0", str.data());
	ASSERT_EQ(0, str.size());
	ASSERT_EQ(1, str.capacity());
	ASSERT_EQ(1, str.countRef());
	ASSERT_EQ(true, str.empty());

	String _str = String(nullptr, 0);
	EXPECT_STRCASEEQ("\0", _str.data());
	EXPECT_EQ(0, _str.size());
	EXPECT_EQ(1, _str.capacity());
	EXPECT_EQ(1, _str.countRef());
	EXPECT_EQ(true, _str.empty());
}

TEST(Ñonstructor, CharArray)
{
	String str_1 = String("Test_1");
	EXPECT_STRCASEEQ("Test_1", str_1.data());
	EXPECT_EQ(6, str_1.size());
	EXPECT_EQ(7, str_1.capacity());
	EXPECT_EQ(1, str_1.countRef());
	EXPECT_EQ(false, str_1.empty());

	String str_2 = String("Test_1", 4);
	EXPECT_STRCASEEQ("Test", str_2.data());
	EXPECT_EQ(4, str_2.size());
	EXPECT_EQ(5, str_2.capacity());
	EXPECT_EQ(1, str_2.countRef());
	EXPECT_EQ(false, str_2.empty());
}

TEST(Ñonstructor, Char)
{
	String str_1 = String(5, 'a');
	EXPECT_STRCASEEQ("aaaaa", str_1.data());
	EXPECT_EQ(5, str_1.size());
	EXPECT_EQ(6, str_1.capacity());
	EXPECT_EQ(1, str_1.countRef());
	EXPECT_EQ(false, str_1.empty());

	String str_2 = String(5, '\0');
	EXPECT_STRCASEEQ("\0", str_2.data());
	EXPECT_EQ(5, str_2.size());
	EXPECT_EQ(6, str_2.capacity());
	EXPECT_EQ(1, str_2.countRef());
	EXPECT_EQ(false, str_2.empty());
}

TEST(Ñonstructor, CopyEmpty)
{
	String str = String();
	String str_1 = String(str);
	ASSERT_STRCASEEQ("\0", str_1.data());
	ASSERT_EQ(0, str_1.size());
	ASSERT_EQ(1, str_1.capacity());
	ASSERT_EQ(2, str_1.countRef());
	ASSERT_EQ(true, str_1.empty());
}

TEST(Ñonstructor, Copy)
{
	String _str = String("abcd");
	String str_2 = String(_str);
	EXPECT_STRCASEEQ("abcd", str_2.data());
	EXPECT_EQ(4, str_2.size());
	EXPECT_EQ(5, str_2.capacity());
	EXPECT_EQ(2, str_2.countRef());
	EXPECT_EQ(false, str_2.empty());
}

TEST(Ñonstructor, CopyPosLenEmpty)
{
	String str = String();
	String str_0 = String(str, 0, 1);
	ASSERT_STRCASEEQ("\0", str_0.data());
	ASSERT_EQ(0, str_0.size());
	ASSERT_EQ(1, str_0.capacity());
	ASSERT_EQ(1, str_0.countRef());
	ASSERT_EQ(true, str_0.empty());
}

TEST(Ñonstructor, CopyPosLen)
{
	String _str = String("Test_1");
	String str_1 = String(_str, 2, 2);
	EXPECT_STRCASEEQ("st", str_1.data());
	EXPECT_EQ(2, str_1.size());
	EXPECT_EQ(3, str_1.capacity());
	EXPECT_EQ(1, str_1.countRef());
	EXPECT_EQ(false, str_1.empty());

	String str_2 = String(_str, 2);
	EXPECT_STRCASEEQ("st_1", str_2.data());
	EXPECT_EQ(4, str_2.size());
	EXPECT_EQ(5, str_2.capacity());
	EXPECT_EQ(1, str_2.countRef());
	EXPECT_EQ(false, str_2.empty());
}

TEST(Reserve, LessThanCapacity)
{
	String str = String("abcd");
	str.reserve(1);
	ASSERT_STRCASEEQ("abcd", str.data());
	ASSERT_EQ(4, str.size());
	ASSERT_EQ(5, str.capacity());
}

TEST(Reserve, GreaterThanCapacity)
{
	String str = String("abcd");
	str.reserve(100);
	ASSERT_STRCASEEQ("abcd", str.data());
	ASSERT_EQ(4, str.size());
	ASSERT_EQ(100, str.capacity());
}

TEST(Clear, EmptyString)
{
	String str = String();
	str.clear();
	ASSERT_STRCASEEQ("\0", str.data());
	ASSERT_EQ(0, str.size());
	ASSERT_EQ(1, str.capacity());
}

TEST(Clear, NonEmptyString)
{
	String str = String("abcd");
	str.clear();
	ASSERT_STRCASEEQ("\0", str.data());
	ASSERT_EQ(0, str.size());
	ASSERT_EQ(5, str.capacity());
}

TEST(Methods, NonConstAt)
{
	String _str = String("Test");
	String str = String(_str);
	ASSERT_EQ(2, str.countRef());
	ASSERT_EQ('s', str.at(2));
	str.at(2) = 'e';
	ASSERT_EQ('e', str.at(2));
	ASSERT_EQ(1, str.countRef());
}

TEST(Methods, ConstAt)
{
	String _str = String("Test");
	const String str = String(_str);
	ASSERT_EQ(2, str.countRef());
	ASSERT_EQ('s', str.at(2));
	ASSERT_EQ(2, str.countRef());
}

TEST(Operators, NonConstOperatorBrackets)
{
	String _str = String("Test");
	String str = String(_str);
	ASSERT_EQ(2, str.countRef());
	ASSERT_EQ('s', str.at(2));
	str[2] = 'e';
	ASSERT_EQ('e', str.at(2));
	ASSERT_EQ(1, str.countRef());
}

TEST(Operators, ConstOperatorBrackets)
{
	String _str = String("Test");
	const String str = String(_str);
	ASSERT_EQ(2, str.countRef());
	ASSERT_EQ('s', str[2]);
	ASSERT_EQ(2, str.countRef());
}

TEST(Methods, NonConstBack)
{
	String _str = String("Test");
	String str = String(_str);
	ASSERT_EQ(2, str.countRef());
	ASSERT_EQ('t', str.back());
	str.back() = 'e';
	ASSERT_EQ('e', str.back());
	ASSERT_EQ(1, str.countRef());
}

TEST(Methods, ConstBack)
{
	String _str = String("Test");
	const String str = String(_str);
	ASSERT_EQ(2, str.countRef());
	ASSERT_EQ('t', str.back());
	ASSERT_EQ(2, str.countRef());
}

TEST(Methods, NonConstFront)
{
	String _str = String("Test");
	String str = String(_str);
	ASSERT_EQ(2, str.countRef());
	ASSERT_EQ('T', str.front());
	str.front() = 'e';
	ASSERT_EQ('e', str.front());
	ASSERT_EQ(1, str.countRef());
}

TEST(Methods, ConstFront)
{
	String _str = String("Test");
	const String str = String(_str);
	ASSERT_EQ(2, str.countRef());
	ASSERT_EQ('T', str.front());
	ASSERT_EQ(2, str.countRef());
}

TEST(OperatorPlusAssignment, String)
{
	String str_1 = String("Test");
	String str_2 = String("Check");
	str_1 += str_2;
	ASSERT_STRCASEEQ("TestCheck", str_1.data());
}

TEST(OperatorPlusAssignment, CharArray)
{
	const char* test = "Test";
	String str = String("Check");
	str += test;
	ASSERT_STRCASEEQ("CheckTest", str.data());
}

TEST(OperatorPlusAssignment, Char)
{
	String str = String("Test");
	const char symbol = 's';
	str += symbol;
	ASSERT_STRCASEEQ("Tests", str.data());
}

TEST(OperatorAssignment, String)
{
	String str_1 = String("Test");
	String str_2 = String("Check");
	str_2 = str_1;
	ASSERT_STRCASEEQ("Test", str_2.data());
}

TEST(OperatorAssignment, CharArray)
{
	const char* test = "Test";
	String str = String("Check");
	str = test;
	ASSERT_STRCASEEQ("Test", str.data());
}

TEST(Insert, EmptyString)
{
	String str_1 = String();
	String str_2 = String();
	str_1.insert(0, str_2);
	ASSERT_STRCASEEQ("\0", str_1.data());
}

TEST(Insert, NonEmptyString)
{
	String str_1 = String("ad");
	String str_2 = String("bc");
	str_1.insert(1, str_2);
	ASSERT_STRCASEEQ("abcd", str_1.data());
}

TEST(Insert, CharArray)
{
	String str_1 = String("ad");
	const char* str = "bc";
	str_1.insert(1, str);
	ASSERT_STRCASEEQ("abcd", str_1.data());
}

TEST(Insert, Char)
{
	String str_1 = String("ad");
	char ch = 'c';
	str_1.insert(1, 2, ch);
	ASSERT_STRCASEEQ("accd", str_1.data());
}

TEST(Erase, Empty)
{
	String str = String();
	str.erase();
	ASSERT_STRCASEEQ("\0", str.data());
}

TEST(Erase, NposTest)
{
	String str = String("Test_test");
	str.erase(4);
	ASSERT_STRCASEEQ("Test", str.data());
}

TEST(Erase, SimpleTest)
{
	String str = String("Test_test");
	str.erase(3, 5);
	ASSERT_STRCASEEQ("Test", str.data());
}

TEST(Replace, SimpleTest)
{
	const char* test = "Test", * check = "Check";
	String str_1 = String(test);
	String str_2 = String(check);

	str_1.replace(0, 2, str_2);
	EXPECT_STRCASEEQ("Checkst", str_1.data());

	str_1.replace(3, 0, 3, '%');
	EXPECT_STRCASEEQ("Che%%%ckst", str_1.data());

	str_1.replace(6, 4, test);
	EXPECT_STRCASEEQ("Che%%%Test", str_1.data());
}

TEST(Swap, Empty) {
	String str_1 = String();
	String str_2 = String(10, '\0');
	str_1.swap(str_2);
	ASSERT_STRCASEEQ("\0", str_1.data());
	ASSERT_STRCASEEQ("\0", str_2.data());
	ASSERT_EQ(11, str_1.capacity());
	ASSERT_EQ(1, str_2.capacity());
}

TEST(Swap, NonEmpty) {
	String str_1 = String("Test");
	String str_2 = String("Check");
	str_1.swap(str_2);
	ASSERT_STRCASEEQ("Check", str_1.data());
	ASSERT_STRCASEEQ("Test", str_2.data());
}

TEST(Find, Null) {
	String str("abcd");
	ASSERT_EQ(String::npos, str.find(nullptr));
}

TEST(Find, SimpleTest)
{
	const char* haystack = "Hello World", * needle = "Wo";
	String str_1 = String(haystack);
	String str_2 = String(needle);
	EXPECT_EQ(6 , str_1.find(str_2));
	EXPECT_EQ(-1, str_1.find(str_2, 7));
	EXPECT_EQ(6, str_1.find(needle, 3));
	EXPECT_EQ(-1, str_1.find(needle, 8));
	EXPECT_EQ(4, str_1.find('o'));
}

TEST(Substr, SimpleTest)
{
	String str = String("Hello World");

	String new_str = str.substr(2);
	EXPECT_STRCASEEQ("llo World", new_str.data());

	String new_str_2 = str.substr(2, 5);
	EXPECT_STRCASEEQ("llo W", new_str_2.data());

	String new_str_3 = str.substr(0, 1);
	EXPECT_STRCASEEQ("H", new_str_3.data());

	String new_str_4 = str.substr();
	EXPECT_STRCASEEQ("Hello World", new_str_4.data());
}

TEST(Compare, Less)
{
	String str_1 = String("Hello World");
	String str_2 = String("hello world");
	ASSERT_EQ(-1, str_1.compare(str_2));
	ASSERT_EQ(-1, str_1.compare("hello world"));
}

TEST(Compare, Greater)
{
	String str_1 = String("Hello World");
	String str_2 = String("Hello");
	ASSERT_EQ(1, str_1.compare(str_2));
	ASSERT_EQ(1, str_1.compare("Hello"));
}

TEST(Compare, Equal)
{
	String str_1 = String("Hello World");
	String str_2 = String("Hello World");
	ASSERT_EQ(0, str_1.compare(str_2));
	ASSERT_EQ(0, str_1.compare("Hello World"));
}

TEST(RefCount, ConstructorTest)
{
	String str_1 = String("Test");
	String str_2 = String(str_1);
	EXPECT_EQ(2, str_1.countRef());
	EXPECT_EQ(2, str_2.countRef());

	String str_3 = String(str_1);
	EXPECT_EQ(3, str_1.countRef());
	EXPECT_EQ(3, str_3.countRef());
}

TEST(RefCount, OperatorTest)
{
	String str_1 = String("Test");
	String str_2 = String(str_1);
	String str_3 = String(str_1);

	str_2 += str_1;
	EXPECT_EQ(2, str_1.countRef());

	str_1 += str_3;
	EXPECT_EQ(1, str_3.countRef());
}

TEST(RefCount, InsertTest) {
	String str_1 = String("Test");
	String str_2 = String(str_1);
	str_2.insert(0, "1");
	EXPECT_EQ(1, str_2.countRef());
}
#include "pch.h"
#include "skip_list.hpp"
#include <random>

class custom{
	double x_{};
	std::string str_{};
public:
	custom() = default;
	explicit custom(double&& x, std::string&& str) : x_(x), str_(str) {}
	bool operator==(const custom& another) const { return x_ == another.x_ && str_ == another.str_; }
	bool operator!=(const custom& another) const { return !(*this == another); }
	bool operator<(const custom& another) const { return x_ < another.x_; }
};

template <typename T>
struct custom_alloc {
	static void* allocate(const size_t count) {
		T* ptr = static_cast<T*>(::operator new(count * sizeof(T)));
		return static_cast<void*>(ptr);
	}
	static void deallocate(T* ptr, const size_t count) {
		operator delete(static_cast<void*>(ptr));
	}
};

TEST(Constructor, Empty)
{
	skip_list<int, std::string> list;
	ASSERT_TRUE(list.empty());
	ASSERT_EQ(list.size(), 0);
	ASSERT_TRUE(list.begin() == list.end());
}

TEST(Constructor, CustomComp)
{
	skip_list<int, std::string> list_1;
	list_1[1] = "one";
	list_1[5] = "five";
	std::string buf;
	for (auto i = list_1.begin(); i != list_1.end(); ++i) {
		buf += i->second;
	}
	ASSERT_EQ(buf, "onefive");
	
	skip_list<int, std::string, std::greater<>> list_2;
	list_2[1] = "one";
	list_2[5] = "five";
	buf.clear();
	for (auto i = list_2.begin(); i != list_2.end(); ++i) {
		buf += i->second;
	}
	ASSERT_EQ(buf, "fiveone");
}

TEST(Constructor, CustomAlloc)
{
	skip_list<int, custom, std::greater<>, custom_alloc<std::pair<const int, custom>>> list;
	list[1] = custom(0.2, "one");
	list[0] = custom(0.1, "zero");
	ASSERT_EQ(list.begin()->second, custom(0.2, "one"));
	ASSERT_EQ((++list.begin())->second, custom(0.1, "zero"));
}

TEST(Constructor, Copy)
{
	skip_list<std::string, custom> list_1;
	list_1["One"] = custom(0.5, "one");
	auto list_2 = skip_list<std::string, custom>(list_1);
	ASSERT_TRUE(!list_2.empty());
	ASSERT_EQ(list_2.size(), 1);
	ASSERT_EQ((list_2.find("One"))->second, custom(0.5, "one"));
}

TEST(Constructor, Move)
{
	skip_list<custom, int> list_1;
	const auto key_0 = custom(0, "zero");
	const auto key_1 = custom(0.1, "one");
	const auto key_2 = custom(0.2, "two");
	list_1[key_2] = 2;
	list_1[key_1] = 1;
	list_1[key_0] = 0;
	auto list_2 = skip_list(std::move(list_1));
	ASSERT_TRUE(list_1.empty());
	ASSERT_EQ(list_2[key_2], 2);
	ASSERT_EQ(list_2[key_1], 1);
	ASSERT_EQ(list_2[key_0], 0);
}

TEST(Operator, CopyAssignment)
{
	skip_list<std::string, std::string> list_1;
	list_1["One"] = "_one";
	auto& list_2 = list_1;
	ASSERT_TRUE(!list_2.empty());
	ASSERT_EQ(list_2.size(), 1);
	ASSERT_EQ((list_2.find("One"))->second, "_one");
}

TEST(Operator, Move)
{
	skip_list<custom, std::string> list_1;
	const auto key_0 = custom(0, "zero");
	const auto key_1 = custom(0.1, "one");
	const auto key_2 = custom(0.2, "two");
	list_1[key_2] = "str2";
	list_1[key_1] = "str1";
	list_1[key_0] = "str0";
	auto list_2 = std::move(list_1);
	ASSERT_TRUE(list_1.empty());
	ASSERT_EQ(list_2[key_2], "str2");
	ASSERT_EQ(list_2[key_1], "str1");
	ASSERT_EQ(list_2[key_0], "str0");

	list_1 = std::move(list_2);
	ASSERT_TRUE(list_2.empty());
	ASSERT_EQ(list_1[key_2], "str2");
	ASSERT_EQ(list_1[key_1], "str1");
	ASSERT_EQ(list_1[key_0], "str0");
}

TEST(Operator, Brackets)
{
	const auto key_1 = custom(0.2, "one");
	const auto key_2 = custom(0.1, "zero");
	skip_list<custom, std::string> list;
	list[key_1] = "_one";
	list[key_2] = "_two";
	ASSERT_TRUE(!list.empty());
	ASSERT_EQ(list.size(), 2);
	ASSERT_EQ(list[key_1], "_one");
	ASSERT_EQ(list[key_2], "_two");
}

TEST(Operator, Equal)
{
	skip_list<std::string, std::string> list_1;
	list_1["One"] = "_one";
	const auto& list_2 = list_1;
	ASSERT_TRUE(list_1 == list_2);
}

TEST(Operator, NonEqual)
{
	skip_list<std::string, std::string> list_1;
	skip_list<std::string, std::string> list_2;
	list_1["One"] = "_one";
	list_2["Two"] = "_two";
	ASSERT_TRUE(list_1 != list_2);
}

TEST(Iterator, Begin)
{
	skip_list<std::string, std::string> list;
	list["One"] = "_one";
	ASSERT_EQ(list.begin()->second, "_one");
	list["Two"] = "_two";
	ASSERT_EQ(list.begin()->second, "_one");
}

TEST(Iterator, ConstBegin)
{
	skip_list<std::string, std::string> list;
	list["One"] = "_one";
	const auto& list_1 = list;
	ASSERT_EQ(list_1.begin()->second, "_one");
	list["Two"] = "_two";
	const auto& list_2 = list;
	ASSERT_EQ(list_2.begin()->second, "_one");
}

TEST(Iterator, End)
{
	skip_list<std::string, std::string> list;
	list["One"] = "_one";
	list["Two"] = "_two";
	list["Three"] = "_three";
	std::string buf;
	for (auto i = list.begin(); i != list.end(); ++i) {
		buf += i->second;
	}
	ASSERT_EQ(buf, "_one_three_two");
}

TEST(Iterator, ConstEnd)
{
	skip_list<std::string, std::string> list_1;
	list_1["One"] = "_one";
	list_1["Two"] = "_two";
	list_1["Three"] = "_three";
	std::string buf;
	const auto& list_2 = list_1;
	for (auto i = list_2.begin(); i != list_2.end(); ++i) {
		buf += i->second;
	}
	ASSERT_EQ(buf, "_one_three_two");
}

TEST(At, NonConst)
{
	skip_list<std::string, int> list;
	ASSERT_TRUE(list.empty());
	ASSERT_EQ(list.size(), 0);
	try{
		list.at("one");
	}
	catch (std::out_of_range&){
		ASSERT_TRUE(list.empty());
		ASSERT_EQ(list.size(), 0);
	}
	list["one"] = 1;
	ASSERT_TRUE(!list.empty());
	ASSERT_EQ(list.size(), 1);
	ASSERT_EQ(list.at("one"), 1);
}

TEST(At, Const)
{
	skip_list<std::string, int> list;
	list["one"] = 1;
	const auto& list_1 = list;
	try {
		list.at("two");
	}
	catch (std::out_of_range&) {
		ASSERT_EQ(list.size(), 1);
	}
	ASSERT_EQ(list_1.at("one"), 1);
}

TEST(Insert, Empty)
{
	skip_list<std::string, int> list;
	ASSERT_TRUE(list.empty());
	ASSERT_EQ(list.size(), 0);
	ASSERT_TRUE(list.begin() == list.end());
	auto it = list.insert(std::make_pair("two", 2));
	ASSERT_EQ(it.first, list.begin());
	ASSERT_EQ(it.second, true);
	ASSERT_TRUE(!list.empty());
	ASSERT_EQ(list.size(), 1);
	ASSERT_EQ(list.begin()->second, 2);
}

TEST(Insert, ExsistingKey)
{
	skip_list<std::string, int> list;
	list["two"] = 2;
	auto it = list.insert(std::make_pair("two", 1));
	ASSERT_EQ(it.first, list.begin());
	ASSERT_EQ(it.second, false);
	ASSERT_EQ(list.size(), 1);
	ASSERT_EQ(list.begin()->second, 2);
	it = list.insert(std::make_pair("two", 33));
	ASSERT_EQ(it.first, list.begin());
	ASSERT_EQ(it.second, false);
	ASSERT_EQ(list.size(), 1);
	ASSERT_EQ(list.begin()->second, 2);
}

TEST(Insert, NonExsistingKey)
{
	skip_list<std::string, int> list;
	list["two"] = 2;
	auto it = list.insert(std::make_pair("one", 1));
	ASSERT_EQ(it.first, list.begin());
	ASSERT_EQ(it.second, true);
	ASSERT_EQ(list.size(), 2);
	ASSERT_EQ(list.begin()->second, 1);
	it = list.insert(std::make_pair("a", 33));
	ASSERT_EQ(it.first, list.begin());
	ASSERT_EQ(it.second, true);
	ASSERT_EQ(list.size(), 3);
	ASSERT_EQ(list.begin()->second, 33);
}

TEST(KeyErase, Empty)
{
	skip_list<int, int> list;
	ASSERT_EQ(list.erase(5), 0);
}

TEST(KeyErase, ExsistingItem)
{
	skip_list<int, int> list;
	list[5] = 6;
	ASSERT_EQ(list.erase(5), 1);
	ASSERT_TRUE(list.empty());
	list.insert(std::make_pair(1, 4));
	list.insert(std::make_pair(1, 5));
	list.insert(std::make_pair(1, 6));
	ASSERT_EQ(list.erase(1), 1);
	ASSERT_TRUE(list.empty());
}

TEST(KeyErase, NonExsistingItem)
{
	skip_list<int, int> list;
	list[5] = 6;
	ASSERT_EQ(list.erase(6), 0);
	ASSERT_EQ(list.size(), 1);
}

TEST(PosErase, Empty)
{
	skip_list<int, int> list;
	ASSERT_EQ(list.begin(), list.end());
	try {
		list.erase(list.begin());
	}
	catch(std::out_of_range&){}
	ASSERT_EQ(list.begin(), list.end());
}

TEST(PosErase, NonEmpty)
{
	skip_list<int, int> list;
	list[0] = 1;
	list[1] = 2;
	list[2] = 3;
	list.insert(std::make_pair(2, 4));
	list[3] = 5;
	list.erase(list.find(2));
	ASSERT_EQ(list.size(), 3);
	ASSERT_EQ(list.find(2), list.end());
}

TEST(IterErase, Empty)
{
	skip_list<int, int> list;
	list.erase(list.begin(), list.end());
	ASSERT_TRUE(list.empty());
	ASSERT_EQ(list.size(), 0);
}

TEST(IterErase, NonEmpty)
{
	skip_list<int, int> list;
	list[0] = 1;
	list[1] = 2;
	list[2] = 3;
	list.insert(std::make_pair(2, 4));
	list[3] = 5;
	ASSERT_EQ(list.size(), 4);
	list.erase(list.find(1), list.end());
	ASSERT_EQ(list.size(), 1);
	ASSERT_EQ(list.begin()->second, 1);
}

TEST(Find, NonConst)
{
	skip_list<int, int> list;
	list[0] = 1;
	list[1] = 2;
	list[2] = 33;
	list.insert(std::make_pair(2, 4));
	list[3] = 5;
	auto it = list.find(2);
	ASSERT_EQ(it->first, 2); ASSERT_EQ(it->second, 33);
	it->second = 3;
	it = list.find(2);
	ASSERT_EQ(it->first, 2); ASSERT_EQ(it->second, 3);
}

TEST(Find, Const)
{
	skip_list<int, int> list;
	list[0] = 1;
	list[1] = 2;
	list[2] = 3;
	list.insert(std::make_pair(2, 4));
	list[3] = 5;
	const auto list_copy = skip_list<int, int>(list);
	auto it = list_copy.find(3);
	ASSERT_EQ(it->first, 3); ASSERT_EQ(it->second, 5);
}

TEST(Clear, Empty)
{
	skip_list<std::string, int> list;
	ASSERT_TRUE(list.empty());
	ASSERT_EQ(list.size(), 0);
	list.clear();
	ASSERT_TRUE(list.empty());
	ASSERT_EQ(list.size(), 0);
}

TEST(Clear, NonEmpty)
{
	skip_list<int, int> list;
	list[0] = 1;
	list[1] = 2;
	list[2] = 3;
	list.insert(std::make_pair(2, 4));
	list[3] = 5;
	ASSERT_EQ(list.begin()->second, 1);
	ASSERT_TRUE(!list.empty());
	ASSERT_EQ(list.size(), 4);
	list.clear();
	ASSERT_TRUE(list.empty());
	ASSERT_EQ(list.size(), 0);
	list[-5] = 1;
	list[-10] = 0;
	list[0] = 2;
	list[-1] = 5;
	ASSERT_EQ(list.begin()->second, 0);
	ASSERT_EQ(list.begin()->first, -10);
	ASSERT_TRUE(!list.empty());
	ASSERT_EQ(list.size(), 4);
}

TEST(Swap, Empty) {
	skip_list<int, int> list_1, list_2;
	list_2[1] = 3;
	list_1.swap(list_2);
	ASSERT_TRUE(list_2.empty());
	ASSERT_EQ(list_1.begin()->second, 3);
}

TEST(Swap, NonEmpty) {
	skip_list<std::string, int> list_1, list_2;
	list_1["one"] = 1;
	list_2["two"] = 2;
	list_1.swap(list_2);
	ASSERT_EQ(list_1.begin()->second, 2);
	ASSERT_EQ(list_2.begin()->second, 1);
	ASSERT_EQ(list_1.begin()->first, "two");
	ASSERT_EQ(list_2.begin()->first, "one");
}

TEST(Iterator, IndirectionOperatorEmpty)
{
	skip_list<int, int>::iterator i;
	try{
		auto ptr = *i;
	}
	catch(std::out_of_range&) {}
}


TEST(Iterator, PrefixPostfix)
{
	skip_list<int, int> list;
	std::map<int, int> map;
	for (auto i = 0; i < 4; ++i) {
		list[i] = i;
		map[i] = i;
	}
	auto mit = map.begin();
	EXPECT_EQ(mit->first, 0);
	auto m0 = mit++;
	EXPECT_EQ(*m0, *map.begin());
	auto m1 = ++mit;
	EXPECT_EQ((*m1).first, 2);
	auto m2 = mit++;
	EXPECT_EQ(m2->first, 2);
	EXPECT_EQ(m1, m2);
	EXPECT_EQ(*m1, *m2);
	auto lit = list.begin();
	EXPECT_EQ(lit->first, 0);
	auto l0 = lit++;
	EXPECT_EQ(*l0, *list.begin());
	auto l1 = ++lit;
	EXPECT_EQ((*l1).first, 2);
	auto l2 = lit++;
	EXPECT_EQ(l2->first, 2);
	EXPECT_EQ(l1, l2);
	EXPECT_EQ(*l1, *l2);
	lit = list.begin();
	mit = map.begin();
	while (mit != map.end()) {
		EXPECT_EQ(*(lit++), *(mit++));
	}
}

namespace {
	template<typename T, typename Seed = unsigned int>
	struct PseudoRand
	{
		PseudoRand(T min_ = std::numeric_limits<T>::min(), T max_ = std::numeric_limits<T>::max(), Seed seed = Seed{ 0 }) : gen{ seed }, distrib{ min_, max_ }{}
		std::default_random_engine gen;
		std::uniform_int_distribution<T> distrib;
		T operator()() {
			return distrib(gen);
		}
	};

}

TEST(Iterator_ex, forwardFill) {
	skip_list<int, int> tr;
	for (int i = 0; i < 10; ++i) {
		tr[i] = i;
	}
	auto lit = tr.begin();
	for (int i = 0; i < 10; ++i) {
		EXPECT_EQ(*lit, (std::pair<const int, int>{ i, i }));
		++lit;
	}
}

TEST(Iterator_ex, backwardFill) {
	skip_list<int, int> tr;
	for (int i = 9; i >= 0; --i) {
		tr[i] = i;
	}
	auto lit = tr.begin();
	for (int i = 0; i < 10; ++i) {
		EXPECT_EQ(*lit, (std::pair<const int, int>{ i, i }));
		++lit;
	}
}

TEST(Iterator_ex, randomFill) {
	skip_list<int, int> tr;
	std::vector<int> v(100);
	int cnt = 0;
	std::generate_n(v.begin(), v.size(), [&]() { return cnt++; });
	std::shuffle(begin(v), end(v), std::default_random_engine{ 0 });
	for (auto i : v) {
		tr[i] = i;
	}
	auto lit = tr.begin();
	for (int i = 0; i < (int)v.size(); ++i) {
		EXPECT_EQ(*lit, (skip_list<int, int>::value_type{ i,i }));
		++lit;
	}
}

TEST(Iterator_ex, withMap) {
	skip_list<int, int> tr;
	std::map<int, int> map;
	PseudoRand<int> rand(0, 99);
	for (int i = 0; i < 100; ++i) {
		auto r = rand();
		tr[i] = r;
		map[i] = r;
	}
	EXPECT_EQ(tr.size(), map.size());
	EXPECT_EQ(std::mismatch(tr.begin(), tr.end(), map.begin(), map.end()).first, tr.end());

	{
		auto lit = tr.begin();
		auto mit = map.begin();
		while (lit != tr.end()) {
			EXPECT_EQ(*(lit++), *(mit++));
		}
	}
}

TEST(Iterator_ex, postfixBackwardFill) {
	skip_list<int, int> tr;
	for (int i = 9; i >= 0; --i) {
		tr[i] = i;
	}
	{
		auto lit = tr.begin();
		for (int i = 0; i < 10; ++i) {
			EXPECT_EQ(*(lit++), (skip_list<int, int>::value_type{ i,i }));
		}
	}
}
TEST(Iterator_ex, postfixForwardFill) {
	skip_list<int, int> tr;
	for (int i = 0; i < 10; ++i) {
		tr[i] = i;
	}
	{
		auto lit = tr.begin();
		for (int i = 0; i < 10; ++i) {
			EXPECT_EQ(*(lit++), (skip_list<int, int>::value_type{ i,i }));
		}
	}
}
TEST(Iterator_ex, prefixBackwardFill) {
	skip_list<int, int> tr;
	for (int i = 9; i >= 0; --i) {
		tr[i] = i;
	}
	{
		auto lit = tr.begin();
		for (int i = 1; i < 10; ++i) {
			EXPECT_EQ(*(++lit), (skip_list<int, int>::value_type{ i,i }));
		}
	}
}
TEST(Iterator_ex, prefixForwardFill) {
	skip_list<int, int> tr;
	for (int i = 0; i < 10; ++i) {
		tr[i] = i;
	}
	{
		auto lit = tr.begin();
		for (int i = 1; i < 10; ++i) {
			EXPECT_EQ(*(++lit), (skip_list<int, int>::value_type{ i,i }));
		}
	}
}

TEST(Iterator_ex, PrefixPostfix) {
	skip_list<int, int> tr;
	std::map<int, int> map;
	for (int i = 0; i < 4; ++i) {
		tr[i] = i;
		map[i] = i;
	}
	auto mit = map.begin();
	EXPECT_EQ(mit->first, 0);
	auto m0 = mit++;
	EXPECT_EQ(*m0, *map.begin());
	auto m1 = ++mit;
	EXPECT_EQ((*m1).first, 2);
	auto m2 = mit++;
	EXPECT_EQ(m2->first, 2);
	EXPECT_EQ(m1, m2);
	EXPECT_EQ(*m1, *m2);

	auto lit = tr.begin();
	EXPECT_EQ(lit->first, 0);
	auto l0 = lit++;
	EXPECT_EQ(*l0, *tr.begin());
	auto l1 = ++lit;
	EXPECT_EQ((*l1).first, 2);
	auto l2 = lit++;
	EXPECT_EQ(l2->first, 2);
	EXPECT_EQ(l1, l2);
	EXPECT_EQ(*l1, *l2);

	lit = tr.begin();
	mit = map.begin();
	while (mit != map.end()) {
		EXPECT_EQ(*(lit++), *(mit++));
	}
}

TEST(CopyConstructor, ExsistingKey)
{
	skip_list<std::string, int> list;
	list.insert(std::make_pair("two", 2));
	list.insert(std::make_pair("two", 1));
	list.insert(std::make_pair("two", 33));
	skip_list<std::string, int> copy{ list };
	EXPECT_EQ(list.size(), copy.size());
	EXPECT_EQ(list, copy);
}

TEST(Copy, ExsistingKey)
{
	skip_list<std::string, int> list;
	list.insert(std::make_pair("two", 2));
	list.insert(std::make_pair("two", 1));
	list.insert(std::make_pair("two", 33));
	skip_list<std::string, int> copy;
	copy = list;
	EXPECT_EQ(list.size(), copy.size());
	EXPECT_EQ(list, copy);
}
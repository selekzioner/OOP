#include "pch.h"
#include "tree.hpp"
#include <random>

class custom {
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
	tree<int, std::string> t;
	ASSERT_TRUE(t.empty());
	ASSERT_EQ(t.size(), 0);
	ASSERT_TRUE(t.begin() == t.end());
}

TEST(Constructor, CustomComp)
{
	tree<int, std::string> t1;
	t1[3] = "three";
	t1[1] = "one";
	t1[5] = "five";
	std::string buf;
	for (auto i = t1.begin(); i != t1.end(); ++i) {
		buf += i->second;
	}
	ASSERT_EQ(buf, "onethreefive");

	tree<int, std::string, std::greater<>> t2;
	t2[3] = "three";
	t2[1] = "one";
	t2[5] = "five";
	buf.clear();
	for (auto i = t2.begin(); i != t2.end(); ++i) {
		buf += i->second;
	}
	ASSERT_EQ(buf, "fivethreeone");
}

TEST(Constructor, CustomAlloc)
{
	tree<int, custom, std::greater<>, custom_alloc<std::pair<const int, custom>>> t;
	t[1] = custom(0.2, "one");
	t[0] = custom(0.3, "zero");
	t[2] = custom(0.44, "six");
	t[3] = custom(0.7, "seven");
	ASSERT_EQ(t.size(), 4);
	ASSERT_EQ(t.begin()->second, custom(0.7, "seven"));
	ASSERT_EQ((++t.begin())->second, custom(0.44, "six"));
}

TEST(Constructor, Copy)
{
	tree<std::string, custom> t1;
	t1["Two"] = custom(0.5, "one");
	t1["One"] = custom(0.1, "two");
	t1["Four"] = custom(0.4, "three");
	t1["Three"] = custom(0.11, "five");
	auto t2 = tree<std::string, custom>(t1);
	ASSERT_TRUE(!t2.empty());
	ASSERT_EQ(t2.size(), 4);
	for (auto i = t1.begin(); i != t1.end(); ++i){
		ASSERT_EQ(t1.at(i->first), t2.at(i->first));
	}
}

TEST(Constructor, Move)
{
	tree<custom, int> t1;
	const auto key_0 = custom(0, "zero");
	const auto key_1 = custom(0.1, "one");
	const auto key_2 = custom(0.2, "two");
	t1[key_2] = 2;
	t1[key_1] = 1;
	t1[key_0] = 0;
	auto t2 = tree<custom, int>(std::move(t1));
	ASSERT_TRUE(t1.empty());
	ASSERT_EQ(t2[key_2], 2);
	ASSERT_EQ(t2[key_1], 1);
	ASSERT_EQ(t2[key_0], 0);
}

TEST(Operator, CopyAssignment)
{
	tree<std::string, std::string> t1;
	t1["One"] = "_one";
	auto& t2 = t1;
	ASSERT_TRUE(!t2.empty());
	ASSERT_EQ(t2.size(), 1);
	ASSERT_EQ((t2.find("One"))->second, "_one");
}

TEST(Operator, Move)
{
	tree<custom, std::string> t1;
	const auto key_0 = custom(0, "zero");
	const auto key_1 = custom(0.1, "one");
	const auto key_2 = custom(0.2, "two");
	t1[key_2] = "str2";
	t1[key_1] = "str1";
	t1[key_0] = "str0";
	auto t2 = std::move(t1);
	ASSERT_TRUE(t1.empty());
	ASSERT_EQ(t2[key_2], "str2");
	ASSERT_EQ(t2[key_1], "str1");
	ASSERT_EQ(t2[key_0], "str0");

	t1 = std::move(t2);
	ASSERT_TRUE(t2.empty());
	ASSERT_EQ(t1[key_2], "str2");
	ASSERT_EQ(t1[key_1], "str1");
	ASSERT_EQ(t1[key_0], "str0");
}

TEST(Operator, Brackets)
{
	const auto key_1 = custom(0.2, "one");
	const auto key_2 = custom(0.1, "zero");
	tree<custom, std::string> t;
	t[key_1] = "_one";
	t[key_2] = "_two";
	t[key_1] = "_three";
	ASSERT_TRUE(!t.empty());
	ASSERT_EQ(t.size(), 2);
	ASSERT_EQ(t[key_1], "_three");
	ASSERT_EQ(t[key_2], "_two");
}

TEST(Operator, Equal)
{
	tree<std::string, std::string> t1;
	t1["Two"] = "_two";
	t1["One"] = "_one";
	t1["Three"] = "_three";
	const auto t2 = t1;
	ASSERT_TRUE(t1 == t2);
}

TEST(Operator, NonEqual)
{
	tree<std::string, std::string> t1;
	tree<std::string, std::string> t2;
	t1["One"] = "_one";
	t2["Two"] = "_two";
	ASSERT_TRUE(t1 != t2);
}

TEST(Iterator, Begin)
{
	tree<std::string, std::string> t;
	t["One"] = "_one";
	ASSERT_EQ(t.begin()->second, "_one");
	t["Two"] = "_two";
	ASSERT_EQ(t.begin()->second, "_one");
}

TEST(Iterator, ConstBegin)
{
	tree<std::string, std::string> t;
	t["One"] = "_one";
	const auto& t1 = t;
	ASSERT_EQ(t1.begin()->second, "_one");
	t["Two"] = "_two";
	const auto& t2 = t;
	ASSERT_EQ(t2.begin()->second, "_one");
}

TEST(Iterator, End)
{
	tree<std::string, std::string> t;
	t["One"] = "_one";
	t["Two"] = "_two";
	t["Three"] = "_three";
	std::string buf;
	for (auto i = t.begin(); i != t.end(); ++i) {
		buf += i->second;
	}
	ASSERT_EQ(buf, "_one_three_two");
}

TEST(Iterator, ConstEnd)
{
	tree<std::string, std::string> t1;
	t1["One"] = "_one";
	t1["Two"] = "_two";
	t1["Three"] = "_three";
	std::string buf;
	const auto& t2 = t1;
	for (auto i = t2.begin(); i != t2.end(); ++i) {
		buf += i->second;
	}
	ASSERT_EQ(buf, "_one_three_two");
}

TEST(At, NonConst)
{
	tree<std::string, int> t;
	ASSERT_TRUE(t.empty());
	ASSERT_EQ(t.size(), 0);
	EXPECT_THROW(t.at("one"), std::out_of_range);
	ASSERT_TRUE(t.empty());
	ASSERT_EQ(t.size(), 0);
	t["one"] = 1;
	ASSERT_TRUE(!t.empty());
	ASSERT_EQ(t.size(), 1);
	ASSERT_EQ(t.at("one"), 1);
}

TEST(At, Const)
{
	tree<std::string, int> t;
	t["one"] = 1;
	const auto& t1 = t;
	EXPECT_THROW(t.at("two"), std::out_of_range);
	ASSERT_EQ(t.size(), 1);
	ASSERT_EQ(t1.at("one"), 1);
}

TEST(Insert, Empty)
{
	tree<std::string, int> t;
	ASSERT_TRUE(t.empty());
	ASSERT_EQ(t.size(), 0);
	ASSERT_TRUE(t.begin() == t.end());
	auto it = t.insert(std::make_pair("two", 2));
	ASSERT_EQ(it.first, t.begin());
	ASSERT_EQ(it.second, true);
	ASSERT_TRUE(!t.empty());
	ASSERT_EQ(t.size(), 1);
	ASSERT_EQ(t.begin()->second, 2);
}

TEST(Insert, ExsistingKey)
{
	tree<std::string, int> t;
	t["two"] = 2;
	auto it = t.insert(std::make_pair("two", 1));
	ASSERT_EQ(it.first, t.begin());
	ASSERT_EQ(it.second, false);
	ASSERT_EQ(t.size(), 1);
	ASSERT_EQ(t.begin()->second, 2);
	it = t.insert(std::make_pair("two", 33));
	ASSERT_EQ(it.first, t.begin());
	ASSERT_EQ(it.second, false);
	ASSERT_EQ(t.size(), 1);
	ASSERT_EQ(t.begin()->second, 2);
}

TEST(Insert, NonExsistingKey)
{
	tree<std::string, int> t;
	t["two"] = 2;
	auto it = t.insert(std::make_pair("one", 1));
	ASSERT_EQ(it.first, t.begin());
	ASSERT_EQ(it.second, true);
	ASSERT_EQ(t.size(), 2);
	ASSERT_EQ(t.begin()->second, 1);
	it = t.insert(std::make_pair("a", 33));
	ASSERT_EQ(it.first, t.begin());
	ASSERT_EQ(it.second, true);
	ASSERT_EQ(t.size(), 3);
	ASSERT_EQ(t.begin()->second, 33);
}

TEST(KeyErase, Empty)
{
	tree<int, int> t;
	t.erase(6);
	ASSERT_TRUE(t.empty());
}

TEST(KeyErase, ExsistingItem)
{
	tree<int, int> t;
	t[5] = 6;
	t.erase(5);
	ASSERT_TRUE(t.empty());
	t.insert(std::make_pair(1, 4));
	t.insert(std::make_pair(1, 5));
	t.insert(std::make_pair(1, 4));
	t.erase(1);
	ASSERT_TRUE(t.empty());
}

TEST(KeyErase, NonExsistingItem)
{
	tree<int, int> t;
	t[5] = 6;
	t.erase(6);
	ASSERT_EQ(t.size(), 1);
}

TEST(PosErase, Empty)
{
	tree<int, int> t;
	ASSERT_EQ(t.begin(), t.end());
	t.erase(t.begin());
	ASSERT_EQ(t.begin(), t.end());
}

TEST(PosErase, NonEmpty)
{
	tree<int, int> t;
	t[2] = 3;
	t[0] = 1;
	t[1] = 2;
	t.insert(std::make_pair(2, 4));
	t[3] = 5;
	t.erase(t.find(2));
	ASSERT_EQ(t.size(), 3);
	ASSERT_EQ(t.find(2), t.end());
}

TEST(IterErase, Empty)
{
	tree<int, int> t;
	t.erase(t.begin(), t.end());
	ASSERT_TRUE(t.empty());
	ASSERT_EQ(t.size(), 0);
}

TEST(IterErase, NonEmpty)
{
	tree<int, int> t;
	t[1] = 2;
	t[0] = 1;
	t[2] = 3;
	t.insert(std::make_pair(2, 4));
	t[3] = 5;
	t.erase(t.find(2), t.find(3));
	ASSERT_EQ(t.size(), 3);
	ASSERT_EQ(t.begin()->second, 1);
	ASSERT_EQ(++(t.begin())->second, 2);
}

TEST(Find, NonConst)
{
	tree<int, int> t;
	t[0] = 1;
	t[1] = 2;
	t[2] = 33;
	t.insert(std::make_pair(2, 4));
	t[3] = 5;
	auto it = t.find(2);
	ASSERT_EQ(it->first, 2); ASSERT_EQ(it->second, 33);
	it->second = 3;
	it = t.find(2);
	ASSERT_EQ(it->first, 2); ASSERT_EQ(it->second, 3);
}

TEST(Find, Const)
{
	tree<int, int> t;
	t[0] = 1;
	t[1] = 2;
	t[2] = 3;
	t.insert(std::make_pair(2, 4));
	t[3] = 5;
	const auto t_copy = tree<int, int>(t);
	auto it = t_copy.find(3);
	ASSERT_EQ(it->first, 3); ASSERT_EQ(it->second, 5);
}

TEST(Clear, Empty)
{
	tree<std::string, int> t;
	ASSERT_TRUE(t.empty());
	ASSERT_EQ(t.size(), 0);
	t.clear();
	ASSERT_TRUE(t.empty());
	ASSERT_EQ(t.size(), 0);
}

TEST(Clear, NonEmpty)
{
	tree<int, int> t;
	t[0] = 1;
	t[1] = 2;
	t[2] = 3;
	t.insert(std::make_pair(2, 4));
	t[3] = 5;
	ASSERT_TRUE(!t.empty());
	ASSERT_EQ(t.size(), 4);
	t.clear();
	ASSERT_TRUE(t.empty());
	ASSERT_EQ(t.size(), 0);
	t[-5] = 1;
	t[-10] = 0;
	t[0] = 2;
	t[-1] = 5;
	ASSERT_EQ(t.begin()->second, 0);
	ASSERT_EQ(t.begin()->first, -10);
	ASSERT_TRUE(!t.empty());
	ASSERT_EQ(t.size(), 4);
}

TEST(Swap, Empty) {
	tree<int, int> t1, t2;
	t2[1] = 3;
	t1.swap(t2);
	ASSERT_TRUE(t2.empty());
	ASSERT_EQ(t1.begin()->second, 3);
}

TEST(Swap, NonEmpty) {
	tree<std::string, int> t1, t2;
	t1["one"] = 1;
	t2["two"] = 2;
	t1.swap(t2);
	ASSERT_EQ(t1.begin()->second, 2);
	ASSERT_EQ(t2.begin()->second, 1);
	ASSERT_EQ(t1.begin()->first, "two");
	ASSERT_EQ(t2.begin()->first, "one");
}

TEST(Iterator, IndirectionOperatorEmpty)
{
	tree<int, int> tr;
	EXPECT_THROW(*(tr.begin()), std::out_of_range);
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
	tree<int, int> tr;
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
	tree<int, int> tr;
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
	tree<int, int> tr;
	std::vector<int> v(100);
	int cnt = 0;
	std::generate_n(v.begin(), v.size(), [&]() { return cnt++; });
	std::shuffle(begin(v), end(v), std::default_random_engine{ 0 });
	for (auto i : v) {
		tr[i] = i;
	}

	auto lit = tr.begin();
	for (int i = 0; i < (int)v.size(); ++i) {
		EXPECT_EQ(*lit, (tree<int, int>::value_type{ i,i }));
		++lit;
	}
}

TEST(Iterator_ex, withMap) {
	tree<int, int> tr;
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
	tree<int, int> tr;
	for (int i = 9; i >= 0; --i) {
		tr[i] = i;
	}
	{
		auto lit = tr.begin();
		for (int i = 0; i < 10; ++i) {
			EXPECT_EQ(*(lit++), (tree<int, int>::value_type{ i,i }));
		}
	}
}
TEST(Iterator_ex, postfixForwardFill) {
	tree<int, int> tr;
	for (int i = 0; i < 10; ++i) {
		tr[i] = i;
	}
	{
		auto lit = tr.begin();
		for (int i = 0; i < 10; ++i) {
			EXPECT_EQ(*(lit++), (tree<int, int>::value_type{ i,i }));
		}
	}
}
TEST(Iterator_ex, prefixBackwardFill) {
	tree<int, int> tr;
	for (int i = 9; i >= 0; --i) {
		tr[i] = i;
	}
	{
		auto lit = tr.begin();
		for (int i = 1; i < 10; ++i) {
			EXPECT_EQ(*(++lit), (tree<int, int>::value_type{ i,i }));
		}
	}
}
TEST(Iterator_ex, prefixForwardFill) {
	tree<int, int> tr;
	for (int i = 0; i < 10; ++i) {
		tr[i] = i;
	}
	{
		auto lit = tr.begin();
		for (int i = 1; i < 10; ++i) {
			EXPECT_EQ(*(++lit), (tree<int, int>::value_type{ i,i }));
		}
	}
}

TEST(Iterator_ex, IncrementPrefixPostfix) {
	tree<int, int> tr;
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

TEST(Iterator, DecrementPrefixPostfix)
{
	tree<int, int> tr;
	for (int i = 0; i < 4; ++i) {
		tr[i] = i;
	}
	auto lit = tr.end();
	EXPECT_EQ((--lit)->first, 3);
	EXPECT_EQ(lit->first, 3);
	EXPECT_EQ((lit++)->first, 3);
	EXPECT_EQ(lit, tr.end());
	EXPECT_EQ(lit--, tr.end());
	EXPECT_EQ(lit->first, 3);
	EXPECT_EQ((--lit)->first, 2);
	EXPECT_THROW(--(tr.begin()), std::out_of_range);
	EXPECT_EQ((--lit)->first, 1);
	EXPECT_EQ((lit--)->first, 1);
	EXPECT_EQ(lit->first, 0);
	EXPECT_THROW((lit--), std::out_of_range);
}

TEST(Iterator, RandomDecrementPrefixPostfix)
{
	tree<int, int> tr;
	tr[0] = 0;
	tr[1] = 1;
	tr[-1] = -1;
	tr[2] = 2;
	tr[-2] = -2;
	tr[3] = 3;
	tr[-3] = -3;
	auto lit = tr.end();
	EXPECT_EQ((--lit)->first, 3);
	EXPECT_EQ(lit->first, 3);
	EXPECT_EQ((lit++)->first, 3);
	EXPECT_EQ(lit, tr.end());
	EXPECT_EQ(lit--, tr.end());
	EXPECT_EQ(lit->first, 3);
	EXPECT_EQ((--lit)->first, 2);
	EXPECT_THROW(--(tr.begin()), std::out_of_range);
	EXPECT_EQ((--lit)->first, 1);
	EXPECT_EQ((lit--)->first, 1);
	EXPECT_EQ(lit->first, 0);
	EXPECT_EQ((--lit)->first, -1);
	EXPECT_EQ((lit--)->first, -1);
	EXPECT_EQ(lit->first, -2);
	EXPECT_EQ((--lit)->first, -3);
	EXPECT_THROW((lit--), std::out_of_range);
}

TEST(Iterator_ex, withMapIncrementDecrement) {
	tree<int, int> tr;
	std::map<int, int> map;
	PseudoRand<int> rand(0, 99);
	for (int i = 0; i < 100; ++i) {
		auto r = rand();
		tr[r] = r;
		map[r] = r;
	}
	EXPECT_EQ(tr.size(), map.size());
	EXPECT_TRUE(std::equal(tr.begin(), tr.end(), map.begin(), map.end()));
	{
		auto lit = tr.begin();
		auto mit = map.begin();
		std::advance(lit, 10);
		std::advance(mit, 10);
		for (int j = 0; j < 2; ++j) {
			for (int i = 0; i < (int)tr.size() / 10; ++i) {
				EXPECT_EQ(*(lit++), *(mit++));
			}
			for (int i = 0; i < (int)tr.size() / 10; ++i) {
				EXPECT_EQ(*(lit--), *(mit--));
			}
		}
	}
}
#include "pch.h"
#include "select_sort.hpp"
#include "type.hpp"
#include <random>
#include <vector>
#include <deque>
#include <array>

template<typename T, typename Container = std::vector<T>,
	typename dummy = std::enable_if<std::is_integral_v<T>>>
auto generate_int(T low, T hight, T size)
{
	/*std::random_device rnd_device{};
	std::mt19937 engine{ rnd_device() }; */
	std::mt19937 engine{ 0 };
	std::uniform_int_distribution<T> dist{ low, hight };
	Container vec(size);
	std::generate(begin(vec), end(vec), [&dist, &engine]() { return dist(engine); });
	return vec;
}

TEST(vector, _int)
{
	std::vector<int> v = { 4, 1, -7, 5, 23, 1, 6 };
	select_sort::sort(v.begin(), v.end());
	EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(vector, _int_cmp)
{
	std::vector<int> v = { 4, 1, -7, 5, 23, 1, 6 };
	select_sort::sort(v.begin(), v.end(), [](const int a, const int b) {return a > b; });
	EXPECT_TRUE(std::is_sorted(v.begin(), v.end(), std::greater<>()));
}

TEST(vector, _short_random)
{
	auto v = generate_int<short>(0, 100, 100);
	select_sort::sort(v.begin(), v.end());
	EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(vector, _double)
{
	std::vector<double> v = { 4.55, 1.2, 7.6, 5.888, 23.111, 23.11, 1.201 };
	select_sort::sort(v.begin(), v.end());
	EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(vector, _strings) {
	std::vector <std::string> v = { "abc", "a", "abcabc", "ab", "ab" };
	select_sort::sort(v.begin(), v.end());
	ASSERT_EQ(v[0], "a");
	ASSERT_EQ(v[1], "ab");
	ASSERT_EQ(v[2], "ab");
	ASSERT_EQ(v[3], "abc");
	ASSERT_EQ(v[4], "abcabc");
}

TEST(vector, _type) {
	std::vector<type> v = {
		type(2, "abc"),
		type(3, "abcab"),
		type(1, "abcabc"),
		type(4, "ab")
	};
	select_sort::sort(v.begin(), v.end());
	ASSERT_EQ(v[0].x, 4);
	ASSERT_EQ(v[1].x, 2);
	ASSERT_EQ(v[2].x, 3);
	ASSERT_EQ(v[3].x, 1);
}

TEST(vector, _type_cmp) {
	std::vector<type> v = {
		type(2, "bc"),
		type(3, "cd"),
		type(1, "abcabc"),
		type(4, "d") };
	select_sort::sort(v.begin(), v.end(), type::cmp_first_symbol);
	ASSERT_EQ(v[0].x, 1);
	ASSERT_EQ(v[1].x, 2);
	ASSERT_EQ(v[2].x, 3);
	ASSERT_EQ(v[3].x, 4);
}

TEST(vector, empty)
{
	std::vector<int> v;
	select_sort::sort(v.begin(), v.end());
	EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(arr, _int)
{
	std::array<int, 7> arr = { 4, 1, -7, 5, 23, 1, 6 };
	select_sort::sort(arr.begin(), arr.end());
	EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));
}

TEST(arr, _int_cmp)
{
	std::array<int, 7> arr = { 4, 1, -7, 5, 23, 1, 6 };
	select_sort::sort(arr.begin(), arr.end(), [](const int a, const int b) {return a > b; });
	EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end(), std::greater<>()));
}

TEST(arr, _double)
{
	std::array<double, 7> arr = { 4.55, 1.2, 7.6, 5.888, 23.111, 23.11, 1.201 };
	select_sort::sort(arr.begin(), arr.end());
	EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));
}

TEST(arr, _strings)
{
	std::array <std::string, 5> arr = { "abc", "a", "abcabc", "ab", "ab" };
	select_sort::sort(arr.begin(), arr.end());
	ASSERT_EQ(arr[0], "a");
	ASSERT_EQ(arr[1], "ab");
	ASSERT_EQ(arr[2], "ab");
	ASSERT_EQ(arr[3], "abc");
	ASSERT_EQ(arr[4], "abcabc");
}

TEST(arr, _type)
{
	std::array<type, 4> v = {
		type(2, "abc"),
		type(3, "abcab"),
		type(1, "abcabc"),
		type(4, "ab")
	};
	select_sort::sort(v.begin(), v.end());
	ASSERT_EQ(v[0].x, 4);
	ASSERT_EQ(v[1].x, 2);
	ASSERT_EQ(v[2].x, 3);
	ASSERT_EQ(v[3].x, 1);
}

TEST(arr, _type_cmp)
{
	std::array<type, 4> v = {
		type(2, "bc"),
		type(3, "cd"),
		type(1, "abcabc"),
		type(4, "d")
	};
	select_sort::sort(v.begin(), v.end(), type::cmp_first_symbol);
	ASSERT_EQ(v[0].x, 1);
	ASSERT_EQ(v[1].x, 2);
	ASSERT_EQ(v[2].x, 3);
	ASSERT_EQ(v[3].x, 4);
}

TEST(arr, empty)
{
	std::array<int, 10> arr = {0};
	select_sort::sort(arr.begin(), arr.end());
	EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));
}

TEST(deque, _int)
{
	std::deque<int> dq = { 4, 1, -7, 5, 23, 1, 6 };
	select_sort::sort(dq.begin(), dq.end());
	EXPECT_TRUE(std::is_sorted(dq.begin(), dq.end()));
}

TEST(deque, _int_cmp)
{
	std::deque<int> dq = { 4, 1, -7, 5, 23, 1, 6 };
	select_sort::sort(dq.begin(), dq.end(), [](const int a, const int b) {return a > b; });
	EXPECT_TRUE(std::is_sorted(dq.begin(), dq.end(), std::greater<>()));
}

TEST(deque, _ulong_random)
{
	auto v = generate_int<unsigned long, std::deque<unsigned long>>(0, 1000, 1000);
	select_sort::sort(v.begin(), v.end());
	EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(deque, _double)
{
	std::deque<double> dq = { 4.55, 1.2, 7.6, 5.888, 23.111, 23.11, 1.201 };
	select_sort::sort(dq.begin(), dq.end());
	EXPECT_TRUE(std::is_sorted(dq.begin(), dq.end()));
}

TEST(deque, _strings)
{
	std::deque<std::string> dq = { "abc", "a", "abcabc", "ab", "ab" };
	select_sort::sort(dq.begin(), dq.end());
	ASSERT_EQ(dq[0], "a");
	ASSERT_EQ(dq[1], "ab");
	ASSERT_EQ(dq[2], "ab");
	ASSERT_EQ(dq[3], "abc");
	ASSERT_EQ(dq[4], "abcabc");
}

TEST(deque, _type)
{
	std::deque<type> v = {
		type(2, "abc"),
		type(3, "abcab"),
		type(1, "abcabc"),
		type(4, "ab")
	};
	select_sort::sort(v.begin(), v.end());
	ASSERT_EQ(v[0].x, 4);
	ASSERT_EQ(v[1].x, 2);
	ASSERT_EQ(v[2].x, 3);
	ASSERT_EQ(v[3].x, 1);
}

TEST(deque, _type_cmp)
{
	std::deque<type> v = {
		type(2, "bc"),
		type(3, "cd"),
		type(1, "abcabc"),
		type(4, "d")
	};
	select_sort::sort(v.begin(), v.end(), type::cmp_first_symbol);
	ASSERT_EQ(v[0].x, 1);
	ASSERT_EQ(v[1].x, 2);
	ASSERT_EQ(v[2].x, 3);
	ASSERT_EQ(v[3].x, 4);
}

TEST(deque, empty)
{
	std::deque<int> dq;
	select_sort::sort(dq.begin(), dq.end());
	EXPECT_TRUE(std::is_sorted(dq.begin(), dq.end()));
}
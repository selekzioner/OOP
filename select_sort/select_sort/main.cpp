#include "select_sort.hpp"
#include <iostream>
#include <vector>

bool cmp(int a, int b)
{
	return a < b;
}

int main()
{
	std::vector<int> v = { 4, 1, 7, 5, 23, 1, 6 };
	select_sort::sort(v.begin(), v.end(), cmp);
	std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
	return 0;
}
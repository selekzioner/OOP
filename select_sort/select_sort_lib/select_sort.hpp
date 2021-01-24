#pragma once
#include <algorithm>
#include <functional>

namespace select_sort {
	template<class Iterator>
		using iterator_type = typename std::iterator_traits<Iterator>::iterator_category;

	template<class Iterator>
		using is_based_of_random_access = std::is_base_of<std::random_access_iterator_tag, iterator_type<Iterator>>;


	template <class RandomAccessIterator, class Compare,
		typename std::enable_if<is_based_of_random_access<RandomAccessIterator>::value>::type* = nullptr>
	void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		for (auto i = first; i != last; ++i) {
			auto i_min = i;
			for (auto j = i + 1; j != last; ++j) {
				if (comp(*j, *i_min)) {
					i_min = j;
				}
			}
			std::swap(*i, *i_min);
		}
	}


	template <class RandomAccessIterator>
	void sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		select_sort::sort(first, last, std::less<>());
	}
}
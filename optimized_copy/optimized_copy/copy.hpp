#pragma once
#include <type_traits>

namespace optimized
{	
    template<class Iterator>
    using iterator_value = typename std::iterator_traits<Iterator>::value_type;

    template<class T>
    using vector_iterator = typename std::vector<T>::iterator;

    template<class T>
    using const_vector_iterator = typename std::vector<T>::const_iterator;

    template<typename Iterator1, typename Iterator2>
	inline constexpr bool is_same_it_val = std::is_same_v<iterator_value<Iterator1>, iterator_value <Iterator2>>;
	
    template<typename Iterator>
    inline constexpr bool is_ptr_or_vec_it = std::is_same_v<Iterator,
		vector_iterator<iterator_value<Iterator>>> || std::is_pointer_v<Iterator>;

    template <class InputIterator, class OutputIterator>
    inline constexpr bool is_val_for_opt_copy = std::is_trivially_copyable_v<iterator_value<InputIterator>> &&
        (is_ptr_or_vec_it<InputIterator> || std::is_same_v<const_vector_iterator<iterator_value<InputIterator>>, InputIterator>) &&
        is_ptr_or_vec_it<OutputIterator> && is_same_it_val<InputIterator, OutputIterator>;
	
    template <class InputIterator, class OutputIterator>
    std::enable_if_t<!is_val_for_opt_copy<InputIterator, OutputIterator>, OutputIterator>
    copy(InputIterator first, InputIterator last, OutputIterator result)
    {
        while (first != last) {
            *result = *first;
            ++result; ++first;
        }
        return result;
    }

	template <class InputIterator, class OutputIterator>
    std::enable_if_t<is_val_for_opt_copy<InputIterator, OutputIterator>, OutputIterator>
    copy(InputIterator first, InputIterator last, OutputIterator result)
    {
        auto size = std::distance(first, last) * sizeof(*first);
        std::memmove(&(*result), &(*first), size);
        return result;
    }
}
#pragma once
#include <algorithm>
#include <functional>
#include <array>

template <typename Key,
    typename Value,
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<std::pair<const Key, Value>>>
	class skip_list{
	public:
	
		using key_type = Key;
		using mapped_type = Value;
		using value_type = std::pair<const Key, Value>;
		using size_type = std::size_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
	
	private:
		static const size_type max_level = 3;
		Compare comp_ = Compare();
		Alloc alloc_ = Alloc();

		using value_ptr = std::unique_ptr<value_type, std::function<void(value_type*)>>;
		struct skip_node {
			value_ptr data;
			std::array<std::shared_ptr<skip_node>, max_level + 1> next;
			skip_node() = default;
			explicit skip_node(value_ptr&& data) : data(std::move(data)) {}
		};
	
		using node_ptr = std::shared_ptr<skip_node>;
		node_ptr make_node(key_type&& key, mapped_type&& value) {
			auto ptr = alloc_.allocate(1);
			auto v_ptr = value_ptr(
				new(ptr) value_type(key, value),
				[this](value_type* ptr1) { ptr1->~value_type(); alloc_.deallocate(ptr1, 1); }
			);
			return std::make_shared<skip_node>(std::move(v_ptr));
		}
		
		size_type size_{}; size_type level_{};
		node_ptr header_ = make_node(key_type(), mapped_type());

		template <bool IsConst>
		struct skip_list_iterator {
		protected:
			std::weak_ptr<skip_node> node_;
			friend class skip_list;
		public:
			skip_list_iterator() = default;
			explicit skip_list_iterator(node_ptr& node) { node_ = node; }

			using value_type = std::conditional_t<IsConst, const value_type, value_type>;
			using reference = std::conditional_t<IsConst, const_reference, reference>;
			using pointer = std::conditional_t<IsConst, const_pointer, pointer>;
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using distance_type = difference_type;
			
			skip_list_iterator& operator++()
			{
				node_ptr node = node_.lock();
				if (!node) {
					throw std::out_of_range("skip list iterator out of range");
				}
				node_ = node->next[0];
				return *this;
			}

			skip_list_iterator operator++(int)
			{
				node_ptr node = node_.lock();
				if (!node) {
					throw std::out_of_range("skip list iterator out of range");
				}
				auto tmp = *this;
				node_ = node->next[0];
				return tmp;
			}

			reference operator*() const
			{
				node_ptr node = node_.lock();
				if (!node) {
					throw std::out_of_range("skip list iterator out of range");
				}
				return *(node->data);
			}
			
			pointer operator->() const
			{
				node_ptr node = node_.lock();
				if (!node) {
					throw std::out_of_range("skip list iterator out of range");
				}
				return node->data.get();
			}

			bool operator==(const skip_list_iterator& another) const;
			bool operator!=(const skip_list_iterator& another) const;
		};
	
    public:
        using iterator = skip_list_iterator<false> ;
        using const_iterator = skip_list_iterator<true> ;
        
        skip_list() = default;
        explicit skip_list(const Compare& comp, const Alloc& a = Alloc());
	
        skip_list(const skip_list& another);
		skip_list(skip_list&& another) noexcept;

        skip_list& operator=(const skip_list& another);
		skip_list& operator=(skip_list&& another) noexcept;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

        bool empty() const;
        size_t size() const;

        Value& operator[](const Key& key);
        Value& at(const Key& key);
        const Value& at(const Key& key) const;

        std::pair<iterator, bool> insert(const_reference val);

        void erase(iterator position);
        size_type erase(const Key& key);
        void erase(iterator first, iterator last);

        void swap(skip_list& another) noexcept;
        void clear();

        iterator find(const Key& key);
        const_iterator find(const Key& key) const;
		~skip_list() = default;
};

template <typename Key, typename Value, typename Compare, typename Alloc>
skip_list<Key, Value, Compare, Alloc>::skip_list(const Compare& comp, const Alloc& a)
{
	comp_ = comp; alloc_ = a;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
skip_list<Key, Value, Compare, Alloc>::skip_list(const skip_list& another)
{
	*this = another;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
skip_list<Key, Value, Compare, Alloc>::skip_list(skip_list&& another) noexcept
{
	*this = std::move(another);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
skip_list<Key, Value, Compare, Alloc>& skip_list<Key, Value, Compare, Alloc>::operator=(const skip_list& another)
{
	clear();
	comp_ = another.comp_; alloc_ = another.alloc_;
	for (auto i = another.begin(); i != another.end(); ++i) {
		(*this)[i->first] = i->second;
	}
	return *this;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
skip_list<Key, Value, Compare, Alloc>& skip_list<Key, Value, Compare, Alloc>::operator=(skip_list&& another) noexcept
{
	std::swap(header_, another.header_);
	std::swap(size_, another.size_);
	std::swap(level_, another.level_);
	std::swap(comp_, another.comp_);
	std::swap(alloc_, another.alloc_);
	return *this;
}


template <typename Key, typename Value, typename Compare, typename Alloc>
typename skip_list<Key, Value, Compare, Alloc>::iterator skip_list<Key, Value, Compare, Alloc>::begin()
{
    return iterator(header_->next[0]);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename skip_list<Key, Value, Compare, Alloc>::const_iterator skip_list<Key, Value, Compare, Alloc>::begin() const
{
    return const_iterator(header_->next[0]);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename skip_list<Key, Value, Compare, Alloc>::iterator skip_list<Key, Value, Compare, Alloc>::end()
{
	return iterator();
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename skip_list<Key, Value, Compare, Alloc>::const_iterator skip_list<Key, Value, Compare, Alloc>::end() const
{
	return const_iterator();
}

template <typename Key, typename Value, typename Compare, typename Alloc>
bool skip_list<Key, Value, Compare, Alloc>::empty() const
{
	return size_ == 0;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
size_t skip_list<Key, Value, Compare, Alloc>::size() const
{
	return size_;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
Value& skip_list<Key, Value, Compare, Alloc>::operator[](const Key& key)
{
	auto it = find(key);
	if (it == end()) {
		return insert(std::make_pair(key, mapped_type())).first->second;
	}
	return it->second;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
Value& skip_list<Key, Value, Compare, Alloc>::at(const Key& key)
{
	auto it = find(key);
	if (it == end()) {
		throw std::out_of_range("key not found");
	}
	return it->second;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
const Value& skip_list<Key, Value, Compare, Alloc>::at(const Key& key) const
{
	auto it = find(key);
	if (it == end()) {
		throw std::out_of_range("key not found");
	}
	return it->second;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
std::pair<typename skip_list<Key, Value, Compare, Alloc>::iterator,
	bool> skip_list<Key, Value, Compare, Alloc>::insert(const_reference val)
{
	std::array<node_ptr, max_level + 1> update;
	std::fill(update.begin(), update.end(), header_);
	auto current = header_;
	for (size_type i = 0; i <= level_; ++i) {
		while (current->next[level_ - i] && comp_(current->next[level_ - i]->data->first, val.first)) {
			current = current->next[level_ - i];
		}
		update[level_ - i] = current;
	}
	current = current->next[0];

	if (current && current->data->first == val.first) {
		return std::make_pair(iterator(current), false);
	}
	
	size_type new_level;
	for (new_level = 0; rand() < RAND_MAX / 2 && new_level < max_level; ++new_level) {}
	level_ = (new_level > level_) ? new_level : level_;
	node_ptr new_node = make_node(std::move(key_type(val.first)), std::move(mapped_type(val.second)));

	++size_;
	for (size_type i = 0; i <= new_level; ++i) {
		new_node->next[i] = update[i]->next[i];
		update[i]->next[i] = new_node;
	}
	return std::make_pair(iterator(new_node), true);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
void skip_list<Key, Value, Compare, Alloc>::erase(iterator position)
{
	erase(position->first);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename skip_list<Key, Value, Compare, Alloc>::size_type skip_list<Key, Value, Compare, Alloc>::erase(const Key& key)
{
	std::array<node_ptr, max_level + 1> update;
	std::fill(update.begin(), update.end(), header_);
	auto current = header_;
	for (size_type i = 0; i <= level_; ++i) {
		while (current->next[level_ - i] && comp_(current->next[level_ - i]->data->first, key)) {
			current = current->next[level_ - i];
		}
		update[level_ - i] = current;
	}
	current = current->next[0];
	if (!current || current->data->first != key) {
		return 0;
	}
	for (size_type i = 0; i <= level_; ++i) {
		if (update[i]->next[i] != current) {
			break;
		}
		update[i]->next[i] = current->next[i];
	}
	while (level_ > 0 && header_->next[level_]) {
		--level_;
	}
	--size_;
	return 1;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
void skip_list<Key, Value, Compare, Alloc>::erase(iterator first, iterator last)
{
	for (auto i = first; i != last;) {
		auto current = i++;
		erase(current);
	}
}

template <typename Key, typename Value, typename Compare, typename Alloc>
void skip_list<Key, Value, Compare, Alloc>::clear()
{
	size_ = {}; level_ = {};
	header_ = make_node(key_type(), mapped_type());
}

template <typename Key, typename Value, typename Compare, typename Alloc>
void skip_list<Key, Value, Compare, Alloc>::swap(skip_list& another) noexcept
{
	auto tmp = std::move(*this);
	*this = std::move(another);
	another = std::move(tmp);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename skip_list<Key, Value, Compare, Alloc>::iterator skip_list<Key, Value, Compare, Alloc>::find(const Key& key)
{
	auto current = header_;
	for (size_type i = 0; i <= level_; ++i) {
		while (current->next[level_ - i] && comp_(current->next[level_ - i]->data->first, key)) {
			current = current->next[level_ - i];
		}
	}
	if (current->next[0] && current->next[0]->data->first == key) {
		return iterator(current->next[0]);
	}
	return end();
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename skip_list<Key, Value, Compare, Alloc>::const_iterator skip_list<Key, Value, Compare, Alloc>::find(const Key& key) const
{
	auto current = header_;
	for (size_type i = 0; i <= level_; ++i) {
		while (current->next[level_ - i] && comp_(current->next[level_ - i]->data->first, key)) {
			current = current->next[level_ - i];
		}
	}
	if (current->next[0] && current->next[0]->data->first == key) {
		return const_iterator(current->next[0]);
	}
	return end();
}

template <typename K, typename V, typename C, typename A>
bool operator==(const skip_list<K, V, C, A>& x, const skip_list<K, V, C, A>& y)
{
	if (x.size() != y.size()){
		return false;
	}
	for (auto i = x.begin(), j = y.begin(); i != x.end(); ++i, ++j){
		if (i != j){
			return false;
		}
	}
	return true;
}

template <typename K, typename V, typename C, typename A>
bool operator!=(const skip_list<K, V, C, A>& x, const skip_list<K, V, C, A>& y)
{
	return !(x == y);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
template <bool IsConst>
bool skip_list<Key, Value, Compare, Alloc>::skip_list_iterator<IsConst>::operator!=(const skip_list_iterator& another) const
{
	return !(*this == another);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
template <bool IsConst>
bool skip_list<Key, Value, Compare, Alloc>::skip_list_iterator<IsConst>::operator==(const skip_list_iterator& another) const
{
	node_ptr node = node_.lock(), another_node = another.node_.lock();
	if (node && another_node){
		return node->data->first == another_node->data->first &&
			node->data->second == another_node->data->second && node->next == another_node->next;
	}
	if (!node && !another_node){
		return true;
	}
	return false;
}
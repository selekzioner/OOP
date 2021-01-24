#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <stack>

template <typename Key,
    typename Value,
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<std::pair<const Key, Value>>>
    class tree {
    public:
        typedef std::pair<const Key, Value> value_type;

        using key_type = Key;
        using mapped_type = Value;
        using size_type = std::size_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;

    private:
        using value_ptr = std::unique_ptr<value_type, std::function<void(value_type*)>>;
        struct tree_node {
            value_ptr data;
            std::shared_ptr<tree_node> left, right;
            std::weak_ptr<tree_node> parent;
            explicit tree_node(value_ptr&& pair) : data(std::move(pair)) {}
        };
        using node_ptr = std::shared_ptr<tree_node>;

        node_ptr make_node(key_type&& key, mapped_type&& value) {
            auto ptr = alloc_.allocate(1);
            auto v_ptr = value_ptr(
                new(ptr) value_type(key, value),
                [this](value_type* ptr1) { ptr1->~value_type(); alloc_.deallocate(ptr1, 1); }
            );
            return std::make_shared<tree_node>(std::move(v_ptr));
        }

        node_ptr head_{};
        size_t size_{};
        Compare comp_ = Compare(); Alloc alloc_ = Alloc();

        template <bool IsConst>
        struct tree_iterator {
        protected:
            std::weak_ptr<tree_node> node_{};
            bool end_flag_ {};
            friend class tree;

            void increment() {
                node_ptr node = node_.lock();
                if (end_flag_) {
                    throw std::out_of_range("skip list iterator out of range");
                }
                if (node->right) {
                	if (!node->right->left) {
                        node = node->right;
                	}
                    else {
                        node = node->right;
                        while (node->left) {
                            node = node->left;
                        }
                    }
                }
                else {
	                while (node->parent.lock() && node == node->parent.lock()->right) {
                        node = node->parent.lock();
	                }
                	if (!node->parent.lock()) {
                        end_flag_ = true;
                	}
                    else {
                        node = node->parent.lock();
                    }
                }
                node_ = node;
            }

        	void decrement(){
                node_ptr node = node_.lock();
                if (end_flag_) {
                    while (node->right) {
                        node = node->right;
                    }
                    end_flag_ = false;
                }
                else if (node->left) {
                    if (!node->left->right) {
                        node = node->left;
                    }
                    else {
                        node = node->left;
                        while (node->right) {
                            node = node->right;
                        }
                    }
                }
                else {
                    while (node->parent.lock() && node == node->parent.lock()->left) {
                        node = node->parent.lock();
                    }
                    if (!node->parent.lock()) {
                        throw std::out_of_range("skip list iterator out of range");
                    }
                    node = node->parent.lock();
                }
                node_ = node;
            }
        	
        public:
            tree_iterator() = delete;
            explicit tree_iterator(const node_ptr& node, const bool end_flag = false) {
	            node_ = node; end_flag_ = end_flag;
            }

            using value_type = std::conditional_t<IsConst, const value_type, value_type>;
            using reference = std::conditional_t<IsConst, const_reference, reference>;
            using pointer = std::conditional_t<IsConst, const_pointer, pointer>;
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using distance_type = difference_type;

            tree_iterator& operator++() {
                increment();
                return *this;
            }

            tree_iterator operator++(int) {
                auto tmp = *this;
                increment();
                return tmp;
            }

            tree_iterator& operator--() {
                decrement();
                return *this;
            }

            tree_iterator operator--(int) {
                auto tmp = *this;
                decrement();
                return tmp;
            }

            reference operator*() const {
            	node_ptr node = node_.lock();
                if (end_flag_ || !node) {
                    throw std::out_of_range("skip list iterator out of range");
                }
                return *(node->data);
            }
        	
            pointer operator->() const {
                node_ptr node = node_.lock();
                if (end_flag_ || !node) {
                    throw std::out_of_range("skip list iterator out of range");
                }
                return node->data.get();
            }

            bool operator==(const tree_iterator& another) const {
	            return end_flag_ == another.end_flag_ && node_.lock() == another.node_.lock();
            }
        	
            bool operator!=(const tree_iterator& another) const {
	            return !(*this == another);
            }
        };

    public:
        using iterator = tree_iterator<false>;
        using const_iterator = tree_iterator<true>;

        tree() = default;
        explicit tree(const Compare& comp, const Alloc& a = Alloc());
        tree(const tree& another);
        tree(tree&& another) noexcept;

        tree& operator=(const tree& another);
        tree& operator=(tree&& another) noexcept;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

        bool empty() const;
        size_type size() const;

        mapped_type& operator[](const key_type& key);
        mapped_type& at(const key_type& key);
        const mapped_type& at(const key_type& key) const;

        std::pair<iterator, bool> insert(const value_type&);

        void erase(iterator position);
        size_type erase(const key_type& key);
        void erase(iterator first, iterator last);

        void swap(tree& another) noexcept;
        void clear();

        iterator find(const key_type& key);
        const_iterator find(const key_type& key) const;
        ~tree();
};

template <typename Key, typename Value, typename Compare, typename Alloc>
tree<Key, Value, Compare, Alloc>::tree(const Compare& comp, const Alloc& a) {
    comp_ = comp; alloc_ = a;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
tree<Key, Value, Compare, Alloc>::tree(const tree& another) {
    *this = another;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
tree<Key, Value, Compare, Alloc>::tree(tree&& another) noexcept {
    *this = std::move(another);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
tree<Key, Value, Compare, Alloc>& tree<Key, Value, Compare, Alloc>::operator=(const tree& another) {
    clear();
    comp_ = another.comp_; alloc_ = another.alloc_;
    for (auto i = another.begin(); i != another.end(); ++i) {
        (*this)[i->first] = i->second;
    }
    return *this;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
tree<Key, Value, Compare, Alloc>& tree<Key, Value, Compare, Alloc>::operator=(tree&& another) noexcept {
    std::swap(head_, another.head_);
    std::swap(size_, another.size_);
    std::swap(comp_, another.comp_);
    std::swap(alloc_, another.alloc_);
    return *this;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename tree<Key, Value, Compare, Alloc>::iterator tree<Key, Value, Compare, Alloc>::begin() {
    auto current = head_;
    if (!current) {
        return end();
    }
    while (current->left) {
        current = current->left;
    }
    return iterator(current);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename tree<Key, Value, Compare, Alloc>::const_iterator tree<Key, Value, Compare, Alloc>::begin() const {
    auto current = head_;
    if (!current) {
        return end();
    }
    while (current->left) {
        current = current->left;
    }
    return const_iterator(current);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename tree<Key, Value, Compare, Alloc>::iterator tree<Key, Value, Compare, Alloc>::end() {
    return iterator(head_, true);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename tree<Key, Value, Compare, Alloc>::const_iterator tree<Key, Value, Compare, Alloc>::end() const {
    return const_iterator(head_, true);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
bool tree<Key, Value, Compare, Alloc>::empty() const {
    return size_ == 0;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename tree<Key, Value, Compare, Alloc>::size_type tree<Key, Value, Compare, Alloc>::size() const {
    return size_;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename tree<Key, Value, Compare, Alloc>::mapped_type& tree<Key, Value, Compare, Alloc>::operator[](const key_type& key) {
    auto it = find(key);
    if (it == end()) {
        return insert(std::make_pair(key, mapped_type())).first->second;
    }
    return it->second;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename tree<Key, Value, Compare, Alloc>::mapped_type& tree<Key, Value, Compare, Alloc>::at(const key_type& key) {
    auto it = find(key);
    if (it == end()) {
        throw std::out_of_range("key not found");
    }
    return it->second;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
const typename tree<Key, Value, Compare, Alloc>::mapped_type& tree<Key, Value, Compare, Alloc>::at(const key_type& key) const {
    auto it = find(key);
    if (it == end()) {
        throw std::out_of_range("key not found");
    }
    return it->second;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
std::pair<typename tree<Key, Value, Compare, Alloc>::iterator, bool>
tree<Key, Value, Compare, Alloc>::insert(const value_type& val) {
    auto new_node = make_node(std::move(key_type(val.first)), std::move(mapped_type(val.second)));
    if (!head_) {
        head_ = new_node;
        ++size_;
        return std::make_pair(iterator(head_), true);
    }
    auto current = head_;
    node_ptr prev;

    while (current) {
        prev = current;
        if (comp_(val.first, current->data->first)) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    if (!prev) {
        prev = new_node;
    }
    else if (val.first == prev->data->first) {
        return std::make_pair(iterator(prev), false);
    }
    else if (comp_(val.first, prev->data->first)) {
        prev->left = new_node;
    }
    else {
        prev->right = new_node;
    }
    new_node->parent = prev;
    ++size_;
    return std::make_pair(iterator(new_node), true);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename tree<Key, Value, Compare, Alloc>::size_type tree<Key, Value, Compare, Alloc>::erase(const key_type& key) {
    auto it = find(key);
    if (it == end()) {
        return 0;
    }
    auto node = it.node_.lock();
    auto cond = -1;
    if (!node->parent.lock()) {
        cond = 0;
    }
    else if (node->parent.lock()->right == node) {
        cond = 1;
    }
    if (node->right) {
        auto leftmost_of_right = node->right;
        while (leftmost_of_right->left) {
            leftmost_of_right = leftmost_of_right->left;
        }

        if (node->left) {
            node->left->parent = leftmost_of_right;
        }
        leftmost_of_right->left = node->left;

        if (cond != 0) {
            node->right->parent = node->parent;
            if (cond == -1) {
                node->parent.lock()->left = node->right;
            }
            else {
                node->parent.lock()->right = node->right;
            }
        }
        else {
            head_ = node->right;
        }
    }
    else if (node->left) {
        if (cond != 0) {
            node->left->parent = node->parent;
            if (cond == -1) {
                node->parent.lock()->left = node->left;
            }
            else {
                node->parent.lock()->right = node->left;
            }
        }
        else {
            head_ = node->left;
        }
    }
    else {
        if (cond == -1) {
            node->parent.lock()->left = nullptr;
        }
        else if (cond == 1) {
            node->parent.lock()->right = nullptr;
        }
        else {
            head_ = nullptr;
        }
    }
    --size_;
    return 1;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
void tree<Key, Value, Compare, Alloc>::erase(iterator position) {
    if (position == end()) {
        return;
    }
    erase(position->first);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
void tree<Key, Value, Compare, Alloc>::erase(iterator first, iterator last) {
    for (auto i = first; i != last;) {
        auto next = ++i;
        erase(i);
        i = next;
    }
}

template <typename Key, typename Value, typename Compare, typename Alloc>
void tree<Key, Value, Compare, Alloc>::clear() {
	size_ = {};
    head_ = make_node(key_type(), mapped_type());
    head_ = nullptr;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
void tree<Key, Value, Compare, Alloc>::swap(tree& another) noexcept {
    auto tmp = std::move(*this);
    *this = std::move(another);
    another = std::move(tmp);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename tree<Key, Value, Compare, Alloc>::iterator tree<Key, Value, Compare, Alloc>::find(const key_type& key) {
    auto current = head_;
    if (!current) {
        return end();
    }
    while (current->data->first != key) {
        if (comp_(key, current->data->first)) {
            current = current->left;
        }
        else if (!comp_(key, current->data->first)) {
            current = current->right;
        }
        if (!current) {
            return end();
        }
    }
    return iterator(current);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
typename tree<Key, Value, Compare, Alloc>::const_iterator tree<Key, Value, Compare, Alloc>::find(const key_type& key) const {
    auto current = head_;
    if (!current) {
        return end();
    }
    while (current->data->first != key) {
        if (comp_(key, current->data->first)) {
            current = current->left;
        }
        else if (!comp_(key, current->data->first)) {
            current = current->right;
        }
        if (!current) {
            return end();
        }
    }
    return const_iterator(current);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
tree<Key, Value, Compare, Alloc>::~tree() {
    clear();
}

template <typename K, typename V, typename C, typename A>
bool operator==(const tree<K, V, C, A>& x, const tree<K, V, C, A>& y) {
    if (x.size() != y.size()) {
        return false;
    }
    for (auto i = x.begin(), j = y.begin(); i != x.end(); ++i, ++j) {
        if (*i != *j) {
            return false;
        }
    }
    return true;
}

template <typename K, typename V, typename C, typename A>
bool operator!=(const tree<K, V, C, A>& x, const tree<K, V, C, A>& y) {
    return !(x == y);
}
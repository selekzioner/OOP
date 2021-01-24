#include "gameboard.hpp"
#include <cstdlib>
#include <ctime>
#include <stack>

gameboard::gameboard(const int& size, const int& bombs_count, const int& cell_size)
{
	size_ = size;
	bombs_count_ = bombs_count;
	empty_ = true;
	need_to_open_ = size_ * size_ - bombs_count_;
	condition_ = board_condition::in_process;
	cell_size_ = cell_size;
	flags_count_ = bombs_count_;
	bang_vector_ = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };

	cells_.resize(size_);
	bombs_around_.resize(size_);
	for (auto i = 0; i < size_; ++i) {
		cells_[i].resize(size_);
		bombs_around_[i].resize(size_);
		bombs_around_[i].assign(size_, 0);
	}
}

void gameboard::set_bombs(const sf::Vector2i& v)
{
	srand(time((nullptr)));
	empty_ = false;
	for (auto i = 0; i < bombs_count_; ++i) {
		auto new_x = rand() % size_;
		auto new_y = rand() % size_;
		while ((new_x == v.x || new_x == v.x + 1 || new_x == v.x - 1)
			&& (new_y == v.y || new_y == v.y + 1 || new_y == v.y - 1)
			|| cells_[new_x][new_y].check_bomb()) {
			new_x = rand() % size_;
			new_y = rand() % size_;
		}
		cells_[new_x][new_y].set_bomb();
	}
	for (auto i = 0; i < gameboard::size_; ++i) {
		for (auto j = 0; j < gameboard::size_; ++j) {
			count_bombs_around(sf::Vector2i(i, j));
		}
	}
}

void gameboard::open_cell(const sf::Vector2i& v)
{
	if (empty_ && cells_[v.x][v.y].condition() == cell::cell_condition::flag) {
		return;
	}
	else if (empty_) {
		set_bombs(v);
	}
	if (!cells_[v.x][v.y].check_bomb() && !empty_) {
		open_empty_cell(v);
	}
	const auto check = cells_[v.x][v.y].open();
	if (check == 1) {
		--need_to_open_;
		if (!need_to_open_) {
			condition_ = board_condition::win;
		}
	}
	else if (check == 0) {
		need_to_open_ = 0;
		condition_ = board_condition::losing;
	}
}

void gameboard::flag(const sf::Vector2i& v)
{
	const auto check = cells_[v.x][v.y].flag();
	if (check == 1) {
		--flags_count_;
		if (flags_count_ < 0) {
			cells_[v.x][v.y].flag();
			++flags_count_;
		}
	}
	else if (check == 0) {
		++flags_count_;
	}
}

void gameboard::draw(sf::RenderWindow& app, const image& img) 
{
	sf::Sprite s;
	app.clear(sf::Color(219, 219, 219));

	for (auto i = 0; i < size_; ++i) {
		for (auto j = 0; j < size_; ++j) {
			auto v = sf::Vector2i(i, j);
			const auto condition = cells_[i][j].condition();
			std::string str;

			if (condition == cell::cell_condition::close) {
				str = "close";
			}
			else if (condition == cell::cell_condition::open) {
				if (cells_[i][j].check_bomb()) {
					if (bang_vector_.x >= size_) {
						bang_vector_ = v;
					}
					else {
						v = bang_vector_;
					}
					str = "red_bomb";
				}
				else {
					str = std::to_string(bombs_around_[i][j]);
				}
			}
			else if (condition == cell::cell_condition::flag) {
				str = "flag";
			}
			img.set_image(s, sf::Vector2i(v.x * cell_size_, v.y * cell_size_), cell_size_, str);
			app.draw(s);
		}
	}
	if (bang_vector_.x < size_) {
		for (auto i = 0; i < size_; ++i) {
			for (auto j = 0; j < size_; ++j) {
				const auto v = sf::Vector2i(i, j);
				if (cells_[i][j].check_bomb() 
					&& !(i == bang_vector_.x && j == bang_vector_.y)) {
					img.set_image(s, sf::Vector2i(v.x * cell_size_, v.y * cell_size_), cell_size_, "bomb");
					open_cell(v);
					app.draw(s);
				}
			}
		}
	}
}

void gameboard::clear()
{
	empty_ = true;
	need_to_open_ = size_ * size_ - bombs_count_;
	flags_count_ = bombs_count_;
	condition_ = board_condition::in_process;
	for (auto i = 0; i < size_; ++i) {
		bombs_around_[i].assign(size_, 0);
		for (auto j = 0; j < size_; ++j) {
			cells_[i][j].clear();
		}
	}
	bang_vector_ = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
}

int gameboard::size() const
{
	return size_;
}

bool gameboard::empty() const
{
	return empty_;
}

int gameboard::cell_size() const
{
	return cell_size_;
}

int gameboard::need_to_open() const
{
	return need_to_open_;
}

int gameboard::flags_count() const
{
	return flags_count_;
}

gameboard::board_condition gameboard::condition() const 
{
	return condition_;
}

void gameboard::open_empty_cell(const sf::Vector2i& v)
{
	if (empty_ || bombs_around_[v.x][v.y] != 0) {
		return;
	}

	std::stack<std::pair<int, int>> stack;
	stack.push(std::make_pair(v.x, v.y));

	while (!stack.empty()) {
		auto [new_x, new_y] = stack.top();
		stack.pop();
		if (cells_[new_x][new_y].open() == 1) {
			--need_to_open_;
		}

		const auto x_min = (new_x == 0 ? new_x : new_x - 1);
		const auto x_max = (new_x + 1 >= size_ ? new_x : new_x + 1);
		const auto y_min = (new_y == 0 ? new_y : new_y - 1);
		const auto y_max = (new_y + 1 >= size_ ? new_y : new_y + 1);

		for (auto i = x_min; i <= x_max; ++i) {
			for (auto j = y_min; j <= y_max; ++j) {
				if (bombs_around_[i][j] != 0 && !cells_[i][j].check_bomb()
					&& cells_[i][j].condition() == cell::cell_condition::close) {
					if (cells_[i][j].open() == 1) {
						--need_to_open_;
					}
				}
				if (!cells_[new_x][new_y].check_bomb() && bombs_around_[i][j] == 0
					&& cells_[i][j].condition() == cell::cell_condition::close) {
					stack.push(std::make_pair(i, j));
				}
			}
		}
	}
}

void gameboard::count_bombs_around(const sf::Vector2i& v)
{
	const auto x_min = (v.x == 0 ? v.x : v.x - 1);
	const auto x_max = (v.x + 1 >= size_ ? v.x : v.x + 1);
	const auto y_min = (v.y == 0 ? v.y : v.y - 1);
	const auto y_max = (v.y + 1 >= size_ ? v.y : v.y + 1);

	for (auto i = x_min; i <= x_max; ++i) {
		for (auto j = y_min; j <= y_max; ++j) {
			if (!(i == v.x && j == v.y) && cells_[i][j].check_bomb()) {
				++bombs_around_[v.x][v.y];
			}
		}
	}
}
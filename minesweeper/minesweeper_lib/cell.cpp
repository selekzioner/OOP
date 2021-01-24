#include "cell.hpp"

cell::cell()
{
	bomb_ = false;
	condition_ = cell_condition::close;
}

void cell::set_bomb()
{
	bomb_ = true;
}

int cell::open()
{
	if (condition_ == cell_condition::flag){
		return -1;
	}
	if (bomb_ == true){
		condition_ = cell_condition::open;
		return 0;
	}
	if (condition_ != cell_condition::open) {
		condition_ = cell_condition::open;
		return 1;
	}
	return -1;
}

int cell::flag()
{
	if (condition_ == cell_condition::flag) {
		condition_ = cell_condition::close;
		return 0;
	}
	if (condition_ != cell_condition::open) {
		condition_ = cell_condition::flag;
		return 1;
	}
	return -1;
}

void cell::clear()
{
	bomb_ = false;
	condition_ = cell_condition::close;
}

cell::cell_condition cell::condition() const
{
	return condition_;
}

bool cell::check_bomb() const
{
	return bomb_;
}
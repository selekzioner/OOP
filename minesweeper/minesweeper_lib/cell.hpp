#pragma once

class cell final
{
public:
	cell();

	void set_bomb();
	int open();
	int flag();

	void clear();

	enum class cell_condition { close, open, flag };

	cell_condition condition() const;
	bool check_bomb() const;

	~cell() = default;

private:
	bool bomb_;
	cell_condition condition_;
};
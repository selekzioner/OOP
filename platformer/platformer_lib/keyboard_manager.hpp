#pragma once
#include <vector>

enum class key_id { left, right, up, no_moving };

class keyboard_manager final
{
public:
	keyboard_manager() = delete;

	static std::vector<key_id> get_key();
};


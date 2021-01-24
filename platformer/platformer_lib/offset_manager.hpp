#pragma once
#include "game_hero.hpp"

class offset_manager final
{
public:
	offset_manager() = delete;
	explicit offset_manager(std::shared_ptr<game_hero>& hero);
	
	sf::Vector2f compute_offset();
	sf::Vector2f get_last_offset() const;

private:
	std::shared_ptr<game_hero>& hero_;

	sf::Vector2f init_pos_;
	sf::Vector2f offset_{ 0, 0 };
};


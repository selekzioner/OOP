#pragma once
#include "tile_map.hpp"
#include "game_hero.hpp"

class interaction_manager final
{
public:
	interaction_manager() = delete;

	static void check_collision(std::shared_ptr<game_hero>& hero, tile_map& map, const sf::Vector2f& off_t);
	
private:
	static void block_collision(std::shared_ptr<game_hero>& hero, tile_map& map, const sf::Vector2f& off_t);
	static void enemy_collision();
};


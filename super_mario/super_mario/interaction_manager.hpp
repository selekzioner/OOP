#pragma once
#include "tile_map.hpp"
#include "game_object.hpp"

class interaction_manager final
{
public:
	interaction_manager() = delete;

	static void block_collision(std::shared_ptr<game_object>& object, tile_map& map);
	//static void enemy_interaction(player& m, tile_map& map);
private:
	enum class axis { x, y };
	
	static unsigned from_pixels_to_blocks(float pos, axis ax);
	static float from_blocks_to_pixels(unsigned pos, axis ax);
};


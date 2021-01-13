#include "interaction_manager.hpp"
#include "game_specifications.hpp"


void interaction_manager::block_collision(std::shared_ptr<game_object>& object, tile_map& map)
{
	const auto bounds = object->get_bounds();
	
	const auto left = from_pixels_to_blocks(bounds.left, axis::x),
			  right = from_pixels_to_blocks(bounds.width + bounds.left, axis::x),
				top = from_pixels_to_blocks(bounds.top, axis::y),
			 bottom = from_pixels_to_blocks(bounds.height + bounds.top, axis::y);

	for (auto x = left; x < right; ++x) {
		for (auto y = top; y < bottom; ++y) {
			if (map.get_tile_id(sf::Vector2u(x, y)) != tile_map::tile_id::empty) {
				const auto posx = from_blocks_to_pixels(x, axis::x),
						   posy = from_blocks_to_pixels(y, axis::y);
				object->collision();
			}
		}
	}
}


unsigned interaction_manager::from_pixels_to_blocks(const float pos, const axis ax)
{
	unsigned converted = 0;
	switch (ax) {
	case axis::x:
		converted = static_cast<unsigned>(pos / block_scale.x);
		break;
	case axis::y:
		converted = static_cast<unsigned>(pos / block_scale.y);
	}

	return converted;
}


float interaction_manager::from_blocks_to_pixels(const unsigned pos, const axis ax)
{
	float converted = 0;
	switch (ax) {
	case axis::x:
		converted = static_cast<float>(pos) * block_scale.x;
		break;
	case axis::y:
		converted = static_cast<float>(pos) * block_scale.y;
	}

	return converted;
}
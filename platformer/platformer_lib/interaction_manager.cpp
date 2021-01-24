#include "interaction_manager.hpp"
#include "game_specifications.hpp"


void interaction_manager::check_collision(std::shared_ptr<game_hero>& hero, tile_map& map, const sf::Vector2f& off_t)
{
	block_collision(hero, map, off_t);
}


void interaction_manager::block_collision(std::shared_ptr<game_hero>& hero, tile_map& map, const sf::Vector2f& off_t)
{
	auto bounds = hero->get_bounds();

	const auto left = tile_map::from_pixels_to_blocks(bounds.left + off_t.x, tile_map::axis::x),
			  right = tile_map::from_pixels_to_blocks(bounds.width + bounds.left + off_t.x, tile_map::axis::x),
				top = tile_map::from_pixels_to_blocks(bounds.top, tile_map::axis::y),
			 bottom = tile_map::from_pixels_to_blocks(bounds.height + bounds.top, tile_map::axis::y);

	auto on_surface = 0;
	for (auto x = left; x <= right; ++x) {
		for (auto y = top; y <= bottom; ++y) {
			auto id = map.get_tile_id(sf::Vector2u(x, y));
			switch (id) {
			case tile_map::tile_id::block:
				hero->collision();
				on_surface += (y == bottom);
				break;
			case tile_map::tile_id::coin:
				if (hero->get_id() == hero_id::player) {
					map.collect_coin({ x, y });
				}
				break;
			case tile_map::tile_id::out_of_range:
				hero->take_life();
				break;
			}
		}
	}
	if (on_surface == 0) {
		hero->fall();
	}
}
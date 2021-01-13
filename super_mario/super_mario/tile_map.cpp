#include "tile_map.hpp"
#include "game_specifications.hpp"
#include <fstream>


block::block(const sf::Texture& texture)
{
	sprite_.setTexture(texture);
}


void block::set_position(const sf::Vector2f& pos)
{
	sprite_.setPosition(pos);
}


void block::draw(sf::RenderWindow& window)
{
	window.draw(sprite_);
}


tile_map::tile_map(std::ifstream& instream, texture_loader& loader) : loader_(loader)
{
	for (std::string line; std::getline(instream, line); ) {
		map_.push_back(line);
	}

	block_ptr_ = std::make_unique<block>(loader_[texture_id::brick]);
}


tile_map::tile_id tile_map::get_tile_id(const sf::Vector2u& pos)
{
	if (map_[pos.y][pos.x] == 'B') {
		return tile_id::block;
	}
	return tile_id::empty;
}


void tile_map::draw(sf::RenderWindow& render_window)
{
	for (size_t y = 0; y < map_.size(); ++y) {
		for (size_t x = 0; x < map_[y].size(); ++x) {
			if (map_[y][x] == 'B') {
				
				block_ptr_->set_position(sf::Vector2f(
					block_scale.x * x, block_scale.y * y));
				block_ptr_->draw(render_window);
			}
		}
	}
}

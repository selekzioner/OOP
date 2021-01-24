#include "tile_map.hpp"
#include "game_specifications.hpp"
#include <fstream>


tile_map::tile_map(std::ifstream& instream, texture_loader& loader)
{
	for (std::string line; std::getline(instream, line); ) {
		map_.push_back(line);
		for (auto& i : line) {
			coins_count_ += i == '0' ? 1 : 0;
		}
	}

	block_ptr_ = std::make_unique<block>(loader[texture_id::block], block_scale);
	coin_ptr_ = std::make_unique<coin>(loader[texture_id::coin], block_scale);
	pipe_ptr_ = std::make_unique<pipe>(loader[texture_id::pipe], sf::Vector2f{ block_scale.x, block_scale.y * 2 });
}


tile_map::tile_id tile_map::get_tile_id(const sf::Vector2u& pos) const
{
	if (pos.y >= map_.size() || pos.x >= map_.front().size()) {
		return tile_id::out_of_range;
	}
	if (map_.at(pos.y).at(pos.x) == 'B' || map_.at(pos.y).at(pos.x) == 'P') {
		return tile_id::block;
	}
	if (map_.at(pos.y).at(pos.x) == '0') {
		return tile_id::coin;
	}
	return tile_id::empty;
}


unsigned tile_map::get_coin_count() const
{
	return coins_count_;
}


void tile_map::draw(sf::RenderWindow& render_window, const sf::Vector2f& off_t, const sf::Vector2f& current_pos)
{
	const auto begin = current_pos.x >
		screen_scale.x / 2 ? current_pos.x - screen_scale.x / 2 : 0;
	const auto end = off_t.x != 0 ? current_pos.x + screen_scale.x / 2 : screen_scale.x;
	
	const auto left = from_pixels_to_blocks(begin + off_t.x, axis::x);
	auto right = from_pixels_to_blocks(end + off_t.x, axis::x);
	
	right = right < map_.front().size() ? right : map_.front().size() - 1;
	
	for (size_t y = 0; y < map_.size(); ++y) {
		for (auto x = left; x <= right; ++x) {
			if (map_[y][x] == 'B') {
				block_ptr_->set_position(sf::Vector2f(
					block_scale.x * x - off_t.x, block_scale.y * y));
				block_ptr_->draw(render_window);
			}
			if (map_[y][x] == '0') {
				coin_ptr_->set_position(sf::Vector2f(
					block_scale.x * x - off_t.x, block_scale.y * y));
				coin_ptr_->draw(render_window);
			}
			if (map_[y][x] == 'P' && map_[y + 1][x] == 'P') {
				pipe_ptr_->set_position(sf::Vector2f(
					block_scale.x * x - off_t.x, block_scale.y * y));
				pipe_ptr_->draw(render_window);
			}
		}
	}
}


void tile_map::collect_coin(const sf::Vector2u& pos)
{
	map_[pos.y][pos.x] = ' ';
	--coins_count_;
}


unsigned tile_map::from_pixels_to_blocks(const float pos, const axis ax)
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
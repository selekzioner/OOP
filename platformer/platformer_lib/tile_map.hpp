#pragma once
#include <vector>
#include <string>
#include "drawable.hpp"
#include "texture_loader.hpp"

class block final : public drawable
{
public:
	block() = delete;
	explicit block(const sf::Texture& texture, const sf::Vector2f& scale) : drawable(texture, scale) {}
};


class coin final : public drawable
{
public:
	coin() = delete;
	explicit coin(const sf::Texture& texture, const sf::Vector2f& scale) : drawable(texture, scale) {}
};


class pipe final : public drawable
{
public:
	pipe() = delete;
	explicit pipe(const sf::Texture& texture, const sf::Vector2f& scale) : drawable(texture, scale) {}
};


class tile_map final
{
public:
	enum class tile_id { block, coin, empty, out_of_range };
	enum class axis { x, y };
	
	tile_map() = delete;
	explicit tile_map(std::ifstream& instream, texture_loader& loader);
	
	void draw(sf::RenderWindow& render_window, const sf::Vector2f& off_t, const sf::Vector2f& current_pos);

	void collect_coin(const sf::Vector2u& pos);

	tile_id get_tile_id(const sf::Vector2u& pos) const;
	unsigned get_coin_count() const;
	
	static unsigned from_pixels_to_blocks(float pos, axis ax);
	
private:
	std::vector<std::string> map_;
	unsigned coins_count_ = 0;

	std::unique_ptr<block> block_ptr_;
	std::unique_ptr<coin> coin_ptr_;
	std::unique_ptr<pipe> pipe_ptr_;
};
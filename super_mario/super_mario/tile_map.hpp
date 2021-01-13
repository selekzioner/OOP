#pragma once
#include <vector>
#include <string>
#include "texture_loader.hpp"

enum class block_id { brick, stone };

class block
{
public:
	block() = delete;
	explicit block(const sf::Texture& texture);

	void set_position(const sf::Vector2f& pos);
	void draw(sf::RenderWindow& window);
private:
	sf::Sprite sprite_;
};


class tile_map final
{
public:
	enum class tile_id { block, empty };
	
	tile_map() = delete;
	explicit tile_map(std::ifstream& instream, texture_loader& loader);

	tile_id get_tile_id(const sf::Vector2u& pos);
	
	void draw(sf::RenderWindow& render_window);
private:
	using block_ptr = std::shared_ptr<block>;
	
	texture_loader& loader_;
	std::vector<std::string> map_;
	//std::vector<std::vector<block_ptr> blocks_;

	std::unique_ptr<block> block_ptr_;
};
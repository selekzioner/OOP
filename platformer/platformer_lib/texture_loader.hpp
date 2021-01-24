#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include "game_hero.hpp"

enum class texture_id
{
	player = static_cast<int>(hero_id::player),
	mushroom = static_cast<int>(hero_id::mushroom),
	turtle = static_cast<int>(hero_id::turtle),
	coin,
	block,
	pipe,
	win,
	game_over
};


class texture_loader final
{
public:
	using texture = sf::Texture;
	
	texture_loader() = default;
	
	void load();
	texture& operator[](const texture_id& id);
	
private:
	std::map<texture_id, texture> textures_;
};


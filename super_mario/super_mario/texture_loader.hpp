#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include "game_object.hpp"

enum class texture_id
{
	player = static_cast<int>(object_id::player),
	coin = static_cast<int>(object_id::coin),
	mushroom = static_cast<int>(object_id::mushroom),
	beetle = static_cast<int>(object_id::beetle),
	brick,
	stone,
	tree,
	background
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


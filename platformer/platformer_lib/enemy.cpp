#include "enemy.hpp"
#include "game_specifications.hpp"
#include <random>


enemy::enemy(const sf::Texture& texture, const sf::Vector2f& scale) : game_hero(texture, scale)
{
	
}


void enemy::update(const sf::Event&)
{
	move_left();
	jump_processing();
}


mushroom::mushroom(const sf::Texture& texture, const sf::Vector2f& scale) : enemy(texture, scale)
{
	id_ = hero_id::mushroom;
}


turtle::turtle(const sf::Texture& texture, const sf::Vector2f& scale) : enemy(texture, scale)
{
	id_ = hero_id::turtle;
}
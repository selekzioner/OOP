#pragma once
#include "game_hero.hpp"

class player final : public game_hero
{
public:
	player() = delete;
	explicit player(const sf::Texture& texture, const sf::Vector2f& scale);

	void update(const sf::Event& event) override;
};
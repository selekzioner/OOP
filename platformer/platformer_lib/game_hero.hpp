#pragma once
#include <SFML/Graphics.hpp>
#include "drawable.hpp"

enum class hero_id
{
	player,
	mushroom,
	turtle
};


class game_hero : public drawable
{
public:
	game_hero() = delete;
	explicit game_hero(const sf::Texture& texture, const sf::Vector2f& scale);

	void collision();
	void fall();
	void take_life();
	
	virtual void update(const sf::Event&) = 0;

	hero_id get_id() const;
	unsigned lives_count() const;

protected:
	hero_id id_{};
	unsigned lives_ = 1;

	float off_t_x_{}, off_t_y_{}, last_off_t_x_{}, last_off_t_y_{};

	enum class state { standing, jumping, falling };
	enum class direction { left, right };

	void move_left();
	void move_right();
	void move_up();
	
	void set_direction(direction dir);

	state state_ = state::standing;
	direction direction_ = direction::right;

	float jump_height_ = 0;
	float last_surface_height_ = 0;
	void jump_processing();
};
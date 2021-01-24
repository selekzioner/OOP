#include "game_hero.hpp"
#include "game_specifications.hpp"


game_hero::game_hero(const sf::Texture& texture, const sf::Vector2f& scale) : drawable(texture, scale) {}


void game_hero::collision()
{	
	if (last_off_t_y_ > 0) {
		sprite_.move(0, -last_off_t_y_);
		state_ = state::standing;
	}
	if (last_off_t_y_ < 0) {
		sprite_.move(0, -last_off_t_y_);
		state_ = state::falling;
	}
	if (last_off_t_x_ != 0) {
		sprite_.move(-last_off_t_x_, 0);
	}
	last_off_t_x_ = 0; last_off_t_y_ = 0;
}


void game_hero::fall()
{
	if (state_ == state::standing) {
		state_ = state::falling;
	}
}


void game_hero::take_life()
{
	lives_ > 0 ? --lives_ : 0;
}


hero_id game_hero::get_id() const
{
	return id_;
}


unsigned game_hero::lives_count() const
{
	return lives_;
}


void game_hero::move_right()
{
	set_direction(direction::right);
	const auto dx = sprite_.getPosition().x <
			screen_scale.x - sprite_.getTextureRect().width ? off_t_x_ : 0;
	sprite_.move(dx, 0);
	last_off_t_x_ = dx;
}


void game_hero::move_left()
{
	set_direction(direction::left);
	const auto dx = sprite_.getPosition().x >
			sprite_.getTextureRect().width ? -off_t_x_ : 0;
	sprite_.move(dx, 0);
	last_off_t_x_ = dx;
}


void game_hero::move_up()
{
	float dy = 0;
	if (state_ == state::standing) {
		dy = sprite_.getPosition().y > 0 ? -off_t_y_ : 0;
		state_ = state::jumping;
		last_surface_height_ = sprite_.getPosition().y;
		sprite_.move(0, dy);
	}
	last_off_t_y_ = dy;
}


void game_hero::set_direction(const direction dir)
{
	if (dir != direction_) {
		direction_ = dir;
		sprite_.scale(-1, 1);
		if (dir == direction::left) {
			sprite_.move(scale_.x, 0);
		}
		else {
			sprite_.move(-scale_.x, 0);
		}
	}
}


void game_hero::jump_processing()
{
	float dy = 0;
	switch (state_) {
	case state::jumping:
		dy = -off_t_y_;
		if (last_surface_height_ - sprite_.getPosition().y >= jump_height_ 
							|| sprite_.getPosition().y <= block_scale.y / 32) {
			state_ = state::falling;
		}
		break;
	case state::falling:
		dy = off_t_y_;
		break;
	case state::standing:
		return;
	}

	sprite_.move(0, dy);
	last_off_t_y_ = dy;
}

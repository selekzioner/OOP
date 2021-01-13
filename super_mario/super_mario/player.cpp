#include "player.hpp"
#include "keyboard_manager.hpp"
#include "tile_map.hpp"


player::player(const sf::Texture& texture) : game_object(texture)
{
	type_ = object_id::player;
	sprite_scale_ = sf::Vector2f(32, 60);
	set_state(state::standing);
	off_t_x_ = screen_scale.x / 1500; off_t_y_ = screen_scale.y / 1500;
	sprite_.setPosition(screen_scale.x / 2, screen_scale.y / 2);
}


void player::update(const sf::Event&)
{
	jump_processing();
	
	auto [x, y] = sprite_.getPosition();
	float dx = 0, dy = 0;
	
	switch (keyboard_manager::get_key()) {
	case key_id::no_moving:
		if (j_phase_ == jump_phase::no_jump) {
			set_state(state::standing);
		}
		return;
	case key_id::right:
		set_direction(direction::right);
		dx = x < screen_scale.x - sprite_.getTextureRect().width ? off_t_x_ : 0;
		break;
	case key_id::left:
		set_direction(direction::left);
		dx = x > sprite_.getTextureRect().width ? -off_t_x_ : 0;
		break;
	case key_id::up:
		if (is_on_surface_) {
			dy = y > 0 ? -off_t_y_ : 0;
			is_on_surface_ = false;
			j_phase_ = jump_phase::up;
			last_surface_height_ = y;
		}
	}
	
	sprite_.move(dx, dy);
	last_off_t_x_ = dx; last_off_t_y_ = dy;
	set_state(state::moving);	
}


void player::jump_processing()
{
	const float dx = 0; float dy = 0;

	if (is_on_surface_) {
		j_phase_ = jump_phase::no_jump;
		set_state(state::standing);
		return;
	}
	
	switch (j_phase_) {
	case jump_phase::up:
		dy = -off_t_y_;
		if (last_surface_height_ - sprite_.getPosition().y >= jump_height) {
			j_phase_ = jump_phase::down;
		}
		break;
	case jump_phase::down:
		dy = off_t_y_;
		/*if (sprite_.getPosition().y >= 700) {
			is_on_surface_ = true;
			j_phase_ = jump_phase::no_jump;
			set_state(state::standing);
		}*/
		break;
	case jump_phase::no_jump:
		return;
	}
	
	sprite_.move(dx, dy);
	last_off_t_x_ = dx; last_off_t_y_ = dy;
}


void player::set_state(const state st)
{
	switch (st) {
	case state::standing:
		sprite_.setTextureRect(sf::IntRect(
			0, 0, sprite_scale_.x, sprite_scale_.y));
		break;
	default:
		sprite_.setTextureRect(sf::IntRect(
			sprite_scale_.x, 0, sprite_scale_.x, sprite_scale_.y));
	}
	state_ = st;
}


void player::set_direction(const direction dir)
{
	if (dir != direction_) {
		direction_ = dir;
		sprite_.scale(-1, 1);
	}
}
#include "player.hpp"
#include "keyboard_manager.hpp"
#include "game_specifications.hpp"


player::player(const sf::Texture& texture, const sf::Vector2f& scale) : game_hero(texture, scale)
{
	id_ = hero_id::player;
	off_t_x_ = block_scale.x / 100; off_t_y_ = block_scale.y / 150;
	set_position({ screen_scale.x / 2, screen_scale.y / 2 });
	jump_height_ = 3 * screen_scale.y / 8;
}


void player::update(const sf::Event&)
{
	const auto keys = keyboard_manager::get_key();
	for (const auto& key : keys) {
		switch (key) {
		case key_id::no_moving:
			if (state_ == state::standing) {
				switch_frame({ 0, 0 });
			}
			break;
		case key_id::right:
			move_right();
			switch_frame({ 1, 0 });
			break;
		case key_id::left:
			move_left();
			switch_frame({ 1, 0 });
			break;
		case key_id::up:
			move_up();
			switch_frame({ 1, 0 });
		}
		jump_processing();
	}
	if (state_ != state::standing) {
		
	}
}
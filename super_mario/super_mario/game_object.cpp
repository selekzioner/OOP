#include "game_object.hpp"
#include "game_specifications.hpp"


game_object::game_object(const sf::Texture& texture)
{
	sprite_.setTexture(texture);
}


void game_object::collision()
{
	/*auto [x, y] = sprite_.getPosition();
	if (last_off_t_x_ < 0) {
		sprite_.setPosition(x - last_off_t_x_, 0);
	}
	if (last_off_t_x_ > 0) {
		sprite_.move(-last_off_t_x_ - block_scale.x, 0);
	}
	if (last_off_t_y_ > 0) {
		sprite_.move(0, -last_off_t_y_ - block_scale.y);
		is_on_surface_ = true;
	}
	if (last_off_t_y_ < 0) {
		sprite_.move(0, -last_off_t_y_);
	}*/
	
	if (last_off_t_x_ < 0) {
		sprite_.move(-last_off_t_x_, 0);
	}
	if (last_off_t_x_ > 0) {
		sprite_.move(-last_off_t_x_ - block_scale.x, 0);
	}
	if (last_off_t_y_ > 0) {
		sprite_.move(0, -last_off_t_y_ - block_scale.y);
		is_on_surface_ = true;
	}
	if (last_off_t_y_ < 0) {
		sprite_.move(0, -last_off_t_y_);
	}
}


void game_object::draw(sf::RenderWindow& window)
{
	window.draw(sprite_);
}


object_id game_object::get_type() const
{
	return type_;
}


sf::FloatRect game_object::get_bounds() const
{
	return sprite_.getGlobalBounds();
}
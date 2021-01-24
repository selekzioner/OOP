#include "drawable.hpp"
#include "game_specifications.hpp"


drawable::drawable(const sf::Texture& texture, const sf::Vector2f& scale)
{
	sprite_.setTexture(texture);
	scale_ = scale;
	frame_count_.x = static_cast<unsigned>(texture.getSize().x / scale.x);
	frame_count_.y = static_cast<unsigned>(texture.getSize().y / scale.y);
	switch_frame({ 0, 0 });
}


void drawable::set_position(const sf::Vector2f& pos)
{
	sprite_.setPosition(pos);
}


void drawable::move(const sf::Vector2f& offset)
{
	sprite_.move(offset.x, offset.y);
}


void drawable::set_center()
{
	sprite_.setPosition(screen_scale.x / 2, sprite_.getPosition().y);
}


void drawable::draw(sf::RenderWindow& window)
{
	window.draw(sprite_);
}


sf::Vector2f drawable::get_position() const
{
	return sprite_.getPosition();
}


sf::FloatRect drawable::get_bounds() const
{
	return sprite_.getGlobalBounds();
}


void drawable::switch_frame(const sf::Vector2u& frame_pos)
{
	if (frame_pos.x <= frame_count_.x && frame_pos.y <= frame_count_.y) {
		sprite_.setTextureRect(sf::IntRect(
			frame_pos.x * scale_.x, frame_pos.y * scale_.y, scale_.x, scale_.y));
	}
}

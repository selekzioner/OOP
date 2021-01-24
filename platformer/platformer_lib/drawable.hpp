#pragma once
#include <SFML/Graphics.hpp>

class drawable
{
public:
	drawable() = delete;
	explicit drawable(const sf::Texture& texture, const sf::Vector2f& scale);

	drawable(const drawable&) = default;
	drawable(drawable&&) noexcept = default;

	drawable& operator=(const drawable&) = default;
	drawable& operator=(drawable&&) noexcept = default;

	void set_position(const sf::Vector2f& pos);
	void move(const sf::Vector2f& offset);

	void set_center();

	void draw(sf::RenderWindow& window);

	sf::Vector2f get_position() const;
	sf::FloatRect get_bounds() const;

	virtual ~drawable() = default;
	
protected:
	sf::Sprite sprite_{};
	sf::Vector2f scale_{};
	sf::Vector2u frame_count_{};

	void switch_frame(const sf::Vector2u& frame_pos);
};

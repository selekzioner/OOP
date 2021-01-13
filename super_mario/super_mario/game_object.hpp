#pragma once
#include <SFML/Graphics.hpp>

enum class object_id
{
	player,
	coin,
	mushroom,
	beetle,
};


class game_object
{
public:
	game_object() = delete;
	explicit game_object(const sf::Texture& texture);

	game_object(const game_object& object) = default;
	game_object(game_object&& object) = default;

	game_object& operator=(const game_object& object) = default;
	game_object& operator=(game_object&& object) noexcept = default;

	void collision();
	
	virtual void update(const sf::Event&) = 0;
	
	void draw(sf::RenderWindow& window);
	
	object_id get_type() const;
	sf::FloatRect get_bounds() const;

	virtual ~game_object() = default;

protected:
	object_id type_{};

	sf::Vector2f sprite_scale_{};
	sf::Sprite sprite_{};

	float off_t_x_{}, off_t_y_{}, last_off_t_x_{}, last_off_t_y_{};

	bool is_on_surface_ = true;

	enum class state { standing, moving };
	enum class direction { left, right };

	virtual void set_state(state st) = 0;
	virtual void set_direction(direction dir) = 0;

	state state_ = state::standing;
	direction direction_ = direction::right;
};
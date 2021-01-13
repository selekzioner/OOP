#pragma once
#include "game_object.hpp"
#include "game_specifications.hpp"

class player final : public game_object
{
public:
	player() = delete;
	explicit player(const sf::Texture& texture);

	void update(const sf::Event& event) override;

private:
	void jump_processing();
	
	void set_state(state st) override;
	void set_direction(direction dir) override;

	enum class jump_phase { up, down, no_jump };
	jump_phase j_phase_ = jump_phase::no_jump;
	static inline const float jump_height = screen_scale.y / 3;

	float last_surface_height_ = 0;
};
#pragma once
#include "hero_manager.hpp"
#include "texture_loader.hpp"
#include "enemy.hpp"
#include <vector>
#include "tile_map.hpp"
#include "event_manager.hpp"
#include "offset_manager.hpp"

class hero_manager final
{
public:
	hero_manager() = delete;
	explicit hero_manager(texture_loader& loader);
	
	void update(const sf::Event& event, tile_map& map);

	void render(sf::RenderWindow& window) const;

	sf::Vector2f get_offset() const;
	sf::Vector2f player_pos() const;

	bool is_over() const;

private:
	event_manager e_manager_;
	std::unique_ptr<offset_manager> offt_manager_;
	
	std::vector<std::shared_ptr<game_hero>> heroes_;
	
	std::shared_ptr<game_hero> player_;
	std::shared_ptr<game_hero> turtle_;
	std::shared_ptr<game_hero> mushroom_;

	void add_enemy();
	//static std::vector<std::shared_ptr<game_hero>> enemies_;
};

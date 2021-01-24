#pragma once
#include <vector>
#include "game_hero.hpp"

class event_manager final
{
public:
	event_manager() = default;
	
	void push_event(const sf::Event& event);
	void subscribe(std::shared_ptr<game_hero>& object);
	void unsubscribe(std::shared_ptr<game_hero>& object);
	
private:
	std::vector<std::shared_ptr<game_hero>> objects_;
};
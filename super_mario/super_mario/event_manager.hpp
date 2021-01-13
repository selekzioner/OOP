#pragma once
#include <vector>
#include "game_object.hpp"

class event_manager final
{
public:
	event_manager() = default;
	
	void push_event(const sf::Event& event);
	void subscribe(std::shared_ptr<game_object>& object);
	void unsubcribe(std::shared_ptr<game_object>& object);
	
private:
	std::vector<std::shared_ptr<game_object>> objects_;
};
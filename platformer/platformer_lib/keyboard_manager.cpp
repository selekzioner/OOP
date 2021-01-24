#include "keyboard_manager.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <vector>


std::vector<key_id> keyboard_manager::get_key()
{
	std::vector<key_id> keys;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		keys.push_back(key_id::right);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (keys.empty()) {
			keys.push_back(key_id::left);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		keys.push_back(key_id::up);
	}
	if (keys.empty()) {
		keys.push_back(key_id::no_moving);
	}
	return keys;
}


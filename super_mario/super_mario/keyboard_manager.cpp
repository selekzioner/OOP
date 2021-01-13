#include "keyboard_manager.hpp"
#include <SFML/Window/Keyboard.hpp>


key_id keyboard_manager::get_key()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		return key_id::right;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		return key_id::left;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		return key_id::up;
	}
	return key_id::no_moving;
}

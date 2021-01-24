#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

class image final
{
public:
	image();
	void set_image(sf::Sprite& s, const sf::Vector2i& pos, const int& size, const std::string& name) const;
	~image() = default;
private:
	std::unordered_map<std::string, sf::Texture> textures_;
};
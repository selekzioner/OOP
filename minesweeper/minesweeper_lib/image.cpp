#include "image.hpp"

image::image()
{
	std::vector<std::string> vector;
	vector.emplace_back("close");
	vector.emplace_back("flag");
	vector.emplace_back("bomb");
	vector.emplace_back("red_bomb");
	for (auto i = 0; i < 8; ++i){
		vector.emplace_back(std::to_string(i));
	}
	vector.emplace_back("small_flag");
	vector.emplace_back("clock");
	vector.emplace_back("smile");
	vector.emplace_back("win");
	vector.emplace_back("losing");
	vector.emplace_back("back");
	
	for (const auto& i : vector){
		sf::Texture t;
		if (!t.loadFromFile("images/" + i + ".png")){
			throw std::invalid_argument(i);
		}
		textures_[i] = t;
	}
}

void image::set_image(sf::Sprite& s, const sf::Vector2i& pos, const int& size, const std::string& name) const
{
	const auto scale = static_cast<float> (size) / textures_.at(name).getSize().x;
    s.setTexture(textures_.at(name));
	s.setScale(scale, scale);
	s.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
}
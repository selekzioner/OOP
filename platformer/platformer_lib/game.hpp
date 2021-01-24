#pragma once
#include <fstream>
#include "texture_loader.hpp"

class game final
{
public:
	game() = delete;
	static void run(unsigned short level);
	
private:
	static inline texture_loader t_loader_;
	
	static inline std::ifstream load_map(unsigned short level);

	static void game_over(sf::RenderWindow& window, texture_id id);
};

#include "texture_loader.hpp"
#include <sstream>


void texture_loader::load()
{
	std::map<texture_id, const std::string> paths {
		{ texture_id::player, "Resources/Textures/mario.png" },
		{ texture_id::block, "Resources/Textures/block.png" },
		{ texture_id::pipe, "Resources/Textures/pipe.png" },
		{ texture_id::coin, "Resources/Textures/coin.png" },
		{ texture_id::mushroom, "Resources/Textures/mushroom.png" },
		{ texture_id::turtle, "Resources/Textures/turtle.png" },
		{ texture_id::win, "Resources/Textures/win.png" },
		{ texture_id::game_over, "Resources/Textures/game_over.png" }
	};

	for (auto& [type, path] : paths) {
		if (!textures_[type].loadFromFile(path)) {
			std::ostringstream strm;
			strm << "can't open input file: " << path << std::endl;
			throw std::invalid_argument(strm.str());
		}
	}
}


texture_loader::texture& texture_loader::operator[](const texture_id& id)
{
	return textures_.at(id);
}

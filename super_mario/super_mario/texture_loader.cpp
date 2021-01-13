#include "texture_loader.hpp"
#include <sstream>


void texture_loader::load()
{
	std::map<texture_id, const std::string> paths {
		{ texture_id::player, "Resources/Textures/mario.png" },
		{ texture_id::brick, "Resources/Textures/block.png" }
		/*{ object_type::sky, "Resources/Textures/sky.png" }
		{ object_type::mushroom, "mushroom.png" },
		{ object_type::goomba, "goomba.png" },
		{ object_type::buzzy_beetle, "buzzy_beetle.png" },
		{ object_type::cheep_cheep, "cheep_cheep.png" },
		{ object_type::coin, "coin.png" },*/
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

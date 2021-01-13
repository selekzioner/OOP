#include <SFML/Graphics.hpp>
#include <fstream>
#include "game.hpp"
#include "game_object.hpp"
#include "player.hpp"
#include "texture_loader.hpp"
#include "tile_map.hpp"
#include "event_manager.hpp"
#include "interaction_manager.hpp"


void game::run()
{
    texture_loader t_loader;
    t_loader.load();
	
    sf::RenderWindow game_window(
        sf::VideoMode(static_cast<unsigned>(screen_scale.x), 
            static_cast<unsigned>(screen_scale.y)), "SUPER MARIO", sf::Style::Close);

    std::shared_ptr<game_object> player_ptr = std::make_shared<player>(t_loader[texture_id::player]);

    std::ifstream f_map("Resources/Map/map.txt");
	if (!f_map) {
        throw std::invalid_argument("invalid map file\n");
	}
    tile_map t_map(f_map, t_loader);

    event_manager e_manager;
    e_manager.subscribe(player_ptr);

    while (game_window.isOpen()) {
        sf::Event event;
        while (game_window.pollEvent(event)) {
            if (event.type == sf::Event::EventType::Closed) {
                game_window.close();
                break;
            }
        }

        interaction_manager::block_collision(player_ptr, t_map);
    	
        game_window.clear();
        e_manager.push_event(event);
        player_ptr->draw(game_window);
        t_map.draw(game_window);
        game_window.display();
    }
}

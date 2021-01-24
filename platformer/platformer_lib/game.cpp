#include <SFML/Graphics.hpp>
#include <fstream>
#include <thread>
#include "game.hpp"
#include "tile_map.hpp"
#include "hero_manager.hpp"
#include "game_specifications.hpp"


void game::run(unsigned short level)
{
    t_loader_.load();

    sf::RenderWindow game_window(
        sf::VideoMode(static_cast<unsigned>(screen_scale.x),
            static_cast<unsigned>(screen_scale.y)), "SUPER MARIO", sf::Style::Close);
	
    auto f_map = load_map(level);
    tile_map t_map(f_map, t_loader_);

    hero_manager h_manager(t_loader_);


    while (game_window.isOpen()) {
        sf::Event event{};
        while (game_window.pollEvent(event)) {
            if (event.type == sf::Event::EventType::Closed) {
                game_window.close();
                break;
            }
        }

        h_manager.update(event, t_map);

        game_window.clear(sf::Color(147, 112, 219));

        t_map.draw(game_window, h_manager.get_offset(), h_manager.player_pos());
        h_manager.render(game_window);

        game_window.display();
    	if (h_manager.is_over()) {
            game_over(game_window, texture_id::game_over);
            return;
    	}
        if (t_map.get_coin_count() == 0) {
            game_over(game_window, texture_id::win);
            break;
        }
    }
}


std::ifstream game::load_map(const unsigned short level)
{
    std::ifstream f_map("Resources/Map/map" + std::to_string(level) + ".txt");
    if (!f_map) {
        throw std::invalid_argument("invalid map file\n");
    }
    return f_map;
}


void game::game_over(sf::RenderWindow& window, const texture_id id)
{
    if (id == texture_id::win) {
        window.clear(sf::Color(147, 112, 219));
    }
    else {
        window.clear(sf::Color::Black);
    }
    sf::Sprite s(t_loader_[id]);
    s.setPosition(0, screen_scale.y / 4);

    window.draw(s);
    window.display();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);

    window.close();
}

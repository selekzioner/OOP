#include "hero_manager.hpp"
#include "event_manager.hpp"
#include "interaction_manager.hpp"
#include "player.hpp"


hero_manager::hero_manager(texture_loader& loader)
{
    player_ = std::make_shared<player>(loader[texture_id::player], sf::Vector2f(32, 60));

    turtle_ = std::make_shared<turtle>(loader[texture_id::turtle], sf::Vector2f(32, 32));
    mushroom_ = std::make_shared<mushroom>(loader[texture_id::mushroom], sf::Vector2f(32, 32));

    e_manager_.subscribe(player_);
    offt_manager_ = std::make_unique<offset_manager>(player_);
	
    heroes_.emplace_back(player_);
}


void hero_manager::update(const sf::Event& event, tile_map& map)
{
    e_manager_.push_event(event);

    for (auto& i : heroes_) {
        interaction_manager::check_collision(i, map, offt_manager_->get_last_offset());
        if (i->lives_count() == 0) {
            e_manager_.unsubscribe(i);
        	
            auto it = std::find(heroes_.begin(), heroes_.end(), i);
            if (it != heroes_.end()) {
                heroes_.erase(it);
            }
        }
    }
	
}


void hero_manager::render(sf::RenderWindow& window) const
{
    if (offt_manager_->compute_offset().x > 0) {
        player_->set_center();
    }
    for (const auto& i : heroes_) {
        i->draw(window);
    }
}


sf::Vector2f hero_manager::get_offset() const
{
    return offt_manager_->get_last_offset();
}


sf::Vector2f hero_manager::player_pos() const
{
    return player_->get_position();
}


bool hero_manager::is_over() const
{
    return player_->lives_count() == 0;
}


void hero_manager::add_enemy()
{
    heroes_.emplace_back(mushroom_);
    e_manager_.subscribe(heroes_.back());
}

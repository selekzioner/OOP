#include "event_manager.hpp"


void event_manager::push_event(const sf::Event& event)
{
    for (auto& object : objects_) {
        object->update(event);
    }
}


void event_manager::subscribe(std::shared_ptr<game_hero>& object)
{
    objects_.push_back(object);
}


void event_manager::unsubscribe(std::shared_ptr<game_hero>& object)
{
    auto it = std::find(objects_.begin(), objects_.end(), object);
    if (it != objects_.end()) {
        objects_.erase(it);
    }
}
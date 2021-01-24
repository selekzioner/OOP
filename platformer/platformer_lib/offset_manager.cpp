#include "offset_manager.hpp"


offset_manager::offset_manager(std::shared_ptr<game_hero>& hero) : hero_(hero)
{
	init_pos_ = hero_->get_position();
}


sf::Vector2f offset_manager::compute_offset()
{
	offset_ += (hero_->get_position() - init_pos_);
	offset_.x = offset_.x >= 0 ? offset_.x : 0;
	return offset_;
}


sf::Vector2f offset_manager::get_last_offset() const
{
	return offset_;
}

#pragma once
#include <list>
#include "game_hero.hpp"

class enemy : public game_hero
{
public:
	enemy() = delete;
	explicit enemy(const sf::Texture& texture, const sf::Vector2f& scale);

	void update(const sf::Event&) override;
};


class mushroom final : public enemy
{
public:
	mushroom() = delete;
	explicit mushroom(const sf::Texture& texture, const sf::Vector2f& scale);
};


class turtle final : public enemy
{
public:
	turtle() = delete;
	explicit turtle(const sf::Texture& texture, const sf::Vector2f& scale);
};


/*class enemy_pool final
{
public:
    using e_pool_ptr = std::unique_ptr<enemy_pool>;
    using enemy_ptr = std::unique_ptr<enemy>;
	
    static e_pool_ptr instance;
	
    enemy_ptr&& get_enemy(sf::Texture& t, const sf::Vector2f& scale)
    {
        if (enemies_.empty()) {
            return std::unique_ptr(t, scale);
        }
        auto enemy = std::move(enemies_.front());
        enemies_.pop_front();
        return std::move(enemy);
    }

    void return_enemy(enemy_ptr&& object)
    {
        enemies_.emplace_back(object);
    }
	
private:
    std::list<enemy_ptr> enemies_;

    enemy_pool() = default;
};*/
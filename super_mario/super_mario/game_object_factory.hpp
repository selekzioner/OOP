#pragma once
#include "player.hpp"
#include "enemy.hpp"
#include "texture_loader.hpp"

class game_object_factory
{
public:
    virtual std::shared_ptr<game_object>&& create_object(texture_loader&) = 0;
    virtual ~game_object_factory() = default;
};


class mario_factory final : public game_object_factory
{
public:
    std::shared_ptr<game_object>&& create_object(texture_loader& t_loader) override {
        return std::make_shared<player>(t_loader[texture_id::player]);
    }
};


/*class ArchersFactory final : public game_object_factory
{
public:
    std::shared_ptr<game_object>&& create_object(texture_loader& t_loader) override {
        return std::make_shared<mario>(t_loader[object_id::mario]);
    }
};


class CavalryFactory final : public game_object_factory
{
public:
    std::shared_ptr<game_object>&& create_object(texture_loader& t_loader) override {
        return std::make_shared<mario>(t_loader[object_id::mario]);
    }
};*/
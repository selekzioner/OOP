#pragma once
#include "game.hpp"

class menu final {
public:
	menu() = delete;

	class button final {
	public:
		button() = delete;
		explicit button(const sf::Vector2i& size, const std::string& txt, const sf::Vector2i& pos, sf::Font& font);
		void draw(sf::RenderWindow& app);
		bool contains(const sf::Vector2i& v) const;
	private:
		sf::IntRect borders_;
		sf::Text txt_;
	};

	static bool start();
	const static game::level get_level();

	~menu() = default;

private:
	const static inline int size = 600;
	static inline game::level level_ = game::level::beginner;
	static void draw_information(sf::RenderWindow& app);
};
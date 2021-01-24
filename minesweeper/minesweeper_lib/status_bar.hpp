#pragma once
#include "gameboard.hpp"
#include <SFML/Graphics.hpp>

class status_bar final
{
public:
	status_bar();

	void draw(sf::RenderWindow& app, const gameboard& board, const image& img);

	const static inline int height = 148;
	static bool is_smile_area_contains(const sf::Vector2i& v, const gameboard& board);
	static bool is_back_area_contains(const sf::Vector2i& v, const gameboard& board);

	~status_bar() = default;
	
private:
	sf::Font font_;
	sf::Clock clock_;
	int i_time_;

	void set_text(sf::Text& t, const sf::Vector2i& pos);
	static void set_rect(sf::RectangleShape& rect, const sf::Vector2i& size, const sf::Vector2i& pos);
	static void format_numbers(sf::Text& txt, const int& number);
};
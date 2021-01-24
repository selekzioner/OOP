#pragma once
#include "SFML/Graphics.hpp"

class menu final
{
public:
	menu() = delete;

	static void run();

private:
	static inline sf::Font font_;

	class menu_button final
	{
	public:
		menu_button() = delete;
		explicit menu_button(const sf::Vector2i& size, 
			unsigned short level, const sf::Vector2i& pos, sf::Font& font);

		void draw(sf::RenderWindow& app);
		
		bool contains(const sf::Vector2i& v) const;
		unsigned short get_level() const;
		
	private:
		sf::IntRect borders_;
		sf::Text txt_;
		unsigned short level_;
	};

	static std::unique_ptr<menu_button> button_factory(unsigned 
		short level, const sf::Vector2i& scale, const sf::Vector2i& pos);

	
	class button_manager final
	{
	public:
		button_manager();

		void select(const sf::Vector2i& pos);
		void render(sf::RenderWindow& window);
	private:
		std::vector<std::unique_ptr<menu_button>> level_buttons_;
	};
	
	static void draw_info(sf::RenderWindow& app);
};
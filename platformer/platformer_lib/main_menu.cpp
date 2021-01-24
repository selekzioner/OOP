#include "main_menu.hpp"
#include "game.hpp"
#include "game_specifications.hpp"
#include <string>


void menu::run()
{
	sf::RenderWindow app(sf::VideoMode(static_cast<int>(screen_scale.x / 2 + block_scale.x),
		static_cast<int>(screen_scale.y - block_scale.y / 2)), "Menu", sf::Style::Close);
	
	if (!font_.loadFromFile("Resources/Fonts/Amatic-Bold.ttf")) {
		throw std::invalid_argument("Resources/Fonts/Amatic-Bold.ttf");
	}

	button_manager b_manager;

	while (app.isOpen()) {
		sf::Event e{};

		while (app.pollEvent(e)) {

			if (e.type == sf::Event::Closed) {
				app.close();
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				b_manager.select(sf::Mouse::getPosition(app));
			}
		}
		
		
		app.clear(sf::Color(219, 219, 219));
		
		b_manager.render(app);
		draw_info(app);

		app.display();
	}
}


void menu::draw_info(sf::RenderWindow& app)
{
	sf::Text choose_level;
	choose_level.setFont(font_);
	choose_level.setString("CHOOSE THE LEVEL :");
	choose_level.setPosition(3 * block_scale.x, block_scale.y / 2);
	choose_level.setStyle(sf::Text::Style::Bold);
	choose_level.setFillColor(sf::Color(47, 79, 79));
	choose_level.setCharacterSize(80);
	app.draw(choose_level);
	
	sf::Texture t;
	if (!t.loadFromFile("Resources/Textures/info.png")) {
		throw std::invalid_argument("Resources/Textures/info.png");
	}
	sf::Sprite s;
	s.setTexture(t);
	s.setPosition(screen_scale.x / 4, 4 * block_scale.y);
	app.draw(s);
}



menu::menu_button::menu_button(const sf::Vector2i& size,
	const unsigned short level, const sf::Vector2i& pos, sf::Font& font)
{
	borders_.left = pos.x;
	borders_.top = pos.y;
	borders_.width = size.x;
	borders_.height = size.y;

	txt_.setString(std::to_string(level));
	txt_.setFont(font);
	txt_.setPosition(10 + static_cast<float>(borders_.left), static_cast<float>(borders_.top));
	txt_.setStyle(sf::Text::Style::Bold);
	txt_.setFillColor(sf::Color::White);
	txt_.setCharacterSize(80);

	level_ = level;
}


void menu::menu_button::draw(sf::RenderWindow& app)
{
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Black);
	rect.setPosition(static_cast<float>(borders_.left), static_cast<float>(borders_.top));
	rect.setSize(sf::Vector2f(static_cast<float>(borders_.width), static_cast<float>(borders_.height)));
	app.draw(rect);
	app.draw(txt_);
}


bool menu::menu_button::contains(const sf::Vector2i& v) const
{
	return borders_.contains(v);
}


unsigned short menu::menu_button::get_level() const
{
	return level_;
}



std::unique_ptr<menu::menu_button> menu::button_factory(const 
	unsigned short level, const sf::Vector2i& scale, const sf::Vector2i& pos)
{
	return std::make_unique<menu_button>(scale, level, pos, font_);
}


menu::button_manager::button_manager()
{
	const sf::Vector2i button_scale {static_cast<int>(3 * block_scale.x), static_cast<int>(3 * block_scale.y) };

	float dy = 0;
	for (auto i = 1; i <=3; ++i) {
		level_buttons_.emplace_back(button_factory(i,
			button_scale, { button_scale.x, static_cast<int>(4 * button_scale.y / 3  + dy)}));
		dy += 4 * button_scale.y / 3;
	}
}


void menu::button_manager::select(const sf::Vector2i& pos)
{
	for (auto& button : level_buttons_) {
		if (button->contains(pos)) {
			game::run(button->get_level());
		}
	}
}


void menu::button_manager::render(sf::RenderWindow& window)
{
	for (auto& button : level_buttons_) {
		button->draw(window);
	}
}
#include "menu.hpp"

bool menu::start() 
{
	sf::RenderWindow app (sf::VideoMode(size, size), "Minesweeper Menu", sf::Style::Close);
	sf::Font font;
	if (!font.loadFromFile("font/Amatic-Bold.ttf")) {
		throw std::invalid_argument("font/Amatic-Bold.ttf");
	}

	auto beginner = menu::button(sf::Vector2i(size / 2, size / 6), 
		"BEGINNER", sf::Vector2i(size / 4, size / 4), font);
	auto intermediate = menu::button(sf::Vector2i(size / 2, size / 6),
		"INTERMEDIATE", sf::Vector2i(size / 4, size / 2), font);
	auto expert = menu::button(sf::Vector2i(size / 2, size / 6),
		"EXPERT", sf::Vector2i(size / 4, 3 * size / 4), font);
	
	while (app.isOpen()) {
		sf::Event e{};
		app.clear(sf::Color(219, 219, 219));
		while (app.pollEvent(e)) {
			
			if (e.type == sf::Event::Closed) {
				app.close();
				return false;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& beginner.contains(sf::Mouse::getPosition(app))) {
				level_ = game::level::beginner;
				app.close();
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& intermediate.contains(sf::Mouse::getPosition(app))) {
				level_ = game::level::intermediate;
				app.close();
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& expert.contains(sf::Mouse::getPosition(app))) {
				level_ = game::level::expert;
				app.close();
			}
		}
		beginner.draw(app);
		intermediate.draw(app);
		expert.draw(app);
		draw_information(app);

		sf::Text choose_level;
		choose_level.setFont(font);
		choose_level.setString("CHOOSE THE LEVEL");
		choose_level.setPosition(static_cast<float>(size / 4) - 10, static_cast<float>(size / 24));
		choose_level.setStyle(sf::Text::Style::Bold);
		choose_level.setFillColor(sf::Color::Red);
		choose_level.setCharacterSize(80);
		app.draw(choose_level);

		app.display();
	}
	return true;
}

const game::level menu::get_level()
{
	return level_;
}

void menu::draw_information(sf::RenderWindow& app)
{
	sf::Texture* t = new sf::Texture;
	if (!t->loadFromFile("images/menu.png")) {
		throw std::invalid_argument("images/menu.png");
	}
	sf::Sprite s;
	s.setTexture(*t);
	s.setPosition(0, size / 4);
	app.draw(s);
	delete t;
}

menu::button::button(const sf::Vector2i& size, const std::string& txt, const sf::Vector2i& pos, sf::Font& font)
{
	borders_.left = pos.x;
	borders_.top = pos.y;
	borders_.width = size.x;
	borders_.height = size.y;

	txt_.setString(txt);
	txt_.setFont(font);
	txt_.setPosition(10 + static_cast<float>(borders_.left), static_cast<float>(borders_.top));
	txt_.setStyle(sf::Text::Style::Bold);
	txt_.setFillColor(sf::Color::White);
	txt_.setCharacterSize(80);
}

void menu::button::draw(sf::RenderWindow& app) 
{
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Black);
	rect.setPosition(static_cast<float>(borders_.left), static_cast<float>(borders_.top));
	rect.setSize(sf::Vector2f(static_cast<float>(borders_.width), static_cast<float>(borders_.height)));
	app.draw(rect);
	app.draw(txt_);
}

bool menu::button::contains(const sf::Vector2i& v) const
{
	return borders_.contains(v);
}
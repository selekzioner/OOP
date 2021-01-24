#include "status_bar.hpp"

status_bar::status_bar()
{
	if (!font_.loadFromFile("font/Amatic-Bold.ttf")) {
		throw std::invalid_argument("font/Amatic-Bold.ttf");
	}
	clock_.restart();
	i_time_ = 0;
}

void status_bar::draw(sf::RenderWindow& app, const gameboard& board, const image& img)
{
	auto width = board.size() * board.cell_size();
	
	//rects
	sf::RectangleShape l_rect;
	set_rect(l_rect, sf::Vector2i(9 * height / 16, height / 2), 
		sf::Vector2i(3 * height / 4, ((height / 4) + width)));
	app.draw(l_rect);

	sf::RectangleShape r_rect;
	set_rect(r_rect, sf::Vector2i(9 * height / 16, height / 2), 
		sf::Vector2i(width - (5 * height / 4), ((height / 4) + width)));
	app.draw(r_rect);

	//flags count
	sf::Text txt;
	format_numbers(txt, board.flags_count());
	set_text(txt, sf::Vector2i(3 * height / 4 + 3, (height / 4 + width - 12)));
	app.draw(txt);

	//flag
	sf::Sprite f;
	img.set_image(f, sf::Vector2i(height / 4, 
		height / 4 + width), height / 2, "small_flag");
	app.draw(f);

	//clock
	sf::Sprite c;
	img.set_image(c, sf::Vector2i(width - 5 * height / 8,
		height / 4 + width), height / 2, "clock");
	app.draw(c);

	//smile
	sf::Sprite s; std::string name;
	if (board.condition() == gameboard::board_condition::in_process) {
		name = "smile";
	}
	else if (board.condition() == gameboard::board_condition::win) {
		name = "win";
	}
	else {
		name = "losing";
	}
	img.set_image(s, sf::Vector2i(width / 2 - height / 4, height / 4 + width), height / 2, name);
	app.draw(s);
	
	//time
	sf::Text time;
	if (board.need_to_open()) {
		if (board.empty()) {
			clock_.restart();
		}
		i_time_ = static_cast<int>(clock_.getElapsedTime().asSeconds());
		i_time_ = i_time_ >= 1000 ? 999 : i_time_;
	}
	format_numbers(time, i_time_);
	set_text(time, sf::Vector2i(width - 5 * height / 4 + 3, (height / 4 + width - 12)));
	app.draw(time);

	//back
	sf::Sprite b;
	img.set_image(b, sf::Vector2i(0, width + 5), height / 3, "back");
	app.draw(b);
}

void status_bar::set_text(sf::Text& t, const sf::Vector2i& pos)
{
	t.setFont(font_);
	t.setStyle(sf::Text::Style::Bold);
	t.setFillColor(sf::Color::Red);
	t.setCharacterSize(80);
	t.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
}

void status_bar::set_rect(sf::RectangleShape& rect, const sf::Vector2i& size, const sf::Vector2i& pos)
{
	rect.setFillColor(sf::Color::Black);
	rect.setSize(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
	rect.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
}

void status_bar::format_numbers(sf::Text& txt, const int& number) 
{
	if (number > 99) {
		txt.setString(std::to_string(number));
	}
	else if (number > 9) {
		txt.setString("0" + std::to_string(number));
	}
	else {
		txt.setString("00" + std::to_string(number));
	}
}

bool status_bar::is_smile_area_contains(const sf::Vector2i& v, const gameboard& board)
{
	sf::IntRect area(board.size() * board.cell_size() / 2 - height / 4, 
		board.size() * board.cell_size() + height / 4, height / 2, height / 2);
	return area.contains(v);
}

bool status_bar::is_back_area_contains(const sf::Vector2i& v, const gameboard& board)
{
	sf::IntRect area(0, board.size() * board.cell_size() + 5, height / 3, height / 3);
	return area.contains(v);
}
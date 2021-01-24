#include "game.hpp"
#include "status_bar.hpp"
#include "menu.hpp"

game::game ()
{
	new_game_ = 1;
}

bool game::run(const game::level l)
{
	int size, bombs_count, cell_size;
	set_field_parameters(l, size, bombs_count, cell_size);
	auto board = gameboard(size, bombs_count, cell_size);
	image img;
	
	sf::RenderWindow app(sf::VideoMode(board.size() * board.cell_size(),
		board.size() * board.cell_size() + status_bar::height), "Minesweeper", sf::Style::Close);
	
	while (new_game_) {
		board.clear();
		status_bar sb;
		while (app.isOpen()) {
			new_game_ = 0;
			event_processing(app, board);
			if (!new_game_) {
				board.draw(app, img);
				sb.draw(app, board, img);
				app.display();
				game_over(app, board);
			}
			if (new_game_ == 1) {
				break;
			}
			if (new_game_ == -1) {
				app.close();
				return true;
			}
		}
	}
	return false;
}

void game::set_field_parameters(const level& l, int& size, int& bombs_count, int& cell_size)
{
	if (l == level::beginner) {
		cell_size = 60;
		size = 9; bombs_count = 10;
	}
	else if (l == level::intermediate) {
		cell_size = 45;
		size = 16; bombs_count = 40;
	}
	else {
		cell_size = 32;
		size = 23; bombs_count = 90;
	}
}

void game::event_processing(sf::RenderWindow& app, gameboard& board)
{
	const auto pos = sf::Mouse::getPosition(app);
	const auto v = sf::Vector2i(pos.x / board.cell_size(), pos.y / board.cell_size());
	
	sf::Event e {};
	while (app.pollEvent(e)) {
		if (e.type == sf::Event::Closed) {
			app.close();
		}
		if (e.type == sf::Event::MouseButtonPressed && v.y < board.size() && board.need_to_open()) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				board.open_cell(v);
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				board.flag(v);
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) 
			&& status_bar::is_smile_area_contains(sf::Mouse::getPosition(app), board)) {
			new_game_ = 1;
			return;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& status_bar::is_back_area_contains(sf::Mouse::getPosition(app), board)) {
			new_game_ = -1;
			return;
		}
	}
}

void game::game_over(sf::RenderWindow& app, const gameboard& board)
{
	sf::Event e {};
	if (!board.need_to_open()) {
		while (app.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				app.close();
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& status_bar::is_smile_area_contains(sf::Mouse::getPosition(app), board)){
				new_game_ = 1;
				return;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& status_bar::is_back_area_contains(sf::Mouse::getPosition(app), board)) {
				new_game_ = -1;
				return;
			}
		}
	}
}
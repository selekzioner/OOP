#pragma once
#include "gameboard.hpp"

class game final {
public:
	enum class level { beginner, intermediate, expert };
	
	game();
	bool run(const game::level l);
	~game() = default;
	
private:
	int new_game_;

	void event_processing(sf::RenderWindow& app, gameboard& board);
	void game_over(sf::RenderWindow& app, const gameboard& board);
	static void set_field_parameters(const level& l, int& size, int& bombs_count, int& cell_size);
};
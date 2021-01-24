#define CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#include "menu.hpp"
#include <iostream>
#define newd new(_NORMAL_BLOCK, __FILE__, __LINE__)

int main()
{
	try {
		while (menu::start()) {
			game g;
			if (!g.run(menu::get_level())) {
				break;
			}
		}
	}
	catch (std::invalid_argument& arg) {
		std::cerr << arg.what() << std::endl;
		std::cerr << "file does not exsist" << std::endl;
	}
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}
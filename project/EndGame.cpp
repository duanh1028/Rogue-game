#include "EndGame.hpp"
#include <iostream>

EndGame::EndGame(std::string _name, Creature* _owner) {
	name = _name;
	owner = _owner;
	//std::cout << "EndGame" << std::endl;
}
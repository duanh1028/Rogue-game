#include "Teleport.hpp"
#include <iostream>

Teleport::Teleport(std::string _name, Creature* _owner) {
	name = _name;
	owner = _owner;
	//std::cout << "Teleport" << std::endl;
}
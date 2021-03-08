#include "DropPack.hpp"
#include <iostream>

DropPack::DropPack(std::string _name, Creature* _owner) {
	name = _name;
	owner = _owner;
	//std::cout << "DropPack" << std::endl;
}
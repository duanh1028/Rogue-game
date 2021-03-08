#include "Remove.hpp"
#include <iostream>

Remove::Remove(std::string _name, Creature* _owner) {
	name = _name;
	owner = _owner;
	//std::cout << "Remove" << std::endl;
}
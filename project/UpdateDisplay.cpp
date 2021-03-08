#include "UpdateDisplay.hpp"
#include <iostream>

UpdateDisplay::UpdateDisplay(std::string _name, Creature* _owner) {
	name = _name;
	owner = _owner;
	//std::cout << "UpdateDisplay" << std::endl;
}
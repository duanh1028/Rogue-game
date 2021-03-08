#include <iostream>
#include "Item.hpp"

void Item::setOwner(Creature* _owner) {
	owner = _owner;
	//std::cout << "setOwner" << std::endl;
}

Creature* Item::getOwner() {
	return owner;
}

std::string Item::getName() {
	return name;
}

void Item::removeOwner() {
	owner = NULL;
}

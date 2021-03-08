#include <iostream>
#include "Sword.hpp"

Sword::Sword(std::string _name) {
	name = _name;
	//std::cout << "SwordConstructor" << std::endl;
}

void Sword::setID(int _room, int _serial) {
	room = _room;
	serial = _serial;
	//std::cout << "setID(Sword)" << std::endl;
}

std::string Sword::getName() {
	return name;
}
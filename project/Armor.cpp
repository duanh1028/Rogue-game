#include <iostream>
#include "Armor.hpp"

Armor::Armor(std::string _name) {
	name = _name;
	//std::cout << "ArmorConstructor" << std::endl;
}

void Armor::setName(std::string _name) {
	name = _name;
	//std::cout << "setName(Armor)" << std::endl;
}

void Armor::setID(int _room, int _serial) {
	room = _room;
	serial = _serial;
	//std::cout << "setID(Armor)" << std::endl;
}

std::string Armor::getName() {
	return name;
}
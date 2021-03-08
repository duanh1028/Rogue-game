#include <iostream>
#include "Monster.hpp"

Monster::Monster() {}

void Monster::setName(std::string _monsterName) {
	monsterName = _monsterName;
	//std::cout << "setName(Monster)" << std::endl;
}
void Monster::setID(int _room, int _serial) {
	room = _room;
	serial = _serial;
	//std::cout << "setID(Monster)" << std::endl;
}

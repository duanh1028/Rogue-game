#include <string>
#include <vector>
#include <iostream>
#include "Room.hpp"

Room::Room(std::string _roomName) {
	roomName = _roomName;
	//std::cout << "roomConstructure" << std::endl;
}

void Room::setID(int _room) {
	room = _room;
	//std::cout << "setID(Room)" << std::endl;

}
void Room::setCreature(Creature* _monster) {
	monsters.push_back(_monster);
	//std::cout << "setCreature" << std::endl;
}
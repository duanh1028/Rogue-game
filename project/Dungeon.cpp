#include <string>
#include <iostream>
#include "Dungeon.hpp"

Dungeon* Dungeon::dungeon;

void Dungeon::addRoom(Room* room) {
	rooms.push_back(room);
	//std::cout << "addRoom" << std::endl;
}

void Dungeon::addCreature(Creature* creature) {
	creatures.push_back(creature);
	//std::cout << "addCreature" << std::endl;
}

void Dungeon::addPassage(Passage* passage) {
	passages.push_back(passage);
	//std::cout << "addPassage" << std::endl;
}

void Dungeon::addItem(Item* item) {
	items.push_back(item);
	//std::cout << "addItem" << std::endl;
}

Dungeon* Dungeon::getDungeon(std::string _name, int _width, int _gameHeight) {
	if (dungeon == nullptr) {
		dungeon = new Dungeon(_name, _width, _gameHeight);
	}
	std::string out = "name: " + _name + " width: " + std::to_string(_width) + " gameHeight:" + std::to_string(_gameHeight);
	//std::cout << "getDungeons:" + out << std::endl;
	return dungeon;
}

int Dungeon::getDungeonHeight() {
	return gameHeight;
}

int Dungeon::getDungeonWidth() {
	return width;
}

std::vector<Room*>& Dungeon::getRooms(){
	return rooms;
}

std::vector<Creature*>& Dungeon::getCreatures() {
	return creatures;
}

std::vector<Item*>& Dungeon::getItems() {
	return items;
}

std::vector<Passage*>& Dungeon::getPassages() {
	return passages;
}

//put items and creatures in a vector and check x y each time player move 




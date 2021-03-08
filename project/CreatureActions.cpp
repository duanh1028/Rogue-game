#include "CreatureActions.hpp"
#include <iostream>

CreatureActions::CreatureActions() {}

CreatureActions::CreatureActions(Creature* _owner) {
	owner = _owner;
}
/*

void CreatureActions::YouWin(std::string _name, Creature* owner) {
	owners[ownerCount++] = owner;
	name = _name;
	//std::cout << "YouWin" << std::endl;
}

void CreatureActions::UpdateDisplay(std::string _name, Creature* owner) {
	owners[ownerCount++] = owner;
	name = _name;
	//std::cout << "UpdateDisplay" << std::endl;
}
void CreatureActions::Teleport(std::string _name, Creature* owner) {
	owners[ownerCount++] = owner;
	name = _name;
	//std::cout << "Teleport" << std::endl;
}

void CreatureActions::ChangedDisplayedType(std::string _name, Creature* owner) {
	owners[ownerCount++] = owner;
	name = _name;
	//std::cout << "ChangedDisplayType" << std::endl;
}
void CreatureActions::EndGame(std::string _name, Creature* owner) {
	owners[ownerCount++] = owner;
	name = _name;
	//std::cout << "Endgame" << std::endl;
}
void CreatureActions::DropPack(std::string _name, Creature* owner) {
	owners[ownerCount++] = owner;
	name = _name;
	//std::cout << "DropPack" << std::endl;
}*/
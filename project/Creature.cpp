#include <iostream>
#include "Creature.hpp"

Creature::Creature() {}

void Creature::setHp(int _hp) {
	hp = _hp;
	//std::cout << "setHp" << std::endl;
}
void Creature::setHpMoves(int _hpm) {
	hpm = _hpm;
	//std::cout << "setHpMoves" << std::endl;
}
void Creature::setDeathAction(CreatureActions* da) {
	das.push_back(da);
	//std::cout << "setDeathAction" << std::endl;
}
void Creature::setHitAction(CreatureActions* ha) {
	has.push_back(ha);
	//std::cout << "setHitAction" << std::endl;
}

int Creature::getHp() {
	return hp;
	//std::cout << "setHp" << std::endl;
}
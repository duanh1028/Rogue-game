#include <string>
#include <iostream>
#include "Passage.hpp"

Passage::Passage() {
	//std::cout << "passageConstructor" << std::endl;
}


void Passage::setName(std::string _passageName) {
	passageName = _passageName;
	//std::cout << "passageName" << std::endl;
}

void Passage::setID(int _room1, int _room2) {
	room1 = _room1;
	room2 = _room2;
	//std::cout << "setID(passage)" << std::endl;
}

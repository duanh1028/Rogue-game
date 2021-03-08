#include <iostream>
#include "Scroll.hpp"

Scroll::Scroll(std::string _name) {
	name = _name;
	//std::cout << "ScrollConstructor" << std::endl;
}
void Scroll::setID(int _room, int _serial){
	room = _room;
	serial = _serial;
	//std::cout << "setID(Scroll)" << std::endl;
}
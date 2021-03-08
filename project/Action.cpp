#include "Action.hpp"
#include <iostream>

void Action::setMessage(std::string _msg) {
	msg = _msg;
	//std::cout << "setMessage" << std::endl;
}
void Action::setIntValue(int _v) {
	v = _v;
	//std::cout << "setIntValue" << std::endl;
}

void Action::setCharValue(char _c) {
	c = _c;
	//std::cout << "setID(Scroll)" << std::endl;
}

std::string Action::getMessage() {
	return msg;
}

int Action::getIntValue() {
	return v;
}

char Action::getCharValue() {
	return c;
}

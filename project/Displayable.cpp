#include <iostream>
#include "Displayable.hpp"

Displayable::Displayable(int _maxHit, int _hpMoves, int _Hp, char _t, int _v, int _x, int _y) {
	maxHit = _maxHit;
	hpMoves = _hpMoves;
	Hp = _Hp;
	t = _t;
	v = _v;
	x = _x;
	y = _y;
	//std::cout << "DisplayableConstructor" << std::endl;
}

Displayable::Displayable() {}

void Displayable::setInvisible() {
	//std::cout << "setInvisible" << std::endl;
}

void Displayable::setVisible() {
	//std::cout << "setVisible" << std::endl;
}

void Displayable::setMaxHit(int _maxHit) {
	maxHit = _maxHit;
	//std::cout << "setMaxHit" << std::endl;
}

void Displayable::setHpMove(int _hpMoves) {
	hpMoves = _hpMoves;
	//std::cout << "setHpMove" << std::endl;
}

void Displayable::setHp(int _Hp) {
	Hp = _Hp;
	//std::cout << "setHp" << std::endl;
}

void Displayable::setType(char _t) {
	t = _t;
	//std::cout << "setType" << std::endl;
}

void Displayable::setIntValue(int _v) {
	v = _v;
	//std::cout << "setIntValue" << std::endl;
}

void Displayable::setPosX(int _x) {
	x = _x;
	//std::cout << "setPosX" << std::endl;
}
void Displayable::setPosY(int _y) {
	y = _y;
	//std::cout << "setPoxY" << std::endl;
}
void Displayable::setWidth(int _width) {
	width = _width;
	//std::cout << "setWidth" << std::endl;
}
void Displayable::setHeight(int _height) {
	height = _height;
	//std::cout << "setHeight" << std::endl;
}

int Displayable::getPosX() {
	return x;
}
int Displayable::getPosY() {
	return y;
}
int Displayable::getWidth() {
	return width;
}
int Displayable::getHeight() {
	return height;
}
char Displayable::getType() {
	return t;
}
int Displayable::getHp() {
	return Hp;
}

void Displayable::addX(int x) {
	Xs.push_back(x);
}

void Displayable::addY(int y) {
	Ys.push_back(y);
}

std::vector<int>& Displayable::getXs() {
	return Xs;
}

std::vector<int>& Displayable::getYs() {
	return Ys;
}

int Displayable::getMaxHit() {
	return maxHit;
}

int Displayable::getHpMove() {
	return hpMoves;
}

int Displayable::getIntValue() {
	return v;
}
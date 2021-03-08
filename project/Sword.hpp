#ifndef SWORD_H_
#define SWORD_H_
#include <string>
#include "Item.hpp"

class Sword : public Item {
public:
	Sword(std::string _name);
	void setID(int _room, int _serial);
	std::string getName();

private:
	//std::string name;
	int room;
	int serial;
};
#endif /* SWORD_H_ */
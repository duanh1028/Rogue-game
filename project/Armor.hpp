#ifndef ARMOR_H_
#define ARMOR_H_
#include <string>
#include "Item.hpp"

class Armor : public Item {
public:
	Armor(std::string _name);
	void setName(std::string _name);
	void setID(int _room, int _serial);
	std::string getName();

private:
	//std::string name;
	int room;
	int serial;
};
#endif /* ARMOR_H_ */
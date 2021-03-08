#ifndef MONSTER_H_
#define MONSTER_H_
#include <string>
#include "Creature.hpp"

class Monster : public Creature {
public:
	Monster();
	void setName(std::string _monsterName);
	void setID(int _room, int _serial);

private:
	std::string monsterName;
	int room;
	int serial;
};
#endif /* MONSTER_H_ */
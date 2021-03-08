#ifndef ROOM_H_
#define ROOM_H_
#include <string>
#include <vector>
#include "Structure.hpp"
#include "Creature.hpp"

class Room : public Structure{
public:
	Room(std::string _roomName);
	void setID(int _room);
	void setCreature(Creature* _monster);

private:
	int room;
	std::vector<Creature*> monsters;
	//int monsterCount = 0;
	std::string roomName;
};
#endif /* ROOM_H_ */

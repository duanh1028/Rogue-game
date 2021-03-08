#ifndef DUNGEON_H_
#define DUNGEON_H_
#include <string>
#include <vector>
#include "Room.hpp"
#include "Creature.hpp"
#include "Item.hpp"
#include "Passage.hpp"

class Dungeon {
public:
	Dungeon(std::string _name, int _width, int _gameHeight) { name = _name; width = _width; gameHeight = _gameHeight; };
	void addRoom(Room* room);
	void addCreature(Creature* creature);
	void addPassage(Passage* passage);
	void addItem(Item* item);
	static Dungeon* getDungeon(std::string _name, int _width, int _gameHeight);
	static Dungeon* dungeon;

	//add for step 2
	int getDungeonHeight();
	int getDungeonWidth();

	std::vector<Room*>& getRooms();
	std::vector<Creature*>& getCreatures();
	std::vector<Passage*>& getPassages();
	std::vector<Item*>& getItems();


private:
	//int bottomHeight;
	int gameHeight;
	//int topHeight;
	int width;
	std::string name;
	std::vector<Room*> rooms;
	//int roomCount = 0;
	std::vector<Creature*> creatures;
	//int creatureCount = 0;
	std::vector<Item*> items;
	//int itemCount = 0;
	std::vector<Passage*> passages;
	//int passageCount = 0;
};
#endif /* DUNGEON_H_ */

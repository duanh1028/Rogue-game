#ifndef PLAYER_H_
#define PLAYER_H_
#include <vector>
#include "Creature.hpp"
#include "Item.hpp"

class Player : public Creature {
public:
	void setWeapon(Item* sword);
	void setArmor(Item* armor);
	void setItem(Item* item);
	std::vector<Item*>& getWeapons();
	std::vector<Item*>& getArmors();
	std::vector<Item*>& getItems();
	void removeWeapon(Item* item);
	void removeArmor(Item* item);
	void removeItem(Item* item);
	void removeScroll();

	//add for step4
	void wearWeapon(Item* weapon);
	void wearArmor(Item* armor);
	void offWeapon(Item* _sword);
	void offArmor(Item* _armor);
	Item* getWeapon();
	Item* getArmor();

	void setScroll(Item* Scroll);
	Item* getScroll();

private:
	std::vector<Item*> swords;
	std::vector<Item*> armors;
	std::vector<Item*> items;

	Item* scroll = NULL;

	Item* sword = NULL;
	Item* armor = NULL;
};
#endif /* PLAYER_H_ */
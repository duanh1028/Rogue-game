#ifndef CREATUREACTION_H_
#define CREATUREACTION_H_
#include <string>
#include <vector>
#include "Action.hpp"

class Creature;

#include "Creature.hpp"

class CreatureActions : public Action{
public:
	CreatureActions();
	CreatureActions(Creature* _owner);
	Creature* getOwner() {
		return owner;
	}
	/*
	void Remove(std::string _name, Creature* owner);
	void YouWin(std::string _name, Creature* owner);
	void UpdateDisplay(std::string _name, Creature* owner);
	void Teleport(std::string _name, Creature* owner);
	void ChangedDisplayedType(std::string _name, Creature* owner);
	void EndGame(std::string _name, Creature* owner);
	void DropPack(std::string _name, Creature* owner);
	*/
private:
	//std::string name;
	//std::vector<Creature*> owners;
	//int ownerCount = 0;
	Creature* owner;
};
#endif /* CREATUREACTION_H_ */
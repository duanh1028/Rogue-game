#ifndef ITEMACTIONS_H_
#define ITEMACTIONS_H_
#include <vector>
#include "Action.hpp"
#include "Creature.hpp"

class ItemActions : public Action {
public:
	ItemActions(Creature* _owner);
	ItemActions() {}
	//void BlessCurseOwner(Creature* owner);
	//void Hallucinate(Creature* owner);

private:
	Creature* owner;
};
#endif /* ITEMACTIONS_H_ */
#ifndef BLESSCURSEOWNER_H_
#define BLESSCURSEOWNER_H_
#include <vector>
#include "ItemActions.hpp"
#include "Creature.hpp"

class BlessCurseOwner : public ItemActions {
public:
	BlessCurseOwner(Creature* _owner);

private:
	Creature* owner;
};
#endif /* BLESSCURSEOWNER_H_ */
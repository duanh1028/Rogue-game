#ifndef DROPPACK_H_
#define DROPPACK_H_
#include <string>
#include <vector>
#include "CreatureActions.hpp"
#include "Creature.hpp"

class DropPack : public CreatureActions {
public:
	DropPack(std::string _name, Creature* _owner);

private:
	std::string name;
	Creature* owner;
};
#endif /* DROPPACK_H_ */
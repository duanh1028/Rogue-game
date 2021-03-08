#ifndef TELEPORT_H_
#define TELEPORT_H_
#include <string>
#include <vector>
#include "CreatureActions.hpp"
#include "Creature.hpp"

class Teleport : public CreatureActions {
public:
	Teleport(std::string _name, Creature* _owner);

private:
	std::string name;
	Creature* owner;
};
#endif /* TELEPORT_H_ */
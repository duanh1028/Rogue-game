#ifndef REMOVE_H_
#define REMOVE_H_
#include <string>
#include <vector>
#include "CreatureActions.hpp"
#include "Creature.hpp"

class Remove : public CreatureActions {
public:
	Remove(std::string _name, Creature* _owner);
	
private:
	std::string name;
	Creature* owner;
};
#endif /* REMOVE_H_ */
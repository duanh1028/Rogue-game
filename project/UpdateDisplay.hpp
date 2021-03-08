#ifndef UPDATEDISPLAY_H_
#define UPDATEDISPLAY_H_
#include <string>
#include <vector>
#include "CreatureActions.hpp"
#include "Creature.hpp"

class UpdateDisplay : public CreatureActions {
public:
	UpdateDisplay(std::string _name, Creature* _owner);

private:
	std::string name;
	Creature* owner;
};
#endif /* UPDATEDISPLAY_H_ */
#ifndef YOUWIN_H_
#define YOUWIN_H_
#include <string>
#include <vector>
#include "CreatureActions.hpp"
#include "Creature.hpp"

class YouWin : public CreatureActions {
public:
	YouWin(std::string _name, Creature* _owner);

private:
	std::string name;
	Creature* owner;
};
#endif /* YOUWIN_H_ */
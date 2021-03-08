#ifndef ENDGAME_H_
#define ENDGAME_H_
#include <string>
#include <vector>
#include "CreatureActions.hpp"
#include "Creature.hpp"

class EndGame : public CreatureActions {
public:
	EndGame(std::string _name, Creature* _owner);

private:
	std::string name;
	Creature* owner;
};
#endif /* ENDGAME_H_ */
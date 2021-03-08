#ifndef CREATURE_H_
#define CREATURE_H_
#include <string>
#include <vector>
#include "Displayable.hpp"

class CreatureActions;

#include "CreatureActions.hpp"

class Creature : public Displayable {
public:
	Creature();
	void setHp(int _hp);
	void setHpMoves(int _hpm);
	void setDeathAction(CreatureActions* da);
	void setHitAction(CreatureActions* ha);

	int getHp();

private:
	int hp;
	int hpm;
	std::vector<CreatureActions*> das;
	std::vector<CreatureActions*> has;
};
#endif /* CREATURE_H_ */
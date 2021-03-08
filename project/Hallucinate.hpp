#ifndef HALLUCINATE_H_
#define HALLUCINATE_H_
#include <vector>
#include "ItemActions.hpp"
#include "Creature.hpp"

class Hallucinate : public ItemActions {
public:
	Hallucinate(Creature* _owner);

private:
	Creature* owner;
};
#endif /* HALLUCINATE_H_ */
#ifndef ITEM_H_
#define ITEM_H_
#include <vector>
#include "Displayable.hpp"
#include "Creature.hpp"

class Item : public Displayable {
public:
	void setOwner(Creature* _owner);
	Creature* getOwner();
	std::string getName();
	std::string name;
	void removeOwner();

private:
	Creature* owner = NULL;
};
#endif /* ITEM_H_ */
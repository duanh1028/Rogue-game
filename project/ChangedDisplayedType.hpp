#ifndef CHANGEDDISPLAYEDTYPE_H_
#define CHANGEDDISPLAYEDTYPE_H_
#include <string>
#include <vector>
#include "CreatureActions.hpp"
#include "Creature.hpp"

class ChangedDisplayedType : public CreatureActions {
public:
	ChangedDisplayedType(std::string _name, Creature* _owner);

private:
	std::string name;
	Creature* owner;
};
#endif /* CHANGEDDISPLAYEDTYPE_H_ */
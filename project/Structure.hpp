#ifndef STRUCTURE_H_
#define STRUCTURE_H_

#include "Displayable.hpp"

class Structure : public Displayable {
public:
	Structure(int _maxHit, int _hpMoves, int _Hp, char _t, int _v, int _x, int _y);
	Structure();
private:
};
#endif /* STRUCTURE_H_ */

#ifndef PASSAGE_H_
#define PASSAGE_H_
#include <string>
#include "Structure.hpp"

class Passage : public Structure {
public:
	Passage();
	void setName(std::string _passageName);
	void setID(int _room1, int _room2);

private:
	int room1;
	int room2;
	std::string passageName;
};
#endif /* STRUCTURE_H_ */

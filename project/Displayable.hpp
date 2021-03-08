#ifndef DISPLAYABLE_H_
#define DISPLAYABLE_H_
#include <vector>

class Displayable {
public:
	Displayable(int _maxHit, int _hpMoves, int _Hp, char _t, int _v, int _x, int _y);
	Displayable();
	virtual void setInvisible();
	void setVisible();
	void setMaxHit(int _maxHit);
	void setHpMove(int _hpMoves);
	void setHp(int _Hp);
	void setType(char _t);
	void setIntValue(int _v);
	void setPosX(int _x);
	void setPosY(int _y);
	void setWidth(int _width);
	void setHeight(int _height);
	
	//add for step 2
	int getPosX();
	int getPosY();
	int getWidth();
	int getHeight();
	char getType();
	int getHp();

	//multiple x and y for passage
	void addX(int x);
	void addY(int y);
	std::vector<int>& getXs();
	std::vector<int>& getYs();

	//add for step 3
	int getMaxHit();

	//add for step 4
	int getHpMove();
	int getIntValue();

private:
	int maxHit;
	int hpMoves;
	int Hp;
	char t;
	int v;
	int x;
	int y;
	int width;
	int height;

	std::vector<int> Xs;
	std::vector<int> Ys;
};
#endif /* DISPLAYABLE_H_ */

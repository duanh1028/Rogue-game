#ifndef ACTION_H_
#define ACTION_H_
#include <string>

class Action {
public:
	void setMessage(std::string _msg);
	void setIntValue(int _v);
	virtual void setCharValue(char _c);

	std::string getMessage();
	int getIntValue();
	char getCharValue();

private:
	std::string msg;
	int v;
	char c;
};
#endif /* ACTION_H_ */
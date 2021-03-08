/*#include "ObjectDisplayGrid.hpp"
#include <string>
#include <iostream>


std::string ObjectDisplayGrid::getObjectDisplayGrid(int gameHeight, int width, int topHeight) {
	std::string out = "gameHeight: " + std::to_string(gameHeight) + "width:" + std::to_string(width) + "topHeight:" + std::to_string(topHeight);
	//std::cout << "getObjectDisplayGrid:" + out << std::endl;
	return out;
}

void ObjectDisplayGrid::setTopMessageHeight(int _topHeight) {
	topHeight = _topHeight;
	//std::cout << "setTopMessageHeight" << std::endl;
}
*/

#include <curses.h>
#include "ObjectDisplayGrid.h"
#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <iostream>

// I am not certain about the threadsafety of ncurse methods.
// They appear to work just fine in my testing, but that is likely as it always sets the cursor position before writing

ObjectDisplayGrid::ObjectDisplayGrid(int _width, int _height, int _messages) : width(_width), height(_height), messages(_messages) {
	// create the 2D array of grid characters
	// note if you want to write messages instead, ncurses
	//reserve 2 on top
	height = height + 2;
	objectGrid = new GridChar * *[width];
	for (int i = 0; i < width; i++) {
		objectGrid[i] = new GridChar * [height];
		for (int j = 0; j < height; j++) {
			objectGrid[i][j] = NULL;
		}
	}

	// initialize ncurses

	// set command window size if running on windows, useful when running in Visual Studio
	// as far as I am aware, no way to do this on linux
#ifdef _WIN32
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD size = { (short)width, (short)(height + messages) };
	SMALL_RECT DisplayArea = { 0, 0, (short)(size.X - 1), (short)(size.Y - 1) };
	SetConsoleScreenBufferSize(handle, size);
	SetConsoleWindowInfo(handle, TRUE, &DisplayArea);
#endif

	// initializes ncurses
	initscr();
	// makes characters typed immediately available, instead of waiting for enter to be pressed
	cbreak();
	// stops typed characters from being shown, makes it easier to get keypresses
	noecho();
	// clears the screen to start
	clear();
}

ObjectDisplayGrid::~ObjectDisplayGrid() {
	// free memory from the dynamically sized object grid
	for (int i = 0; i < width; i++) {
		// delete all character objects in the grid
		for (int j = 0; j < height; j++) {
			delete objectGrid[i][j];
		}
		// delete the column
		delete[] objectGrid[i];
	}
	// delete the array of columns
	delete[] objectGrid;
	objectGrid = NULL;

	// free ncurses data
	endwin();
}

void ObjectDisplayGrid::addObjectToDisplay(GridChar* ch, int x, int y) {
	//2 lines reserve on top
	y = y + 2;
	// note grid objects start from 0,0 and go until width,height
	// x between 0 and width
	if ((0 <= x) && (x < width)) {
		// y between 0 and height
		if ((0 <= y) && (y < height)) {
			// delete existing character if present
			if (objectGrid[x][y] != NULL) {
				delete objectGrid[x][y];
			}

			// add new character to the internal character list
			objectGrid[x][y] = ch;
			// draws the character on the screen, note it is relative to 0,0 of the terminal
			mvaddch(y, x, ch->getChar());
		}
	}
}

void ObjectDisplayGrid::update() {
	// refreshes ncurses
	refresh();
}

void ObjectDisplayGrid::writeLineBottom(int line, std::string message) {
	// messages start from 0, height and go until width,(height + messages)
	mvaddstr(height + line, 0, message.c_str());
	// clear after what we wrote to EOL
	clrtoeol();
}

void ObjectDisplayGrid::writeLineTop(int line, std::string message) {
	// messages start from 0, height and go until width,(height + messages)
	mvaddstr(line, 0, message.c_str());
	// clear after what we wrote to EOL
	clrtoeol();
}

//step 1
std::string ObjectDisplayGrid::getObjectDisplayGrid(int gameHeight, int width, int topHeight) {
	std::string out = "gameHeight: " + std::to_string(gameHeight) + "width:" + std::to_string(width) + "topHeight:" + std::to_string(topHeight);
	//std::cout << "getObjectDisplayGrid:" + out << std::endl;
	return out;
}

void ObjectDisplayGrid::setTopMessageHeight(int _topHeight) {
	topHeight = _topHeight;
	//std::cout << "setTopMessageHeight" << std::endl;
}

//step 2
GridChar*** ObjectDisplayGrid::getObjectGrid() {
	return objectGrid;
}

GridChar* ObjectDisplayGrid::movePlayerLeft(int x, int y, GridChar* prevChar, Dungeon* dungeonBeingParsed, std::vector<Action*> actions) {
	//2 lines reserve on top
	y = y + 2;

	GridChar* wall = new GridChar('X');
	GridChar* originChar = new GridChar('@');

	if ((0 <= x - 1) && (x - 1 < width) && objectGrid[x - 1][y]->getChar() != 'X' && objectGrid[x - 1][y]->getChar() != ' ') {
		// y between 0 and height
		if ((0 <= (y)) && ((y) < height) && objectGrid[x - 1][y]->getChar() != 'X' && objectGrid[x - 1][y]->getChar() != ' ') {
			//reference to the vector
			std::vector<Creature*>* creatures = &(dungeonBeingParsed->getCreatures()); //reference to creatures
			Displayable** theCreature = NULL; //reference to creature
			int i = 0;
			bool meetCreature = false;
			int creatureIndex = 0;
			Displayable** player = NULL;

			for (Creature* creature : *creatures) {
				char type = creature->getType();
				//if creature is not the player, check its position
				if (type != '@') {
					if (creature->getPosX() == x - 1) {
						if (creature->getPosY() == y - 2) {
							Displayable* tmp = (Displayable*)creature;
							theCreature = &(tmp);
							meetCreature = true;
							creatureIndex = i;
						}
					}
				}
				else {
					Displayable* tmp = (Displayable*)creature;
					player = &(tmp);
				}
				i++;
			}

			//case when meet creature
			bool deadMonster = false;
			if (meetCreature) {
				//get the armor and sword of player
				int defense = 0;
				int attack = 0;
				Item* armor = ((Player*)(*player))->getArmor();
				if (armor != NULL) {
					defense = armor->getIntValue();
				}
				Item* sword = ((Player*)(*player))->getWeapon();
				if (sword != NULL) {
					attack = sword->getIntValue();
				}

				//player hit the monster
				//monster hit action
				std::string actionInfo = "Action info: ";
				for (Action* action : actions) {
					if (dynamic_cast<DropPack*> (action) != NULL) {
						//problem: the droppedItem disappeared
						actionInfo += action->getMessage() + " ";
						//std::cout << "22222" << std::endl;

						dropItems((*player), dungeonBeingParsed, '1');
					}
					/*if (dynamic_cast<Teleport*> (action) != NULL) {
						actionInfo += action->getMessage() + " ";
						Displayable* owner = ((CreatureActions*)action)->getOwner();
						//std::cout << "111111111" << std::endl;
						((Displayable*)owner)->setPosX(17);
						((Displayable*)owner)->setPosY(17);
					}*/
				}
				actionInfo += "                                                               ";
				mvaddstr(height + 6, 0, actionInfo.c_str());

				int hitByPlayer = rand() % ((*player)->getMaxHit() + 1);
				int originMonsterHp = (*theCreature)->getHp();
				(*theCreature)->setHp(originMonsterHp - hitByPlayer - attack);
				std::string message = "Monster HP: " + std::to_string(originMonsterHp) + " - " + std::to_string(hitByPlayer) + " - " + std::to_string(attack) + " = " + std::to_string((*theCreature)->getHp()) + "      ";
				mvaddstr(height + 2, 0, message.c_str());
				if ((*theCreature)->getHp() <= 0) {
					(*creatures).erase((*creatures).begin() + creatureIndex);
					deadMonster = true;
					message = "Info: Monster Dead, Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "                                                                                                         ";
					mvaddstr(height + 4, 0, message.c_str());
					std::string message = "Player HP: " + std::to_string((*player)->getHp()) + "                                        ";
					mvaddstr(height + 1, 0, message.c_str());
				}
				else {
					// monster hit back when not dead
					int hitByMonster = rand() % ((*theCreature)->getMaxHit() + 1);

					if (defense < hitByMonster) {
						message = "Info: Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "   Creature -> Player Hit: " + std::to_string(hitByMonster) + "-" + std::to_string(defense) + "                   ";
						mvaddstr(height + 4, 0, message.c_str());
						std::string message = "Player HP: " + std::to_string((*player)->getHp()) + " - (" + std::to_string(hitByMonster) + " - " + std::to_string(defense) + ") = " + std::to_string((*player)->getHp() - (hitByMonster-defense)) + "      ";
						mvaddstr(height + 1, 0, message.c_str());
						(*player)->setHp((*player)->getHp() - (hitByMonster-defense));
					}
					else {
						message = "Info: Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "   Creature -> Player Hit: " + std::to_string(hitByMonster) + "-" + std::to_string(defense) + "                   ";
						std::string message = "Player HP: " + std::to_string((*player)->getHp()) + " - 0 = " + std::to_string((*player)->getHp()) + "                                   ";
						mvaddstr(height + 1, 0, message.c_str());
					}
					message = "HP: " + std::to_string((*player)->getHp()) + "  score : 0           ";
					mvaddstr(0, 0, message.c_str());
				}
			}
			else {
				std::string message = "                                                     ";
				mvaddstr(height + 1, 0, message.c_str());
				mvaddstr(height + 2, 0, message.c_str());
			}

			if (meetCreature & !deadMonster) {
				return originChar;
			}

			if (deadMonster) {
				if (objectGrid[x - 1][y] != NULL) {
					delete objectGrid[x - 1][y];
				}

				// add new character to the internal character list
				GridChar* ch = new GridChar('.');
				objectGrid[x - 1][y] = ch;
				// draws the character on the screen, note it is relative to 0,0 of the terminal
				mvaddch(y, x - 1, objectGrid[x - 1][y]->getChar());
				return originChar;
			}

			// delete existing character if present
			if (objectGrid[x - 1][y] != NULL) {
				//store origin char
				originChar = new GridChar(objectGrid[x - 1][y]->getChar());
				delete objectGrid[x - 1][y];
			}

			// add new character to the internal character list
			GridChar* ch = new GridChar('@');
			objectGrid[x - 1][y] = ch;
			// draws the character on the screen, note it is relative to 0,0 of the terminal
			mvaddch(y, x - 1, objectGrid[x - 1][y]->getChar());

			// note grid objects start from 0,0 and go until width,height
            // x between 0 and width
			if ((0 <= x) && (x < width)) {
				// y between 0 and height
				if ((0 <= y) && (y < height)) {
					// delete existing character if present
					if (objectGrid[x][y] != NULL) {
						delete objectGrid[x][y];
					}

					// add prev character to the internal character list
					//GridChar* ch = new GridChar('.');
					objectGrid[x][y] = prevChar;
					// draws the character on the screen, note it is relative to 0,0 of the terminal
					mvaddch(y, x, objectGrid[x][y]->getChar());
				}
			}

		}
	}

	return originChar;
}


GridChar* ObjectDisplayGrid::movePlayerRight(int x, int y, GridChar* prevChar, Dungeon* dungeonBeingParsed, std::vector<Action*> actions) {
	y = y + 2;

	GridChar* wall = new GridChar('X');
	GridChar* originChar = new GridChar('@');

	if ((0 <= x + 1) && (x + 1 < width) && objectGrid[x + 1][y]->getChar() != 'X' && objectGrid[x + 1][y]->getChar() != ' ') {
		if ((0 <= (y)) && ((y) < height) && objectGrid[x + 1][y]->getChar() != 'X' && objectGrid[x + 1][y]->getChar() != ' ') {
			std::vector<Creature*>* creatures = &(dungeonBeingParsed->getCreatures()); //reference to creatures
			Displayable** theCreature = NULL; //reference to creature
			int i = 0;
			bool meetCreature = false;
			int creatureIndex = 0;
			Displayable** player = NULL;

			for (Creature* creature : *creatures) {
				char type = creature->getType();
				//if creature is not the player, check its position
				if (type != '@') {
					if (creature->getPosX() == x + 1) {
						if (creature->getPosY() == y - 2) {
							Displayable* tmp = (Displayable*)creature;
							theCreature = &(tmp);
							meetCreature = true;
							creatureIndex = i;
						}
					}
				}
				else {
					Displayable* tmp = (Displayable*)creature;
					player = &(tmp);
				}
				i++;
			}
			bool deadMonster = false;
			if (meetCreature) {
				//get the armor and sword of player
				int defense = 0;
				int attack = 0;
				Item* armor = ((Player*)(*player))->getArmor();
				if (armor != NULL) {
					defense = armor->getIntValue();
				}
				Item* sword = ((Player*)(*player))->getWeapon();
				if (sword != NULL) {
					attack = sword->getIntValue();
				}

				//player hit the monster
				//monster hit action
				std::string actionInfo = "Action info: ";
				for (Action* action : actions) {
					if (dynamic_cast<DropPack*> (action) != NULL) {
						//problem: the droppedItem disappeared
						actionInfo += action->getMessage() + " ";
						dropItems((*player), dungeonBeingParsed, '1');
					}
				}
				actionInfo += "                                                               ";
				mvaddstr(height + 6, 0, actionInfo.c_str());


				int hitByPlayer = rand() % ((*player)->getMaxHit() + 1);
				int originMonsterHp = (*theCreature)->getHp();
				(*theCreature)->setHp(originMonsterHp - hitByPlayer - attack);
				std::string message = "Monster HP: " + std::to_string(originMonsterHp) + " - " + std::to_string(hitByPlayer) + " - " + std::to_string(attack) + " = " + std::to_string((*theCreature)->getHp()) + "      ";
				mvaddstr(height + 2, 0, message.c_str());
				if ((*theCreature)->getHp() <= 0) {
					(*creatures).erase((*creatures).begin() + creatureIndex);
					deadMonster = true;
					message = "Info: Monster Dead, Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "                                                                                                         ";
					mvaddstr(height + 4, 0, message.c_str());
					std::string message = "Player HP: " + std::to_string((*player)->getHp()) + "                                        ";
					mvaddstr(height + 1, 0, message.c_str());
				}
				else {
					// monster hit back when not dead
					int hitByMonster = rand() % ((*theCreature)->getMaxHit() + 1);

					if (defense < hitByMonster) {
						message = "Info: Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "   Creature -> Player Hit: " + std::to_string(hitByMonster) + "-" + std::to_string(defense) + "                   ";
						mvaddstr(height + 4, 0, message.c_str());
						std::string message = "Player HP: " + std::to_string((*player)->getHp()) + " - (" + std::to_string(hitByMonster) + " - " + std::to_string(defense) + ") = " + std::to_string((*player)->getHp() - (hitByMonster - defense)) + "      ";
						mvaddstr(height + 1, 0, message.c_str());
						(*player)->setHp((*player)->getHp() - (hitByMonster - defense));
					}
					else {
						message = "Info: Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "   Creature -> Player Hit: " + std::to_string(hitByMonster) + "-" + std::to_string(defense) + "                   ";
						std::string message = "Player HP: " + std::to_string((*player)->getHp()) + " - 0 = " + std::to_string((*player)->getHp()) + "                                   ";
						mvaddstr(height + 1, 0, message.c_str());
					}
					message = "HP: " + std::to_string((*player)->getHp()) + "  score : 0           ";
					mvaddstr(0, 0, message.c_str());
				}
			}
			else {
				std::string message = "                                                     ";
				mvaddstr(height + 1, 0, message.c_str());
				mvaddstr(height + 2, 0, message.c_str());
			}

			if (meetCreature & !deadMonster) {
				return originChar;
			}

			if (deadMonster) {
				if (objectGrid[x - 1][y] != NULL) {
					delete objectGrid[x + 1][y];
				}

				GridChar* ch = new GridChar('.');
				objectGrid[x + 1][y] = ch;
				mvaddch(y, x + 1, objectGrid[x + 1][y]->getChar());
				return originChar;
			}

			if (objectGrid[x - 1][y] != NULL) {
				originChar = new GridChar(objectGrid[x + 1][y]->getChar());
				delete objectGrid[x + 1][y];
			}

			GridChar* ch = new GridChar('@');
			objectGrid[x + 1][y] = ch;
			mvaddch(y, x + 1, objectGrid[x + 1][y]->getChar());


			if ((0 <= x) && (x < width)) {
				if ((0 <= y) && (y < height)) {
					if (objectGrid[x][y] != NULL) {
						delete objectGrid[x][y];
					}

					objectGrid[x][y] = prevChar;
					mvaddch(y, x, objectGrid[x][y]->getChar());
				}
			}

		}
	}

	return originChar;
}

GridChar* ObjectDisplayGrid::movePlayerUp(int x, int y, GridChar* prevChar, Dungeon* dungeonBeingParsed, std::vector<Action*> actions) {
	y = y + 2;

	GridChar* wall = new GridChar('X');
	GridChar* originChar = new GridChar('@');

	if ((0 <= x) && (x < width) && objectGrid[x][y-1]->getChar() != 'X' && objectGrid[x][y-1]->getChar() != ' ') {
		if ((0 <= (y-1)) && ((y-1) < height) && objectGrid[x][y-1]->getChar() != 'X' && objectGrid[x][y-1]->getChar() != ' ') {
			//reference to the vector
			std::vector<Creature*>* creatures = &(dungeonBeingParsed->getCreatures()); //reference to creatures
			Displayable** theCreature = NULL; //reference to creature
			int i = 0;
			bool meetCreature = false;
			int creatureIndex = 0;
			Displayable** player = NULL;

			for (Creature* creature : *creatures) {
				char type = creature->getType();
				//if creature is not the player, check its position
				if (type != '@') {
					if (creature->getPosX() == x) {
						if (creature->getPosY() == y - 3) {
							Displayable* tmp = (Displayable*)creature;
							theCreature = &(tmp);
							meetCreature = true;
							creatureIndex = i;
						}
					}
				}
				else {
					Displayable* tmp = (Displayable*)creature;
					player = &(tmp);
				}
				i++;
			}

			//case when meet creature
			bool deadMonster = false;
			if (meetCreature) {
				//get the armor and sword of player
				int defense = 0;
				int attack = 0;
				Item* armor = ((Player*)(*player))->getArmor();
				if (armor != NULL) {
					defense = armor->getIntValue();
				}
				Item* sword = ((Player*)(*player))->getWeapon();
				if (sword != NULL) {
					attack = sword->getIntValue();
				}

				//player hit the monster
				//monster hit action
				std::string actionInfo = "Action info: ";
				for (Action* action : actions) {
					if (dynamic_cast<DropPack*> (action) != NULL) {
						//problem: the droppedItem disappeared
						actionInfo += action->getMessage() + " ";
						dropItems((*player), dungeonBeingParsed, '1');
					}
				}
				actionInfo += "                                                               ";
				mvaddstr(height + 6, 0, actionInfo.c_str());


				int hitByPlayer = rand() % ((*player)->getMaxHit() + 1);
				int originMonsterHp = (*theCreature)->getHp();
				(*theCreature)->setHp(originMonsterHp - hitByPlayer - attack);
				std::string message = "Monster HP: " + std::to_string(originMonsterHp) + " - " + std::to_string(hitByPlayer) + " - " + std::to_string(attack) + " = " + std::to_string((*theCreature)->getHp()) + "      ";
				mvaddstr(height + 2, 0, message.c_str());
				if ((*theCreature)->getHp() <= 0) {
					(*creatures).erase((*creatures).begin() + creatureIndex);
					deadMonster = true;
					message = "Info: Monster Dead, Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "                                                                                                         ";
					mvaddstr(height + 4, 0, message.c_str());
					std::string message = "Player HP: " + std::to_string((*player)->getHp()) + "                                        ";
					mvaddstr(height + 1, 0, message.c_str());
				}
				else {
					// monster hit back when not dead
					int hitByMonster = rand() % ((*theCreature)->getMaxHit() + 1);

					if (defense < hitByMonster) {
						message = "Info: Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "   Creature -> Player Hit: " + std::to_string(hitByMonster) + "-" + std::to_string(defense) + "                   ";
						mvaddstr(height + 4, 0, message.c_str());
						std::string message = "Player HP: " + std::to_string((*player)->getHp()) + " - (" + std::to_string(hitByMonster) + " - " + std::to_string(defense) + ") = " + std::to_string((*player)->getHp() - (hitByMonster - defense)) + "      ";
						mvaddstr(height + 1, 0, message.c_str());
						(*player)->setHp((*player)->getHp() - (hitByMonster - defense));
					}
					else {
						message = "Info: Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "   Creature -> Player Hit: " + std::to_string(hitByMonster) + "-" + std::to_string(defense) + "                   ";
						std::string message = "Player HP: " + std::to_string((*player)->getHp()) + " - 0 = " + std::to_string((*player)->getHp()) + "                                   ";
						mvaddstr(height + 1, 0, message.c_str());
					}
					message = "HP: " + std::to_string((*player)->getHp()) + "  score : 0           ";
					mvaddstr(0, 0, message.c_str());
				}
			}
			else {
				std::string message = "                                                     ";
				mvaddstr(height + 1, 0, message.c_str());
				mvaddstr(height + 2, 0, message.c_str());
			}

			if (meetCreature & !deadMonster) {
				return originChar;
			}

			if (deadMonster) {
				if (objectGrid[x][y - 1] != NULL) {
					delete objectGrid[x][y - 1];
				}

				GridChar* ch = new GridChar('.');
				objectGrid[x][y - 1] = ch;
				mvaddch(y - 1, x, objectGrid[x][y - 1]->getChar());
				return originChar;
			}

			if (objectGrid[x][y-1] != NULL) {
				originChar = new GridChar(objectGrid[x][y-1]->getChar());
				delete objectGrid[x][y-1];
			}

			GridChar* ch = new GridChar('@');
			objectGrid[x][y-1] = ch;
			mvaddch(y-1, x, objectGrid[x][y-1]->getChar());


			if ((0 <= x) && (x < width)) {
				if ((0 <= y) && (y < height)) {
					if (objectGrid[x][y] != NULL) {
						delete objectGrid[x][y];
					}

					objectGrid[x][y] = prevChar;
					mvaddch(y, x, objectGrid[x][y]->getChar());
				}
			}

		}
	}

	return originChar;
}

GridChar* ObjectDisplayGrid::movePlayerDown(int x, int y, GridChar* prevChar, Dungeon* dungeonBeingParsed, std::vector<Action*> actions) {
	y = y + 2;

	GridChar* wall = new GridChar('X');
	GridChar* originChar = new GridChar('@');

	if ((0 <= x) && (x < width) && objectGrid[x][y + 1]->getChar() != 'X' && objectGrid[x][y + 1]->getChar() != ' ') {
		if ((0 <= (y + 1)) && ((y + 1) < height) && objectGrid[x][y + 1]->getChar() != 'X' && objectGrid[x][y + 1]->getChar() != ' ') {
			//reference to the vector
			std::vector<Creature*>* creatures = &(dungeonBeingParsed->getCreatures()); //reference to creatures
			Displayable** theCreature = NULL; //reference to creature
			int i = 0;
			bool meetCreature = false;
			int creatureIndex = 0;
			Displayable** player = NULL;

			for (Creature* creature : *creatures) {
				char type = creature->getType();
				//if creature is not the player, check its position
				if (type != '@') {
					if (creature->getPosX() == x) {
						if (creature->getPosY() == y - 1) {
							Displayable* tmp = (Displayable*)creature;
							theCreature = &(tmp);
							meetCreature = true;
							creatureIndex = i;
						}
					}
				}
				else {
					Displayable* tmp = (Displayable*)creature;
					player = &(tmp);
				}
				i++;
			}

			//case when meet creature
			bool deadMonster = false;
			if (meetCreature) {
				//get the armor and sword of player
				int defense = 0;
				int attack = 0;
				Item* armor = ((Player*)(*player))->getArmor();
				if (armor != NULL) {
					defense = armor->getIntValue();
				}
				Item* sword = ((Player*)(*player))->getWeapon();
				if (sword != NULL) {
					attack = sword->getIntValue();
				}

				//player hit the monster
				//monster hit action
				std::string actionInfo = "Action info: ";
				for (Action* action : actions) {
					if (dynamic_cast<DropPack*> (action) != NULL) {
						//problem: the droppedItem disappeared
						actionInfo += action->getMessage() + " ";
						dropItems((*player), dungeonBeingParsed, '1');
					}
				}
				actionInfo += "                                                               ";
				mvaddstr(height + 6, 0, actionInfo.c_str());


				int hitByPlayer = rand() % ((*player)->getMaxHit() + 1);
				int originMonsterHp = (*theCreature)->getHp();
				(*theCreature)->setHp(originMonsterHp - hitByPlayer - attack);
				std::string message = "Monster HP: " + std::to_string(originMonsterHp) + " - " + std::to_string(hitByPlayer) + " - " + std::to_string(attack) + " = " + std::to_string((*theCreature)->getHp()) + "      ";
				mvaddstr(height + 2, 0, message.c_str());
				if ((*theCreature)->getHp() <= 0) {
					(*creatures).erase((*creatures).begin() + creatureIndex);
					deadMonster = true;
					message = "Info: Monster Dead, Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "                                                                                                         ";
					mvaddstr(height + 4, 0, message.c_str());
					std::string message = "Player HP: " + std::to_string((*player)->getHp()) + "                                        ";
					mvaddstr(height + 1, 0, message.c_str());
				}
				else {
					// monster hit back when not dead
					int hitByMonster = rand() % ((*theCreature)->getMaxHit() + 1);

					if (defense < hitByMonster) {
						message = "Info: Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "   Creature -> Player Hit: " + std::to_string(hitByMonster) + "-" + std::to_string(defense) + "                   ";
						mvaddstr(height + 4, 0, message.c_str());
						std::string message = "Player HP: " + std::to_string((*player)->getHp()) + " - (" + std::to_string(hitByMonster) + " - " + std::to_string(defense) + ") = " + std::to_string((*player)->getHp() - (hitByMonster - defense)) + "      ";
						mvaddstr(height + 1, 0, message.c_str());
						(*player)->setHp((*player)->getHp() - (hitByMonster - defense));
					}
					else {
						message = "Info: Player -> Monster Hit: " + std::to_string(hitByPlayer) + "+" + std::to_string(attack) + "   Creature -> Player Hit: " + std::to_string(hitByMonster) + "-" + std::to_string(defense) + "                   ";
						std::string message = "Player HP: " + std::to_string((*player)->getHp()) + " - 0 = " + std::to_string((*player)->getHp()) + "                                   ";
						mvaddstr(height + 1, 0, message.c_str());
					}
					message = "HP: " + std::to_string((*player)->getHp()) + "  score : 0           ";
					mvaddstr(0, 0, message.c_str());
				}
			}
			else {
				std::string message = "                                                     ";
				mvaddstr(height + 1, 0, message.c_str());
				mvaddstr(height + 2, 0, message.c_str());
			}

			if (meetCreature & !deadMonster) {
				return originChar;
			}

			if (deadMonster) {
				if (objectGrid[x][y + 1] != NULL) {
					delete objectGrid[x][y + 1];
				}

				GridChar* ch = new GridChar('.');
				objectGrid[x][y + 1] = ch;
				mvaddch(y + 1, x, objectGrid[x][y + 1]->getChar());
				return originChar;
			}

			if (objectGrid[x][y + 1] != NULL) {
				originChar = new GridChar(objectGrid[x][y + 1]->getChar());
				delete objectGrid[x][y + 1];
			}

			GridChar* ch = new GridChar('@');
			objectGrid[x][y + 1] = ch;
			mvaddch(y + 1, x, objectGrid[x][y + 1]->getChar());


			if ((0 <= x) && (x < width)) {
				if ((0 <= y) && (y < height)) {
					if (objectGrid[x][y] != NULL) {
						delete objectGrid[x][y];
					}

					objectGrid[x][y] = prevChar;
					mvaddch(y, x, objectGrid[x][y]->getChar());
				}
			}

		}
	}

	return originChar;
}

GridChar* ObjectDisplayGrid::pickUpItem(Displayable* player, Dungeon* dungeonBeingParsed) {
	GridChar* ch = new GridChar('.');
	std::string info = "Info: there's nothing to pick up                          ";
	mvaddstr(height + 4, 0, info.c_str());

	std::vector<Item*> *items = &(dungeonBeingParsed->getItems()); //reference to items

	if (!(*items).empty()) {
		for (std::vector<Item*>::reverse_iterator itr1 = (*items).rbegin(); itr1 < (*items).rend(); itr1++) {
			int x = (*itr1)->getPosX();
			int y = (*itr1)->getPosY();

			if (player->getPosX() == x) {
				if (player->getPosY() == y) {
					//grid->writeLineBottom(3, "item");
					if (dynamic_cast<Armor*> (*itr1) != NULL) {
						((Player*)player)->setArmor(dynamic_cast<Armor*> (*itr1));
						((Player*)player)->setItem((*itr1));
						std::string message = "Pick Up Armor";
						mvaddstr(height + 1, 0, message.c_str());
						std::string info = "Info: adding " + (*itr1)->getName() + " to the pack                                          ";
						mvaddstr(height + 4, 0, info.c_str());
						(*items).erase((itr1 + 1).base());
						break;
					}
					else if (dynamic_cast<Sword*> (*itr1) != NULL) {
						((Player*)player)->setWeapon(dynamic_cast<Sword*> (*itr1));
						((Player*)player)->setItem((*itr1));
						std::string message = "Pick Up Sword";
						mvaddstr(height + 1, 0, message.c_str());
						std::string info = "Info: adding " + (*itr1)->getName() + " to the pack                                         ";
						mvaddstr(height + 4, 0, info.c_str());
						(*items).erase((itr1 + 1).base());
						break;
					}
					else {
						((Player*)player)->setScroll(dynamic_cast<Scroll*> (*itr1));
						((Player*)player)->setItem((*itr1));
						std::string message = "Pick Up Scroll";
						mvaddstr(height + 1, 0, message.c_str());
						std::string info = "Info: adding " + (*itr1)->getName() + " to the pack                                         ";
						mvaddstr(height + 4, 0, info.c_str());
						(*items).erase((itr1 + 1).base());
						break;
					}
				}
			}
		}
	}

	if (!(*items).empty()) {
		for (std::vector<Item*>::reverse_iterator itr1 = (*items).rbegin(); itr1 < (*items).rend(); itr1++) {
			int x = (*itr1)->getPosX();
			int y = (*itr1)->getPosY();

			if (player->getPosX() == x) {
				if (player->getPosY() == y) {
					if (dynamic_cast<Armor*> (*itr1) != NULL) {
						ch = new GridChar(']');
						break;
					}
					else if (dynamic_cast<Sword*> (*itr1) != NULL) {
						ch = new GridChar(')');
						break;
					}
				}
			}
		}
	}

	return ch;
}


GridChar* ObjectDisplayGrid::dropItems(Displayable* player, Dungeon* dungeonBeingParsed, char num) {
	GridChar* ch = new GridChar('.');
	if (num <= 57 && num >= 49) { //integer 1 ~ 9
		bool dropItem = false;
		std::vector<Item*> items = ((Player*)player)->getItems();
		if (!items.empty()) {
			int i = 0;
			for (Item* item : items) {
				if (i == (num - 49)) {
					dropItem = true;
					Item* droppedItem = items[i];
					((Player*)player)->removeItem(droppedItem);
					droppedItem->setPosX(player->getPosX());
					droppedItem->setPosY(player->getPosY());
					dungeonBeingParsed->addItem(droppedItem);
					if (dynamic_cast<Sword*> (droppedItem) != NULL) {
						ch = new GridChar(')');
						((Player*)player)->removeWeapon(droppedItem);
						//problem: only check for name of weapon, when two weapons have same name: do off weapon for both weapons, even if not the right one
						((Player*)player)->offWeapon(item);
						item->removeOwner();
					}
					if (dynamic_cast<Armor*> (droppedItem) != NULL) {
						ch = new GridChar(']');
						((Player*)player)->removeArmor(droppedItem);
						//problem: only check for name of armor, when two armors have same name: do off armor for both armors, even if not the right one
						((Player*)player)->offArmor(item);
						item->removeOwner();
					}

					std::string info = "Info: drop item " + droppedItem->getName() + "                                                            ";
					mvaddstr(height + 4, 0, info.c_str());
				}
				i++;
			}
		}
		if (!dropItem) {
			std::string info = "Info: invalid item                                                            ";
			mvaddstr(height + 4, 0, info.c_str());
		}
		return ch;
	}
	else {
		std::string info = "Info: invalid item                                                            ";
		mvaddstr(height + 4, 0, info.c_str());
		return ch;
	}
}


void ObjectDisplayGrid::showItems(Displayable* player) {
	std::string empty = "Pack: empty                                                ";
	mvaddstr(height + 3, 0, empty.c_str());

	std::string pack = "Pack: ";

	std::vector<Item*> items = ((Player*)player)->getItems();

	int i = 1;
	for (Item* item : items) {
		pack += std::to_string(i) + ": ";
		pack += item->getName() + "(" + std::to_string(((Displayable*)item)->getIntValue()) + ")" + " ";
		if (dynamic_cast<Sword*> (item) != NULL && item->getOwner() != NULL) {
			pack += "(w) ";
		}
		if (dynamic_cast<Armor*> (item) != NULL && item->getOwner() != NULL) {
			pack += "(a) ";
		}
		i++;
	}

	mvaddstr(height + 3, 0, pack.c_str());
}

void ObjectDisplayGrid::wearArmor(Displayable* player, Dungeon* dungeonBeingParsed, char itemNum) {
	bool validItem = false;
	if (itemNum <= 57 && itemNum >= 49) { //integer 1 ~ 9
		std::vector<Item*> items = ((Player*)player)->getItems();
		int i = 1;
		for (Item* item : items) {
			if (i == itemNum - 48) {
				//item is armor
				if (dynamic_cast<Armor*> (item) != NULL) {
					validItem = true;
					if (((Player*)player)->getArmor() == NULL){
						((Player*)player)->wearArmor(item);
						item->setOwner((Creature*)player);
						std::string info = "Info: wearing " + item->getName() + "                                                            ";
						mvaddstr(height + 4, 0, info.c_str());
					}
					else {
						std::string info = "Info: already wearing armor                                                            ";
						mvaddstr(height + 4, 0, info.c_str());
					}
				}
			}
			i++;
		}
	}
	if (validItem == false) {
		std::string info = "Info: invalid item                                                            ";
		mvaddstr(height + 4, 0, info.c_str());
	}
}

void ObjectDisplayGrid::takeOffArmor(Displayable* player, Dungeon* dungeonBeingParsed) {
	Item* item = ((Player*)player)->getArmor();
	if (item == NULL) {
		std::string info = "Info: no armor is being worn                                                            ";
		mvaddstr(height + 4, 0, info.c_str());
	}
	else {
		//problem: only check for name of armor, when two armors have same name: do off armor for both armors, even if not the right one
		((Player*)player)->offArmor(item);
		item->removeOwner();
		std::string info = "Info: taking off " + item->getName() + "                                                            ";
		mvaddstr(height + 4, 0, info.c_str());
	}
}


void ObjectDisplayGrid::wearWeapon(Displayable* player, Dungeon* dungeonBeingParsed, char itemNum) {
	bool validItem = false;
	if (itemNum <= 57 && itemNum >= 49) { //integer 1 ~ 9
		std::vector<Item*> items = ((Player*)player)->getItems();
		int i = 1;
		for (Item* item : items) {
			if (i == itemNum - 48) {
				//item is weapon
				if (dynamic_cast<Sword*> (item) != NULL) {
					validItem = true;
					if (((Player*)player)->getWeapon() == NULL) {
						((Player*)player)->wearWeapon(item);
						item->setOwner((Creature*)player);
						std::string info = "Info: wearing " + item->getName() + "                                                            ";
						mvaddstr(height + 4, 0, info.c_str());
					}
					else {
						std::string info = "Info: already having sword                                                            ";
						mvaddstr(height + 4, 0, info.c_str());
					}
				}
			}
			i++;
		}
	}
	if (validItem == false) {
		std::string info = "Info: invalid item                                                            ";
		mvaddstr(height + 4, 0, info.c_str());
	}
}

int ObjectDisplayGrid::readScroll(Displayable* player, Dungeon* dygeonBeingParsed, char itemNum, std::vector<Action*> actions) {
	bool validItem = false;
	if (itemNum <= 57 && itemNum >= 49) { //integer 1 ~ 9
		std::vector<Item*> items = ((Player*)player)->getItems();
		int i = 1;
		for (Item* item : items) {
			if (i == itemNum - 48) {
				//item is scorll
				if (dynamic_cast<Scroll*> (item) != NULL) {
					validItem = true;
					((Player*)player)->removeScroll();
					((Player*)player)->removeItem(item);
					std::string info = "Info: reading scroll                                                             ";
					mvaddstr(height + 4, 0, info.c_str());
					//find the scroll action
					for (Action* action : actions) {
						//bless/curse
						std::string act1 = "You have picked up a scroll of weakened armor";
						std::string act2 = action->getMessage();
						if (act1.compare(act2) == 0) { //bless armor
							if (action->getCharValue() == 'a') {
								std::string actionInfo = "Action info: +5 Armor cursed! -5 taken from its effectiveness";
								mvaddstr(height + 6, 0, actionInfo.c_str());
								for (Item* item : items) {
									//problem
									if (dynamic_cast<Armor*> (item) != NULL) {
										item->setIntValue(0);
									}
								}
							}
							return 0;
						}
						//hullicination
						act1 = "You have picked up a scroll of hallucination!";
						if (act1.compare(act2) == 0) { //bless armor
							int moves = action->getIntValue();	
							std::string actionInfo = "Action info: " + act2 + "             ";
							mvaddstr(height + 6, 0, actionInfo.c_str());
							std::string info = "Info: Hallucination will last for " + std::to_string(moves) + " moves              ";
							mvaddstr(height + 4, 0, info.c_str());
							return moves;
						}
					}
				}
			}
			i++;
		}
	}
	if (validItem == false) {
		std::string info = "Info: invalid item                                                            ";
		mvaddstr(height + 4, 0, info.c_str());
	}
}

void ObjectDisplayGrid::deadPlayer(Displayable* player, Dungeon* dygeonBeingParsed) {
	int x = player->getPosX();
	int y = player->getPosY();
	y = y + 2;

	GridChar* ch = new GridChar('+');
	objectGrid[x][y] = ch;
	mvaddch(y, x, objectGrid[x][y]->getChar());
}
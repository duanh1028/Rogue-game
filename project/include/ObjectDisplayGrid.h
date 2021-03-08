#pragma once
#include <string>
#include "GridChar.h"
#include "Dungeon.hpp"
#include "Player.hpp"
#include "Armor.hpp"
#include "Sword.hpp"
#include "Scroll.hpp"
#include "DropPack.hpp"
#include "Teleport.hpp"
#include "CreatureActions.hpp"
#include "ChangedDisplayedType.hpp"
#include "EndGame.hpp"
#include "UpdateDisplay.hpp"

class ObjectDisplayGrid {
private:
	/** Keeps track of the characters on the screen */
	GridChar*** objectGrid = NULL;
	/** Keeps track of the consoles width and height */
	int height, width;

	/** Number of lines for message writing */
	int messages;

	//step 1
	int gameHeight;
	//int width;
	int topHeight;

public:
	/**
	 * Creates a new display grid using the given parameters and initializes ncurses.
	 * Screen height will be grid height + messages
	 * @param width     Screen width
	 * @param height    Grid height
	 * @param messages  Number of lines to reserve in the message area
	 */
	ObjectDisplayGrid(int width, int height, int messages);

	/** Object deconstructor, to delete the grid character matrix and free ncurses data */
	~ObjectDisplayGrid();

	/**
	 * Refreshes the grid display
	 */
	virtual void update();

	/**
	 * Adds an object to the display grid
	 * @param ch  Object to display
	 * @param x   X position
	 * @param y   Y position
	 */
	virtual void addObjectToDisplay(GridChar* ch, int x, int y);

	/**
	 * Writes a line of text to the screen at the given line relative to the bottom
	 * @param line    line number
	 * @param message message to write
	 * @param update  If true, immediately updates the screen
	 */
	virtual void writeLineTop(int line, std::string message);
	virtual void writeLineBottom(int line, std::string message);

	//step 1
	std::string getObjectDisplayGrid(int gameHeight, int width, int topHeight);
	void setTopMessageHeight(int _topHeight);

	//step2
	GridChar*** getObjectGrid();

	GridChar* movePlayerLeft(int x, int y, GridChar* prevChar, Dungeon* dungeonBeingParsed, std::vector<Action*> actions);
	GridChar* movePlayerRight(int x, int y, GridChar* prevChar, Dungeon* dungeonBeingParsed, std::vector<Action*> actions);
	GridChar* movePlayerUp(int x, int y, GridChar* prevChar, Dungeon* dungeonBeingParsed, std::vector<Action*> actions);
	GridChar* movePlayerDown(int x, int y, GridChar* prevChar, Dungeon* dungeonBeingParsed, std::vector<Action*> actions);

	//step3
	GridChar* pickUpItem(Displayable* player, Dungeon* dungeonBeingParsed);
	void showItems(Displayable* player);
	GridChar* dropItems(Displayable* player, Dungeon* dungeonBeingParsed, char num);

	//step4
	void wearArmor(Displayable* player, Dungeon* dungeonBeingParsed, char item);
	void takeOffArmor(Displayable* player, Dungeon* dungeonBeingParsed);
	void wearWeapon(Displayable* player, Dungeon* dungeonBeingParsed, char item);
	int readScroll(Displayable* player, Dungeon* dungeonBeingParsed, char item, std::vector<Action*> actions);

	void deadPlayer(Displayable* player, Dungeon* dygeonBeingParsed);
};


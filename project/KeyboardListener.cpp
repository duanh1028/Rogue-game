#include "KeyboardListener.h"
#include <sstream>
#include <iostream>
#include <stdio.h>

KeyboardListener::KeyboardListener(ObjectDisplayGrid* _grid, Displayable* _player, Dungeon* _dungeonBeingParsed, std::vector<Action*> _actions) : grid(_grid), player(_player), dungeonBeingParsed(_dungeonBeingParsed), actions(_actions) {}

void KeyboardListener::run() {
	//grid->writeLine(0, "Press 'x' to exit");
	//grid->writeLine(1, "Press w/a/s/d to move");
	running = true;
	char input;
	bool successMove;
	GridChar* originChar = new GridChar('.');

	//in case the player reach the wall and originChar becomes '@'
	GridChar* backupChar = new GridChar('.');
	int hpMoves = player->getHpMove();
	//std::cout << std::to_string(hpMoves) << std::endl;
	int moves = 0;
	int hallucinateMoves = -1;

	ObjectDisplayGrid* copy = new ObjectDisplayGrid(*grid);

	do {
		// wait for next input
		// lowercase so 'x' and 'X' are treated as the same
		input = std::tolower(getchar());

		switch (input) {
			// press X to stop
		case 'e':
			grid->writeLineTop(1, "Are you sure that you want to exit? (Y|y)  ");
			grid->update();
			input = std::tolower(getchar());
			if (input == 'y') {
				running = false;
				break;
			}
			else {
				//grid->writeLine(4, "Exiting...");
				break;
			}
		case 'k': //up
			moves += 1;
			if (moves == hpMoves) {
				moves = 0;
				player->setHp(player->getHp() + 1);
				grid->writeLineTop(0, "HP: " + std::to_string((player)->getHp()) + "  score : 0             ");
			}
			backupChar = originChar;
			originChar = grid->movePlayerUp(player->getPosX(), player->getPosY(), originChar, dungeonBeingParsed, actions);
			if (originChar->getChar() != '@') {
				player->setPosY(player->getPosY() - 1);
			}
			else {
				//when player doesn't move, restore the originChar
				originChar = backupChar;
			}
			break;
		case 'h': //left
			moves += 1;
			if (moves == hpMoves) {
				moves = 0;
				player->setHp(player->getHp() + 1);
				grid->writeLineTop(0, "HP: " + std::to_string((player)->getHp()) + "  score : 0             ");
			}
			backupChar = originChar;
			originChar = grid->movePlayerLeft(player->getPosX(), player->getPosY(), originChar, dungeonBeingParsed, actions);
			if (originChar->getChar() != '@') {
				player->setPosX(player->getPosX() - 1);
			}
			else {
				originChar = backupChar;
			}
			break;
		case 'j': //down
			moves += 1;
			if (moves == hpMoves) {
				moves = 0;
				player->setHp(player->getHp() + 1);
				grid->writeLineTop(0, "HP: " + std::to_string((player)->getHp()) + "  score : 0             ");
			}
			backupChar = originChar;
			originChar = grid->movePlayerDown(player->getPosX(), player->getPosY(), originChar, dungeonBeingParsed, actions);
			if (originChar->getChar() != '@') {
				player->setPosY(player->getPosY() + 1);
			}
			else {
				originChar = backupChar;
			}
			break;
		case 'l': //right
			moves += 1;
			if (moves == hpMoves) {
				moves = 0;
				player->setHp(player->getHp() + 1);
				grid->writeLineTop(0, "HP: " + std::to_string((player)->getHp()) + "  score : 0             ");
			}
			backupChar = originChar;
			originChar = grid->movePlayerRight(player->getPosX(), player->getPosY(), originChar, dungeonBeingParsed, actions);
			if (originChar->getChar() != '@') {
				player->setPosX(player->getPosX() + 1);
			}
			else {
				originChar = backupChar;
			}
			break;
		case 'p': //pick up item
			originChar = grid->pickUpItem(player, dungeonBeingParsed);
			break;
		case 'd': //drop items
			input = std::tolower(getchar());
			originChar = grid->dropItems(player, dungeonBeingParsed, input);
			break;
		case 'i': //inventory
			grid->showItems(player);
			break;
		case 'w':
			input = std::tolower(getchar());
			grid->wearArmor(player, dungeonBeingParsed, input);
			break;
		case 'c':
			grid->takeOffArmor(player, dungeonBeingParsed);
			break;
		case 't':
			input = std::tolower(getchar());
			grid->wearWeapon(player, dungeonBeingParsed, input);
			break;
		case 'r':
			input = std::tolower(getchar());
			hallucinateMoves = grid->readScroll(player, dungeonBeingParsed, input, actions);\
			break;
		default:
			// C is not happy about appending a character to a string apparently
			std::string message = "Unknown key '";
			message += input;
			//grid->writeLine(4, message + "'");
			break;
		}

		/*
		for (Creature* creature : dungeonBeingParsed->getCreatures()) {
			int x = creature->getPosX();
			int y = creature->getPosY();
			char type = creature->getType();

			//if creature is not the player, check its position
			if (type != '@') {
				//objectGrid = pGrid->getObjectGrid();
				if (player->getPosX() == x) {
					if (player->getPosY() == y) {
						grid->writeLineBottom(3, "creature");
					}
				}
			}
		}
		for (Item* item : dungeonBeingParsed->getItems()) {
			int x = item->getPosX();
			int y = item->getPosY();

			if (player->getPosX() == x) {
				if (player->getPosY() == y) {
					grid->writeLineBottom(3, "item");
				}
			}
		}*/
		//grid->writeLineBottom(3, "x: " + std::to_string(player->getPosX()));
		//grid->writeLineBottom(4, "y: " + std::to_string(player->getPosY()));
		if (player->getHp() < 0) {
			grid->writeLineTop(1, "Player died");
			//add death action for step 4
			std::string actionInfo = "Action info: ";
			for (Action* action : actions) {
				if (dynamic_cast<ChangedDisplayedType*> (action) != NULL) {
					actionInfo += action->getMessage() + " ";
					grid->deadPlayer(player, dungeonBeingParsed);
				}
				if (dynamic_cast<UpdateDisplay*> (action) != NULL) {
					actionInfo += action->getMessage() + " ";
				}
				if (dynamic_cast<EndGame*> (action) != NULL) {
					actionInfo += action->getMessage() + " ";
				}
			}
			actionInfo += "                                                               ";
			grid->writeLineBottom(6, actionInfo);
		}

		if (hallucinateMoves >= 0) {
			std::string info = "Info: Hallucination will last for " + std::to_string(hallucinateMoves) + " moves              ";
			grid->writeLineBottom(4, info.c_str());
			std::vector<Room*> rooms = dungeonBeingParsed->getRooms();
			Room* room = rooms[0];
			int x = room->getPosX();
			int y = room->getPosY();
			int width = room->getWidth();
			int height = room->getHeight();

			char characters[6] = { '.','X','T','H','S','@' };

			for (int i = 0; i < width; i++) {
				for (int j = 0; j < height; j++) {
					grid->addObjectToDisplay(new GridChar(characters[rand() % 6]), i, j);
				}
			}
			hallucinateMoves -= 1;
			if (hallucinateMoves == -1) {
				for (int i = x; i < width + x; i++) {
					for (int j = y; j < height + y; j++) {
						char c;
						if ((j == y) | (j == y + height - 1)) {
							c = 'X';
						}
						else if ((i == x) | (i == x + width - 1)) {
							c = 'X';
						}
						else {
							c = '.';
						}
						grid->addObjectToDisplay(new GridChar(c), i, j);
					}
				}
				for (Creature* creature : dungeonBeingParsed->getCreatures()) {
					//Room* room = dungeonBeingParsed->getRoom();
					int x = creature->getPosX();
					int y = creature->getPosY();
					char type = creature->getType();

					GridChar*** gridChar = grid->getObjectGrid();
					if (gridChar[x][y + 2]->getChar() != 'X') { //change y to y+2 for 2 lines reserve on top
						grid->addObjectToDisplay(new GridChar(type), x, y);
					}
				}
				grid->addObjectToDisplay(new GridChar('@'), 1, 3);
				player->setPosX(1);
				player->setPosY(3);
				originChar = new GridChar('.');
			}
		}
		else {
			std::string info = "                                                           ";
			grid->writeLineBottom(4, info.c_str());
		}

		grid->update();

	} while (running && input != EOF);
}
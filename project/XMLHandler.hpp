#ifndef XMLHANDLER_H_
#define XMLHANDLER_H_

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "Dungeon.hpp"
#include "Creature.hpp"
#include "Action.hpp"
#include "Armor.hpp"
#include "CreatureActions.hpp"
#include "Displayable.hpp"
#include "Item.hpp"
#include "ItemActions.hpp"
#include "Magic.hpp"
#include "Monster.hpp"
//#include "ObjectDisplayGrid.hpp"
#include "ObjectDisplayGrid.h"
#include "Passage.hpp"
#include "Player.hpp"
#include "Room.hpp"
#include "Scroll.hpp"
#include "Structure.hpp"
#include "Sword.hpp" 
#include "Remove.hpp"
#include "YouWin.hpp"
#include "UpdateDisplay.hpp"
#include "Teleport.hpp"
#include "ChangedDisplayedType.hpp"
#include "EndGame.hpp"
#include "DropPack.hpp"
#include "BlessCurseOwner.hpp"
#include "Hallucinate.hpp"

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/Attributes.hpp>

class XMLHandler : public xercesc::DefaultHandler {
private:
	int DEBUG = 1;
	std::string CLASSID = "XMLHandler";
	std::string data;

	bool bposX = false;
	bool bposY = false;
	bool bwidth = false;
	bool bheight = false;
	bool bhp = false;
	bool bhpMoves = false;
	bool bmaxHit = false;
	bool bvisible = false;
	bool btype = false;
	bool bitemIntValue = false;

	bool bactionIntValue = false;
	bool bactionCharValue = false;
	bool bactionMessage = false;

	Dungeon* dungeonBeingParsed = NULL;
	ObjectDisplayGrid* objectDisplayGridBeingParsed = NULL;

	//stack store displayables
	std::vector<Displayable*> displayables;

	//Action* actionBeingParsed = NULL;
	//stack store actions
	std::vector<Action*> actions;

	//void addAction(Action action);

public:
	XMLHandler();
	void startElement(const XMLCh* uri, const XMLCh* localName, const XMLCh* qName, const xercesc::Attributes& attributes);
	void endElement(const XMLCh* uri, const XMLCh* localName, const XMLCh* qName);
	void characters(const XMLCh* const ch, const XMLSize_t length);
	void fatalError(const xercesc::SAXParseException& exception);
	std::string toString();

	//add for step 2
	Dungeon* getDungeonBeingParsed();

	std::vector<Action*>& getActions();

};
#endif // XMLHANDLER_H_
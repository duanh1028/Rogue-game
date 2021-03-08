#include "XMLHandler.hpp"

#include <atomic> 
#include <thread>
#include <sstream>
#include "ObjectDisplayGrid.h"
#include "GridChar.h"
#include "KeyboardListener.h"

int case_insensitive_match(std::string s1, std::string s2) {
    //convert s1 and s2 into lower case strings
    std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower); //std overloads tolower, ::tolower is the definition in the global namespace
    std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
    if (s1.compare(s2) == 0)
        return 1;
    return 0;
}

inline std::string boolToString(bool booleanValue) {
    return booleanValue ? "true" : "false";
}
/*
std::vector<Dungeon> XMLHandler::getDungeons() {
    return dungeons;
}*/

std::vector<Action*>& XMLHandler::getActions() {
    return actions;
}

XMLHandler::XMLHandler() {}

const XMLCh* getXMLChAttributeFromString(const xercesc::Attributes& attributes, const char* strGet) {
    XMLCh* xmlChGet = xercesc::XMLString::transcode(strGet);
    const XMLCh* xmlChAttr = attributes.getValue(xmlChGet);
    xercesc::XMLString::release((&xmlChGet));
    return xmlChAttr;
}

std::string xmlChToString(const XMLCh* xmlChName, int length = -1) {
    //Xerces Parses file into XMLCh* string. So use Transcode to allocate a char* buffer
    char* chStarName = xercesc::XMLString::transcode(xmlChName);
    if (length == -1) {
        std::string StrName(chStarName);
        xercesc::XMLString::release(&chStarName);
        return StrName;
    }
    else {
        std::string StrName(chStarName, 0, length);
        xercesc::XMLString::release(&chStarName);
        return StrName;
    }

}

bool inPassage = false; //when in passage, need to store posX and posY in vector

void XMLHandler::startElement(const XMLCh* uri, const XMLCh* localName, const XMLCh* qName, const xercesc::Attributes& attributes) {
    
    char* qNameStr = xercesc::XMLString::transcode(qName);
    //displayables
    if (case_insensitive_match(qNameStr, "Dungeon")) { //problem: bottomheight not used 
        //int bottomHeight = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "bottomHeight")));
        int gameHeight = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "gameHeight")));
        int topHeight = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "topHeight")));
        int width = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "width")));
        std::string dungeonName = xmlChToString(getXMLChAttributeFromString(attributes, "name"));
        //question: is getObjectDisplayGrid actually setter? -yes
        Dungeon* dungeon = new Dungeon(dungeonName, width, gameHeight);
        dungeonBeingParsed = dungeon;
        dungeonBeingParsed->getDungeon(dungeonName, width, gameHeight);
        //Action* action = new Action();
        //actionBeingParsed = action;
    }
    else if (case_insensitive_match(qNameStr, "Rooms")) { }
    else if (case_insensitive_match(qNameStr, "Room")) {
        std::string roomname = xmlChToString(getXMLChAttributeFromString(attributes, "room"));
        int id = std::stoi(roomname);
        //question: the string in room constructor? -type-cast
        Room*  room = new Room(roomname);
        room->setID(id);
        dungeonBeingParsed->addRoom(room);
        displayables.push_back(room);
        //newroom = 1;
    }
    else if (case_insensitive_match(qNameStr, "Passage")) {
        int passageRoom1 = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "room1")));
        int passageRoom2 = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "room2")));
        Passage* passage = new Passage();
        passage->setID(passageRoom1, passageRoom2);
        dungeonBeingParsed->addPassage(passage);
        displayables.push_back(passage);
        inPassage = true;
    }
    else if (case_insensitive_match(qNameStr, "Player")) {
        std::string playerName = xmlChToString(getXMLChAttributeFromString(attributes, "name"));
        int playerRoom = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "room")));
        int playerSerial = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "serial")));
        Player* player = new Player();
        player->setType('@');
        dungeonBeingParsed->addCreature((Creature*)player);
        displayables.push_back(player);
    }
    else if (case_insensitive_match(qNameStr, "Monster")) {
        std::string monsterName = xmlChToString(getXMLChAttributeFromString(attributes, "name"));
        int monsterRoom = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "room")));
        int monsterSerial = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "serial")));
        Monster* monster = new Monster();
        monster->setName(monsterName);
        monster->setID(monsterRoom, monsterSerial);
        dungeonBeingParsed->addCreature((Creature*)monster);
        displayables.push_back(monster);
        //newmonster = 1;
    }
    else if (case_insensitive_match(qNameStr, "Scroll")) {
        std::string scrollName = xmlChToString(getXMLChAttributeFromString(attributes, "name"));
        int scrollRoom = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "room")));
        int scrollSerial = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "serial")));
        Scroll* scroll = new Scroll(scrollName);
        scroll->setID(scrollRoom, scrollSerial);
        dungeonBeingParsed->addItem((Item*)scroll);
        displayables.push_back(scroll);
    }
    else if (case_insensitive_match(qNameStr, "Armor")) {
        std::string armorName = xmlChToString(getXMLChAttributeFromString(attributes, "name"));
        int armorRoom = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "room")));
        int armorSerial = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "serial")));
        Armor* armor = new Armor(armorName);
        armor->setID(armorRoom, armorSerial);
        /*
        Displayable* tmp = displayables.back();
        if (dynamic_cast<Player*>(tmp) != NULL) {
            ((Item*)armor)->setOwner((Creature*)tmp);
        }*/
        dungeonBeingParsed->addItem((Item*)armor);
        displayables.push_back(armor);
    }
    else if (case_insensitive_match(qNameStr, "Sword")) {
        std::string swordName = xmlChToString(getXMLChAttributeFromString(attributes, "name"));
        int swordRoom = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "room")));
        int swordSerial = std::stoi(xmlChToString(getXMLChAttributeFromString(attributes, "serial")));
        Sword* sword = new Sword(swordName);
        sword->setID(swordRoom, swordSerial);
        dungeonBeingParsed->addItem((Item*)sword);
        displayables.push_back(sword);
    }
    else if (case_insensitive_match(qNameStr, "posX")) {
        bposX = true;
    }
    else if (case_insensitive_match(qNameStr, "posY")) {
        bposY = true;
    }
    else if (case_insensitive_match(qNameStr, "width")) {
        bwidth = true;
    }
    else if (case_insensitive_match(qNameStr, "height")) {
        bheight = true;
    }
    else if (case_insensitive_match(qNameStr, "hp")) {
        bhp = true;
    }
    else if (case_insensitive_match(qNameStr, "hpMoves")) {
        bhpMoves = true;
    }
    else if (case_insensitive_match(qNameStr, "maxhit")) {
        bmaxHit = true;
    }
    else if (case_insensitive_match(qNameStr, "ItemIntValue")) {
        bitemIntValue = true;
    }
    else if (case_insensitive_match(qNameStr, "visible")) {
        bvisible = true;
    }
    else if (case_insensitive_match(qNameStr, "type")) {
        btype = true;
    }
    //actions
    else if (case_insensitive_match(qNameStr, "CreatureAction")) {
        std::string name = xmlChToString(getXMLChAttributeFromString(attributes, "name"));
        std::string type = xmlChToString(getXMLChAttributeFromString(attributes, "type"));
        if (name.compare("EndGame") == 0) {
            EndGame* endgame = new EndGame(name, (Creature*)displayables.back());
            actions.push_back(endgame);
        }
        if (name.compare("Remove") == 0) {
            Remove* remove = new Remove(name, (Creature*)displayables.back());
            actions.push_back(remove);
        }
        if (name.compare("YouWin") == 0) {
            YouWin* youwin = new YouWin(name, (Creature*)displayables.back());
            actions.push_back(youwin);
        }
        if (name.compare("UpdateDisplay") == 0) {
            UpdateDisplay* updatedisplay = new UpdateDisplay(name, (Creature*)displayables.back());
            actions.push_back(updatedisplay);
        }
        if (name.compare("Teleport") == 0) {
            Teleport* teleport = new Teleport(name, (Creature*)displayables.back());
            actions.push_back(teleport);
        }
        if (name.compare("ChangeDisplayedType") == 0) {
            ChangedDisplayedType* changeddisplayedtype = new ChangedDisplayedType(name, (Creature*)displayables.back());
            actions.push_back(changeddisplayedtype);
        }
        if (name.compare("DropPack") == 0) {
            DropPack* droppack = new DropPack(name, (Creature*)displayables.back());
            actions.push_back(droppack);
        }
    }
    else if (case_insensitive_match(qNameStr, "ItemAction")) {
        std::string name = xmlChToString(getXMLChAttributeFromString(attributes, "name"));
        std::string type = xmlChToString(getXMLChAttributeFromString(attributes, "type"));
        if (name.find("Bless") | name.find("Curse")) {
            BlessCurseOwner* blesscurse = new BlessCurseOwner((Creature*)displayables.back());
            actions.push_back(blesscurse);
        }
        if (name.compare("Hallucinate") == 0) {
            Hallucinate* hallucinate = new Hallucinate((Creature*)displayables.back());
            actions.push_back(hallucinate);
        }
    }
    else if (case_insensitive_match(qNameStr, "actionCharValue")) {
        bactionCharValue = true;
    }
    else if (case_insensitive_match(qNameStr, "actionIntValue")) {
        bactionIntValue = true;
    }
    else if (case_insensitive_match(qNameStr, "actionMessage")) {
        bactionMessage = true;
    }
    else {
        //std::cout << "Unknown qname: " << qNameStr << std::endl;
    }

    xercesc::XMLString::release(&qNameStr);
}

std::string displayType;
int x = 0, y = 0, height = 0, width = 0;
char type = 0;
void XMLHandler::endElement(const XMLCh* uri, const XMLCh* localName, const XMLCh* qName)
{
    if (bvisible) {
        if (std::stoi(data) == 1) {
            displayables.back()->setVisible();
        } else {
            displayables.back()->setInvisible();
        }
        bvisible = false;
    }
    else if (bmaxHit) {
        displayables.back()->setMaxHit(std::stoi(data));
        bmaxHit = false;
    }
    else if (bhpMoves) {
        displayables.back()->setHpMove(std::stoi(data));
        bhpMoves = false;
    }
    else if (bhp) {
        displayables.back()->setHp(std::stoi(data));
        bhp = false;
    }
    else if (btype) {
        type = data[0];
        displayables.back()->setType(type);
        btype = false;
    }
    else if (bitemIntValue) {
        displayables.back()->setIntValue(std::stoi(data));
        bitemIntValue = false;
    }
    else if (bposX) {
        x = std::stoi(data);
        if (inPassage) {
            displayables.back()->addX(x);
        }
        else {
            displayables.back()->setPosX(x);
        }
        bposX = false;
    }
    else if (bposY) {
        y = std::stoi(data);
        if (inPassage) {
            displayables.back()->addY(y);
        }
        else {
            displayables.back()->setPosY(y);
        }
        bposY = false;
    }
    else if (bwidth) {
        width = std::stoi(data);
        displayables.back()->setWidth(width);
        bwidth = false;
    }
    else if (bheight) {
        height = std::stoi(data);
        displayables.back()->setHeight(height);
        bheight = false;
    }
    else if (bactionCharValue) {
        type = data[0];
        actions.back()->setCharValue(type);
        bactionCharValue = false;
    }
    else if (bactionMessage) {
        actions.back()->setMessage(data);
        bactionMessage = false;
    }
    else if (bactionIntValue) {
        actions.back()->setIntValue(std::stoi(data));
        bactionIntValue = false;
    }
 
    char* qNameStr = xercesc::XMLString::transcode(qName);
    if (case_insensitive_match(qNameStr, "Dungeon")) {
        //dungeonBeingParsed = nullptr;
        //objectDisplayGridBeingParsed = nullptr;
    }
    else if (case_insensitive_match(qNameStr, "Rooms")) {}
    else if (case_insensitive_match(qNameStr, "Room")) {
        displayables.pop_back();
    }
    else if (case_insensitive_match(qNameStr, "Passage")) {
        displayables.pop_back();
        inPassage = false;
    }
    else if (case_insensitive_match(qNameStr, "Player")) {
        Displayable* player = displayables.back();
        displayables.pop_back();
        Displayable* room = displayables.back();
        //set pos for player
        player->setPosX(player->getPosX() + room->getPosX());
        player->setPosY(player->getPosY() + room->getPosY());
    }
    else if (case_insensitive_match(qNameStr, "Monster")) {
        Displayable* monster = displayables.back();
        displayables.pop_back();
        Displayable* room = displayables.back();
        //set pos for monster
        monster->setPosX(monster->getPosX() + room->getPosX());
        monster->setPosY(monster->getPosY() + room->getPosY());
    }
    else if (case_insensitive_match(qNameStr, "Scroll")) {
        Displayable* scroll = displayables.back();
        displayables.pop_back();
        Displayable* room = displayables.back();
        //set pos for scroll
        scroll->setPosX(scroll->getPosX() + room->getPosX());
        scroll->setPosY(scroll->getPosY() + room->getPosY());
    }
    else if (case_insensitive_match(qNameStr, "Armor")) {
        Displayable* armor = displayables.back();
        displayables.pop_back();
        Displayable* player = displayables.back();
        if (dynamic_cast<Player*> (player) != NULL) {
            //((Item*)armor)->setOwner((Creature*)player);
            //((Player*)player)->wearArmor((Item*)armor);
            ((Player*)player)->setArmor((Item*)armor);
            ((Player*)player)->setItem((Item*)armor);
            //std::cout << "setarmor" << std::endl;
        }
    }
    else if (case_insensitive_match(qNameStr, "Sword")) {
        Displayable* sword = displayables.back();
        displayables.pop_back();
        Displayable* player = displayables.back();
        if (dynamic_cast<Player*> (player) != NULL) {
            //((Item*)sword)->setOwner((Creature*)player);
            //((Player*)player)->wearWeapon((Item*)sword);
            ((Player*)player)->setWeapon((Item*)sword);
            ((Player*)player)->setItem((Item*)sword);
            //std::cout << "setsword" << std::endl;
        }
    }
    xercesc::XMLString::release(&qNameStr);

}
/*
void XMLHandler::addDungeon(Dungeon dungeon) {
    dungeons.push_back(dungeon);
}*/
/*
void XMLHandler::addAction(Action action) {
    actions.push_back(action);
}*/

void XMLHandler::characters(const XMLCh* const ch, const XMLSize_t length) {
    data = xmlChToString(ch, (int)length);

}

void XMLHandler::fatalError(const xercesc::SAXParseException& exception)
{
    char* message = xercesc::XMLString::transcode(exception.getMessage());
    std::cout << "Fatal Error: " << message
        << " at line: " << exception.getLineNumber()
        << std::endl;
    xercesc::XMLString::release(&message);
}


std::string XMLHandler::toString() {
    std::string str = "XMLHandler\n";/*
    str += "   maxStudents: " + std::to_string(maxStudents) + "\n";
    str += "   studentCount: " + std::to_string(studentCount) + "\n";
    for (Student student : students) {
        str += student.toString() + "\n";
    }
    str += "   studentBeingParsed: " + studentBeingParsed->toString() + "\n";
    str += "   activityBeingParsed: " + activityBeingParsed->toString() + "\n";
    str += "   bInstructor: " + boolToString(bInstructor) + "\n";
    str += "   bCredit: " + boolToString(bCredit) + "\n";
    str += "   bName: " + boolToString(bName) + "\n";
    str += "   bNumber: " + boolToString(bNumber) + "\n";
    str += "   bLocation: " + boolToString(bLocation) + "\n";
    str += "   bMeetingTime: " + boolToString(bMeetingTime) + "\n";
    str += "   bMeetingDay: " + boolToString(bMeetingDay) + "\n";*/
    return str;
}

Dungeon* XMLHandler::getDungeonBeingParsed() {
    return dungeonBeingParsed;
}


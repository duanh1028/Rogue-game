#include <iostream>
#include <string>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include "XMLHandler.hpp"
#include <exception>

//add for step 2
#include <atomic> 
#include <thread>
#include <sstream>
#include "ObjectDisplayGrid.h"
#include "GridChar.h"
#include "KeyboardListener.h"
#include "Dungeon.hpp"
#include <algorithm> 


// set to false when done running
//std::atomic_bool isRunning(true);

int main(int argc, char* argv[]) {

    std::string fileName;
    try {
        xercesc::XMLPlatformUtils::Initialize();
    }
    catch (const xercesc::XMLException& toCatch) {
        char* message = xercesc::XMLString::transcode(toCatch.getMessage());
        std::cout << "Error during initialization! :\n";
        std::cout << "Exception message is: \n"
            << message << "\n";
        xercesc::XMLString::release(&message);
        return 1;
    }
    switch (argc) {
    case 2:
        fileName = "./xmlfiles/" + ((std::string) argv[1]);
        break;
    default:
        std::cout << "C++ Test <xmlfilename>" << std::endl;
    }
    xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();

    Dungeon* dungeonBeingParsed = NULL;
    std::vector<Action*> actions;

    try {
        XMLHandler* handler = new XMLHandler();
        parser->setContentHandler(handler);
        parser->setErrorHandler(handler);
        parser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation, true);
        parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, true);
        XMLCh* fileNameXMLEnc = xercesc::XMLString::transcode(fileName.c_str()); //Encode string as UTF-16, but transcode needs casting as const char * (not std::string)


        parser->parse(fileNameXMLEnc);
        //std::cout << fileName << std::endl;

        xercesc::XMLString::release(&fileNameXMLEnc);

        dungeonBeingParsed = handler->getDungeonBeingParsed();
        actions = handler->getActions();

        delete parser;
        delete handler;
        /*
         * the above is a different form of
         for (int i = 0; i < students.length; i++) {
            //std::cout << students[i] << std::endl;
        }
        */
    }
    catch (const xercesc::XMLException& toCatch) {
        char* message = xercesc::XMLString::transcode(toCatch.getMessage());
        std::cout << "Exception message is: \n"
            << message << "\n";
        xercesc::XMLString::release(&message);
        return -1;
    }
    catch (const xercesc::SAXParseException& toCatch) {
        char* message = xercesc::XMLString::transcode(toCatch.getMessage());
        std::cout << "Exception message is: \n"
            << message << "\n";
        xercesc::XMLString::release(&message);
        return -1;
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
    catch (...) {
        std::cout << "Unexpected Exception \n";
        return -1;
    }

    //add for step 2
    int dungeonWidth = dungeonBeingParsed->getDungeonWidth();
    int dungeonHeight = dungeonBeingParsed->getDungeonHeight();

    //create the dungeon
    ObjectDisplayGrid grid(dungeonWidth, dungeonHeight, 8); //2 lines on the top, 6 lines at the bottom.
    ObjectDisplayGrid* pGrid = &grid;

    for (int i = 0; i < dungeonWidth; i++) {
        for (int j = 0; j < dungeonHeight; j++) {
            char c;
            if ((j == 0) | (j == dungeonHeight - 1)) {
                c = ' ';
            }
            else if ((i == 0) | (i == dungeonWidth - 1)) {
                c = ' ';
            }
            else {
                c = ' ';
            }
            pGrid->addObjectToDisplay(new GridChar(c), i, j);
        }
    }
    
    //add rooms
    for (Room* room : dungeonBeingParsed->getRooms()) {
    //Room* room = dungeonBeingParsed->getRoom();
        int x = room->getPosX();
        int y = room->getPosY();
        int width = room->getWidth();
        int height = room->getHeight();

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
                pGrid->addObjectToDisplay(new GridChar(c), i, j);
            }
        }
    }

    //GridChar*** objectGrid;
    Displayable* player = NULL;
       
    //add items
    for (Item* item : dungeonBeingParsed->getItems()) {
        //Room* room = dungeonBeingParsed->getRoom();
        bool haveowner = (item->getOwner() != NULL);
        if (!haveowner) {
            int x = item->getPosX();
            int y = item->getPosY();
            std::string name = item->getName();
            //std::cout << "haveowner: " + std::to_string(haveowner) + " x: " + std::to_string(x) + " y: " + std::to_string(y) + " name: " + name << std::endl;

            char symbol = '?';

            if (name.find("Armor") != std::string::npos) {
                symbol = ']';
            }
            else if (name.find("Sword") != std::string::npos) {
                symbol = ')';
            }

            GridChar*** gridChar = pGrid->getObjectGrid();
            if (gridChar[x][y + 2]->getChar() != 'X') { //change y to y+2 for 2 lines reserve on top
                pGrid->addObjectToDisplay(new GridChar(symbol), x, y);
            }
        }
    }

    //add passages
    for (Passage* passage : dungeonBeingParsed->getPassages()) {
        std::vector<int> Ys = passage->getYs();
        int i = 0;
        int preX = 0;
        int preY = 0;
        for (int x : passage->getXs()) {
            int y = Ys.at(i);
            if (i == 0) {
                //pGrid->addObjectToDisplay(new GridChar('+'), x, y);
                preX = x;
                preY = y;
            }
            else {
                if (x == preX) {
                    for (int j = std::min(y, preY); j <= std::max(y, preY); j++) {
                        pGrid->addObjectToDisplay(new GridChar('#'), x, j);
                    }
                }
                else if (y == preY) {
                    for (int j = std::min(x, preX); j <= std::max(x, preX); j++) {
                        pGrid->addObjectToDisplay(new GridChar('#'), j, y);
                    }
                }
                //pGrid->addObjectToDisplay(new GridChar('#'), x, y);
                preX = x;
                preY = y;
            }
            
            //add out door
            if (i == Ys.size() - 1) {
                pGrid->addObjectToDisplay(new GridChar('+'), x, y);
            }

            i++;
        }
        //add in door
        pGrid->addObjectToDisplay(new GridChar('+'), passage->getXs().at(0), Ys.at(0));
    }

    //add creatures
    for (Creature* creature : dungeonBeingParsed->getCreatures()) {
        //Room* room = dungeonBeingParsed->getRoom();
        int x = creature->getPosX();
        int y = creature->getPosY();
        char type = creature->getType();

        //if creature is the player, get its position
        if (type == '@') {
            //objectGrid = pGrid->getObjectGrid();
            player = creature;
            int hp = player->getHp();
            pGrid->writeLineTop(0, "HP: " + std::to_string(hp) + "  score: 0");
        }

        //(pGrid->objectGrid)[x][y];

        GridChar*** gridChar = pGrid->getObjectGrid();
        if (gridChar[x][y + 2]->getChar() != 'X') { //change y to y+2 for 2 lines reserve on top
            pGrid->addObjectToDisplay(new GridChar(type), x, y);
        }
    }

    //bottom lines
    pGrid->writeLineBottom(3, "Pack: ");
    pGrid->writeLineBottom(4, "Info: ");

    KeyboardListener listener(pGrid, player, dungeonBeingParsed, actions);
    std::thread keyboardThread(&KeyboardListener::run, &listener);

    pGrid->update();
    keyboardThread.join();

    xercesc::XMLPlatformUtils::Terminate(); //valgrind will say there's memory errors if not included

    return 0;
}

//dungeonBeingParsed.




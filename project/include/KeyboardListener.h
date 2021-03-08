#pragma once
#include "ObjectDisplayGrid.h"
#include "Creature.hpp"
#include "Dungeon.hpp"

class KeyboardListener {
private:
    /** Object display grid reference to write errors */
    ObjectDisplayGrid* grid;
    Displayable* player;
    Dungeon* dungeonBeingParsed;
    std::vector<Action*> actions;

    /** set to false to stop running the keyboard listener */
    bool running = false;

public:
    /**
     * Creates a new keyboard listener, you can pass in extra objects here to interact with them during the game if you wish
     * @param grid  Object display grid reference
     */
    KeyboardListener(ObjectDisplayGrid* grid, Displayable* player, Dungeon* dungeonBeingParsed, std::vector<Action*> actions);

    /**
     * Runs the keyboard listener in a thread
     */
    void run();
};


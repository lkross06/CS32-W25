#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Actor.h"

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), _player(nullptr){}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init()
{
    /*
     (pg. 14)
     1. Initialize the data structures used to keep track of your game’s level and actors.
     2. Load the current level’s details from the appropriate level data file.
     3. Allocate and insert a valid Player object into the game world.
     4. Allocate and insert any Kong objects, Floor objects, Ladder objects, Bonfire objects, Fireball objects, Koopa objects, Goodie objects, or other relevant objects into the game world, as required by the specification in the current level’s data file.
     */
    
    Level level(assetPath());
    
    //load level details
    string l_filename = getLevelFilename();
    
    Level::LoadResult result = level.loadLevel(l_filename);
    if (result == Level::load_fail_file_not_found) {
        cerr << "Could not find " << l_filename << " data file\n";
        return GWSTATUS_LEVEL_ERROR;
    } else if (result == Level::load_fail_bad_format) {
        cerr << "Your level was improperly formatted\n";
        return GWSTATUS_LEVEL_ERROR;
    } else if (result == Level::load_success) {
        for (int row = 0; row < VIEW_HEIGHT; row++)
            for (int col = 0; col < VIEW_WIDTH; col++){
                Level::MazeEntry me = level.getContentsOf(col, row);
                switch (me) {
                    case Level::floor:
                        _actors.push_back(new Floor(this, col, row));
                        break;
                    case Level::ladder:
                        break;
                    case Level::left_kong:
                        break;
                    case Level::right_kong:
                        break;
                    case Level::fireball:
                        break;
                    case Level::koopa:
                        break;
                    case Level::bonfire:
                        break;
                    case Level::extra_life:
                        break;
                    case Level::garlic:
                        break;
                    case Level::player:
                        _player = new Player(this, col, row);
                        break;
                    case Level::empty:
                        break;
                }
            }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q
    
    /*
     (pg. 15)
     1. It must ask all of the actors that are currently active in the game world to do something
     (e.g., ask a Fireball to roll, ask a Bonfire to damage the Player, give the Player a chance
     to move left or right, etc.).
        ○ If an actor does something that causes the Player to die, then the move() method
     should immediately return GWSTATUS_PLAYER_DIED.
        ○ Once Kong flees off the screen, completing the current level, then the move()
     method should immediately:
            ■ Increase the Player’s score appropriately.
            ■ Return a value of GWSTATUS_FINISHED_LEVEL.
     2. It must then delete any actors that have died during this tick (e.g., a Barrel that was
     attacked by a burp so it should be removed from the game world, or a goodie that
     disappeared because the Player picked it up).
     3. It must update the status text on the top of the screen with the latest information (e.g.,
     the user’s current score, the number of burps the Player has left, etc.).
     The move() method must return one of three different values when it returns at the end of each
     tick (all are defined in GameConstants.h):
     
     GWSTATUS_PLAYER_DIED
     GWSTATUS_CONTINUE_GAME
     GWSTATUS_FINISHED_LEVEL
     
     */
    _player->doSomething();
    
    for (Actor* actor : _actors){
        actor->doSomething();
    }
    
    //delete all dead dynamic actors
    vector<Actor*>::iterator it = _actors.begin();
    while (it != _actors.end()){
        if ((*it)->isDead()){
            Actor* temp = *it; //remove from vector AND free up the space!!
            it = _actors.erase(it);
            delete temp;
        }
        else it++;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    /*
     (pg. 19)
     When your cleanUp() method is called by our game framework, it means that the Player lost a life or has completed the current level. In this case, every actor in the entire maze (the Player and every Koopa, Fireball, Floor, Bonfire, Barrel, Ladder, Burp, Kong, goodies, etc.) must be deleted and removed from the StudentWorld’s container of active objects, resulting in an empty world. If the user has more lives left, our provided code will subsequently call your init() method to reload and repopulate the level and the level will then continue from scratch with a brand new set of actors. You must not call the cleanUp() method yourself when the Player dies. Instead, this method will be called by our code.
     */
    for (Actor* actor : _actors) delete actor;
    _actors.clear();
    for (Actor* actor : _actors) delete actor;
    _actors.clear();
    delete _player;
    _player = nullptr;
}

bool StudentWorld::solidAt(int x, int y){
    vector<Actor*>::iterator it = _actors.begin();
    while (it != _actors.end()){
        if ((*it)->isAt(x, y) && (*it)->isSolid()) return true;
        it++;
    }
    return false;
}

string StudentWorld::getLevelFilename() const{
    ostringstream level_filename;
    level_filename << "level";
    level_filename.fill('0');
    level_filename << setw(2) << getLevel() << ".txt";
    return level_filename.str();
}

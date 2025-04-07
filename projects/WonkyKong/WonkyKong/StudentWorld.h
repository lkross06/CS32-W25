#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Player;

class StudentWorld : public GameWorld{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();

    void attackAllActorsAt(int x, int y); //attacks all dynamic actors at position (x,y)
    void burnAllBurnableActorsAt(int x, int y); //burns all burnable actors at position (x,y)
    void freezePlayer();
    void attackPlayer();
    
    bool solidAt(int x, int y);
    bool climbableAt(int x, int y);
    
    int getPlayerX() const;
    int getPlayerY() const;
    
    void spawn(Actor* a); //used by Koopa, Fireball, Barrel, Player, Kong to spawn new actors
    void win() { _hasWon = true; }; //has won a level

    void addBurps(int nB) { _burps += nB; }
    void useBurp() { _burps--; }
    int getBurps() const { return _burps; }
private:
    const int START_BURPS = 0;
    const int MAX_LEVEL_NUM = 99;
    
    std::string getLevelFilename(int level_num) const; //formats as "level__.txt"
    void updateGameText();

    std::vector<Actor*> _actors; //dynamically allocates pointers to all actors in the level!
    Player* _player; //points to the player object!

    int _burps; //stored here so that all actors can access it (i treat it very similarly to lives and score)
    bool _hasWon;
    int _lastLevelNum; //the last valid level number in the files
};

#endif // STUDENTWORLD_H_

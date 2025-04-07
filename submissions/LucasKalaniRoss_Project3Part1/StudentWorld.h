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
    
    bool solidAt(int x, int y);
private:
    std::string getLevelFilename() const;

    std::vector<Actor*> _actors;
    Player* _player;
};

#endif // STUDENTWORLD_H_

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

const int EXTRA_LIFE_GOODIE_POINTS = 50;
const int GARLIC_GOODIE_POINTS = 25;
const int KONG_POINTS_AWARDED = 1000;

//=============================
// BASE CLASS
//=============================

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
    Actor(StudentWorld* world, int iid, int x, int y, int dir = none) : GraphObject(iid, x, y, dir), _isAlive(true), _world(world){};
    virtual ~Actor(){ _world = nullptr; };
    
    //accessors
    bool isAt(int x, int y) const {return getX() == x && getY() == y; }
    StudentWorld* getWorld() const { return _world; };
    bool isDead() const { return !_isAlive; }
    
    //behaviors
    virtual bool isSolid() const { return false; }
    
    //mutators
    virtual void die() { _isAlive = false; } //virtual because might need to call some StudentWorld functions on death
    virtual void getAttacked(){};
    virtual void doSomething() = 0;
private:
    bool _isAlive;
    StudentWorld* _world;
};

//=============================
// STATIC ACTORS
//=============================

class StaticActor : public Actor {
public:
    StaticActor(StudentWorld* world, int iid, int x, int y, int dir = none) : Actor(world, iid, x, y){}
    
    void doSomething(){}
private:
};

class Floor : public StaticActor {
public:
    Floor(StudentWorld* world, int x, int y) : StaticActor(world, IID_FLOOR, x, y){}
    
    bool isSolid() const { return true; }
private:
};

//=============================
// DYNAMIC ACTORS
//=============================

class DynamicActor : public Actor {
public:
    DynamicActor(StudentWorld* world, int iid, int x, int y, int dir = none) : Actor(world, iid, x, y, dir){}
    
    void moveHorizontal(int dir);
private:
};

class Player : public DynamicActor {
public:
    Player(StudentWorld* world, int x, int y) : DynamicActor(world, IID_PLAYER, x, y, right){};
    
    void getAttacked() { die(); }
    void doSomething();
private:
};

#endif // ACTOR_H_

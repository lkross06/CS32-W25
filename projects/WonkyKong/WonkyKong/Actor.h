#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

const int EXTRA_LIFE_GOODIE_POINTS = 50;
const int GARLIC_GOODIE_POINTS = 25;
const int KONG_POINTS_AWARDED = 1000;
const int ENEMY_POINTS_AWARDED = 100;

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
    virtual bool isClimbable() const { return false; }
    virtual bool isBurnable() const { return false; }
    
    //mutators
    virtual void die() { _isAlive = false; }
    virtual void getAttacked(){};
    virtual void getFrozen(){};
    virtual void getBurned(){};
    virtual void doSomething() = 0;
private:
    bool _isAlive;
    StudentWorld* _world;
};

//=============================
// STATIC ACTORS
//=============================

//i define a static actor as an actor which does not interact with any other
//actor. their behaviors may affect other actors, but it remains unchanged.
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

class Ladder : public StaticActor {
public:
    Ladder(StudentWorld* world, int x, int y) : StaticActor(world, IID_LADDER, x, y){}
    
    bool isClimbable() const { return true; }
private:
};

//=============================
// DYNAMIC ACTORS
//=============================

//i define a dynamic actor as any actor that can interact with other actors
//i.e. attacking, burning, freezing, moving, etc.
class DynamicActor : public Actor {
public:
    DynamicActor(StudentWorld* world, int iid, int x, int y, int dir = none, int ptsOnDeath = 0) : Actor(world, iid, x, y, dir), _ptsOnDeath(ptsOnDeath){}
    
    void moveHorizontal(int dir);
    void die();
    void climb(int dir);
    void fall();
    void switchDirection(); //left and right ONLY
    
    virtual bool isOnGround() const;
private:
    int _ptsOnDeath; //points awarded on death
};

class Bonfire : public DynamicActor {
public:
    Bonfire(StudentWorld* world, int x, int y) : DynamicActor(world, IID_BONFIRE, x, y){}
    
    void doSomething();
private:
};

class Player : public DynamicActor {
public:
    Player(StudentWorld* world, int x, int y) : DynamicActor(world, IID_PLAYER, x, y, right), _freezeTimer(0), _jumpTimer(0){};
    
    bool isBurnable() const { return true; }
    
    bool isFrozen() const { return _freezeTimer != 0; }
    bool isJumping() const { return _jumpTimer != 0; }
    
    void getAttacked() { die(); }
    void getFrozen() { _freezeTimer = FREEZE_DURATION; }
    void getBurned() { die(); }
    void doSomething();
    
private:
    const int FREEZE_DURATION = 50; //50 ticks
    const int JUMP_DURATION = 5; //T to T+4 ticks
    
    void jumpStep();
    void burp();
    
    //by default = 0, but if frozen/jumping then != 0 until state is over
    int _freezeTimer;
    int _jumpTimer;
};

class Burp : public DynamicActor {
public:
    Burp(StudentWorld* world, int x, int y, int dir) : DynamicActor(world, IID_BURP, x, y, dir), _burpTimer(BURP_DURATION){};
    
    void doSomething();
private:
    const int BURP_DURATION = 5;
    
    int _burpTimer;
};

class Kong : public DynamicActor {
public:
    Kong(StudentWorld* world, int x, int y, bool facingLeft) : DynamicActor(world, IID_KONG, x, y, left, KONG_POINTS_AWARDED), _flee(false), _fleeTimer(FLEE_STEP_DURATION), _ticksSinceLastBarrel(0){ if (!facingLeft) setDirection(right); }
    
    void doSomething();
private:
    const int MINIMUM_DISTANCE_TO_PLAYER = 2;
    const int FLEE_STEP_DURATION = 5;
    
    double distanceToPlayer() const;
    double nextBarrelDuration() const;
    void throwBarrel();
    void fleeStep();
    
    bool _flee;
    int _ticksSinceLastBarrel;
    int _fleeTimer;
};

//=============================
// ENEMIES
//=============================

//i define an enemy as any dynamic actor that moves by itself and can
//(negatively) affect the player's status
class Enemy : public DynamicActor {
public:
    Enemy(StudentWorld* world, int iid, int x, int y, int dir = none) : DynamicActor(world, iid, x, y, dir, ENEMY_POINTS_AWARDED), _moveTimer(MOVE_DURATION){}
    
    bool isOnGroundAhead(int units); //checks to see if theres ground in x units in current dir
    bool isWallAhead(int units) const; //checks to see if theres a wall in x units in current dir
    
    void die();
    void getAttacked() { die(); }
    virtual bool tryToAffectPlayer();
    
    void decMoveTimer() { _moveTimer--; }
    int canMove() const { return _moveTimer <= 0; }
    void resetMoveTimer() { _moveTimer = MOVE_DURATION; }
    void setRandomDirection();
    
private:
    const int MOVE_DURATION = 10; //10 ticks in between moves
    
    int _moveTimer;
};

class Barrel : public Enemy {
public:
    Barrel(StudentWorld* world, int x, int y, int dir) : Enemy(world, IID_BARREL, x, y, dir), _fellLastTick(false){}
    
    void doSomething();
    
    bool isOnGround() const;
    bool isBurnable() const { return true; }
    void getBurned() { DynamicActor::die(); }
private:
    bool _fellLastTick;
};

class Koopa : public Enemy {
public:
    Koopa(StudentWorld* world, int x, int y);
    
    void doSomething();
    bool tryToAffectPlayer();
    
    void die();
private:
    const int FREEZE_COOLDOWN_DURATION = 50; //50 ticks
    
    int _freezeTimer;
};

class Fireball : public Enemy {
public:
    Fireball(StudentWorld* world, int x, int y);
    
    void doSomething();
    
    void die();
private:
    bool canClimbUp() const;
    bool canClimbDown() const;
    
    bool _isClimbingDown;
    bool _isClimbingUp;
};

//=============================
// GOODIES
//=============================

//i define a goodie as any dynamic actor that dies when interacting with the player
//and has some effect on the player's status (burps, lives, score, etc.)
class Goodie : public DynamicActor {
public:
    Goodie(StudentWorld* world, int iid, int x, int y, int pts) : DynamicActor(world, iid, x, y, none, pts){}
    
    void doSomething();
    virtual void applyGoodie() = 0;
private:
};

class ExtraLifeGoodie : public Goodie {
public:
    ExtraLifeGoodie(StudentWorld* world, int x, int y) : Goodie(world, IID_EXTRA_LIFE_GOODIE, x, y, EXTRA_LIFE_GOODIE_POINTS){}
    
    void applyGoodie();
private:
    
};

class GarlicGoodie : public Goodie {
public:
    GarlicGoodie(StudentWorld* world, int x, int y) : Goodie(world, IID_GARLIC_GOODIE, x, y, GARLIC_GOODIE_POINTS){}
    
    void applyGoodie();
private:
    const int NUM_BURPS = 5;
};

#endif // ACTOR_H_

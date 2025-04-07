#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//=============================
// DYNAMIC ACTORS
//=============================

void DynamicActor::moveHorizontal(int dir){
    if (getDirection() == dir){
        int newX, newY;
        getPositionInThisDirection(dir, 1, newX, newY);
        if (!getWorld()->solidAt(newX, newY)){
            moveTo(newX, newY);
        }
        
    } else setDirection(dir);
}

//=============================
// PLAYER
//=============================

void Player::doSomething(){
    int ch;
        if (getWorld()->getKey(ch)){
            switch (ch){
                case KEY_PRESS_LEFT:
                    moveHorizontal(left);
                    break;
                case KEY_PRESS_RIGHT:
                    moveHorizontal(right);
                    break;
        }
    }
}

#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <random>

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
        
    } else switchDirection(); //must be facing in the given direction to move in it
}

void DynamicActor::climb(int dir){
    int newX, newY;
    getPositionInThisDirection(dir, 1, newX, newY);
    
    if (getWorld()->solidAt(newX, newY)) return;
    
    if (dir == up){ //can only climb up if occupying a spot with a ladder
        if (getWorld()->climbableAt(getX(), getY())) moveTo(newX, newY);
    } else if (dir == down){ //can climb up if occupying a spot with a ladder or theres a ladder below
        if (getWorld()->climbableAt(getX(), getY()) || getWorld()->climbableAt(newX, newY)) moveTo(newX, newY);
    }
}

void DynamicActor::fall(){
    int newX, newY;
    getPositionInThisDirection(down, 1, newX, newY);
    
    if (!isOnGround()) moveTo(newX, newY);
}

void DynamicActor::die(){
    Actor::die();
    //kong + enemies increment points when dying
    //in my framework, kong dying => level ending
    getWorld()->increaseScore(_ptsOnDeath);
}

void DynamicActor::switchDirection(){
    //never want to set actors to direction "up" or "down" (warps sprite)
    if (getDirection() == left) setDirection(right);
    else if (getDirection() == right) setDirection(left);
}

bool DynamicActor::isOnGround() const{
    int newX, newY;
    getPositionInThisDirection(down, 1, newX, newY);
    
    //by default, dynamic actors dont fall through floor or ladders
    return (getWorld()->solidAt(newX, newY) || getWorld()->climbableAt(newX, newY)) || getWorld()->climbableAt(getX(), getY());
}

void Bonfire::doSomething(){
    increaseAnimationNumber(); //doesnt move so manually animate
    //try to burn all dynamic actors at given spot
    getWorld()->burnAllBurnableActorsAt(getX(), getY());
}

void Burp::doSomething(){
    if (isDead()) return;
    else {
        _burpTimer--;
        if (_burpTimer == 0) die(); //dies after a certain amount of time
        else {
            getWorld()->attackAllActorsAt(getX(), getY());
        }
    }
}

void Kong::doSomething(){
    if (isDead()) return;
    
    increaseAnimationNumber(); //animate manually
    
    if (distanceToPlayer() <= 2) _flee = true;
    
    //throw a barrel or increment the timer to throw the next barrel
    if (!_flee && _ticksSinceLastBarrel > nextBarrelDuration()) throwBarrel();
    else _ticksSinceLastBarrel++;
    
    if (_fleeTimer == 0){
        //runs every FLEE_STEP_DURATION ticks
        _fleeTimer = FLEE_STEP_DURATION;
        
        if (_flee){
            fleeStep();
        }
    } else _fleeTimer--;
}

double Kong::distanceToPlayer() const{
    int pX = getWorld()->getPlayerX();
    int pY = getWorld()->getPlayerY();
    
    //euclidean distance formula
    return sqrt(pow(getX() - pX, 2) + pow(getY() - pY, 2));
}

double Kong::nextBarrelDuration() const{
    //N = max(200 - 50 * StudentWorld::getLevel(), 50)
    return std::max(200 - 50 * getWorld()->getLevel(), 50);
}

void Kong::throwBarrel(){
    _ticksSinceLastBarrel = 0; //reset timer
    
    int newX, newY;
    getPositionInThisDirection(getDirection(), 1, newX, newY);
    
    getWorld()->spawn(new Barrel(getWorld(), newX, newY, getDirection()));
}

void Kong::fleeStep(){
    //do one step in the fleeing animation
    int upX, upY;
    getPositionInThisDirection(up, 1, upX, upY);
    moveTo(upX, upY);
    
    //if done fleeing
    if (getY() == VIEW_HEIGHT){
        getWorld()->win();
        die(); //die so that 1000 points are awarded
    }
}

//=============================
// ENEMIES
//=============================

bool Enemy::isOnGroundAhead(int units){
    //so that enemies don't walk off platforms
    int oldX = getX();
    int oldY = getY();
    
    int newX, newY;
    getPositionInThisDirection(getDirection(), units, newX, newY);
    
    moveTo(newX, newY); //move actor to that position
    bool rtn = isOnGround(); //store if actor is on ground
    moveTo(oldX, oldY); //go back to old position
    
    return rtn;
}

bool Enemy::isWallAhead(int units) const{
    //so that enemies know to switch direction
    int newX, newY;
    getPositionInThisDirection(getDirection(), units, newX, newY);
    
    return getWorld()->solidAt(newX, newY);
}

void Enemy::die(){
    getWorld()->playSound(SOUND_ENEMY_DIE);
    DynamicActor::die();
}

void Enemy::setRandomDirection(){
    int dir = rand() % 2; //generate random direction left or right (either 0 or 1)
    if (dir == 0) setDirection(left);
    else setDirection(right);
}

bool Enemy::tryToAffectPlayer(){
    //returns true if attacked player, false otherwise
    if (isAt(getWorld()->getPlayerX(), getWorld()->getPlayerY())){
        getWorld()->attackPlayer();
        return true;
    }
    return false;
}

bool Barrel::isOnGround() const{
    int newX, newY;
    getPositionInThisDirection(down, 1, newX, newY);
    
    //barrels ignore ladders
    return getWorld()->solidAt(newX, newY);
}

void Barrel::doSomething(){
    if (isDead()) return;
    
    decMoveTimer();
    
    if (tryToAffectPlayer()) return;
    
    //every time it lands on a platform it switches direction
    if (_fellLastTick && isOnGround()) switchDirection();
    
    if (!isOnGround()){
        fall();
        _fellLastTick = true;
    } else {
        _fellLastTick = false;
        
        if (canMove()){
            resetMoveTimer();
            
            int newX, newY;
            getPositionInThisDirection(getDirection(), 1, newX, newY);
            
            //if theres a solid in front switch direction. otherwise move
            if (getWorld()->solidAt(newX, newY)) switchDirection();
            else moveTo(newX, newY);
        }
    }
    
}

Koopa::Koopa(StudentWorld* world, int x, int y) : Enemy(world, IID_KOOPA, x, y), _freezeTimer(0) {
    setRandomDirection();
}

void Koopa::doSomething(){
    if (isDead()) return;
    
    _freezeTimer--;
    decMoveTimer();
    
    if (tryToAffectPlayer()) return;
    
    if (canMove()){
        resetMoveTimer();
        
        if (isOnGroundAhead(1) && !isWallAhead(1)) moveHorizontal(getDirection());
        else switchDirection();
        
        tryToAffectPlayer();
    }
}

void Koopa::die(){
    int reward = rand() % 3; //either 0 or 1 or 2 (equal chance)
    if (reward == 0) getWorld()->spawn(new ExtraLifeGoodie(getWorld(), getX(), getY()));
    Enemy::die();
}

bool Koopa::tryToAffectPlayer(){
    if (isAt(getWorld()->getPlayerX(), getWorld()->getPlayerY()) && _freezeTimer <= 0){
        getWorld()->freezePlayer();
        _freezeTimer = FREEZE_COOLDOWN_DURATION; //reset freeze timer so player doesn't get softlocked
        return true;
    }
    return false;
}

Fireball::Fireball(StudentWorld* world, int x, int y) : Enemy(world, IID_FIREBALL, x, y), _isClimbingUp(false), _isClimbingDown(false) {
    setRandomDirection();
}

bool Fireball::canClimbUp() const{
    int upX, upY;
    getPositionInThisDirection(up, 1, upX, upY);
    
    return getWorld()->climbableAt(getX(), getY()) && !getWorld()->solidAt(upX, upY);
}

bool Fireball::canClimbDown() const{
    int downX, downY;
    getPositionInThisDirection(down, 1, downX, downY);
    
    return getWorld()->climbableAt(downX, downY);
}


void Fireball::doSomething(){
    if (isDead()) return;
    
    decMoveTimer();
    
    if (tryToAffectPlayer()) return;
    
    if (canMove()){
        resetMoveTimer();
        
        int upX, upY;
        getPositionInThisDirection(up, 1, upX, upY);
        
        //restrict fireballs to only climbing up or down a sequence of ladders
        //so that it doesnt change direction on ladders and get stuck on them
        if (!_isClimbingDown && canClimbUp()){
            int startClimbing = rand() % 3;
            if (_isClimbingUp || startClimbing == 0){
                _isClimbingUp = true;
                climb(up);
                tryToAffectPlayer();
                return; //return here so fireballs can climb up/down and move left/right in the same tick
            }
        } else if (!_isClimbingUp && canClimbDown()){
            int startClimbing = rand() % 3;
            if (_isClimbingDown || startClimbing == 0){
                _isClimbingDown = true;
                climb(down);
                tryToAffectPlayer();
                return;
            }
        }
        
        //if reaching the top or bottom of the later
        if (_isClimbingDown && !canClimbDown()) _isClimbingDown = false;
        if (_isClimbingUp && !canClimbUp()) _isClimbingUp = false;
        
        //if not on a ladder
        if (!_isClimbingUp && !_isClimbingDown) {
            if (isOnGroundAhead(1) && !isWallAhead(1)) moveHorizontal(getDirection());
            else switchDirection();
        }
        
        tryToAffectPlayer();
    }
}

void Fireball::die(){
    int reward = rand() % 3;
    if (reward == 0) getWorld()->spawn(new GarlicGoodie(getWorld(), getX(), getY()));
    Enemy::die();
}

//=============================
// GOODIES
//=============================

void Goodie::doSomething(){
    int pX = getWorld()->getPlayerX();
    int pY = getWorld()->getPlayerY();
    
    if (isDead()) return;
    else if (isAt(pX, pY)){
        die();
        applyGoodie(); //apply positive effect of goodie (like +5 burps or +1 life)
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}

void ExtraLifeGoodie::applyGoodie(){ getWorld()->incLives(); }

void GarlicGoodie::applyGoodie(){ getWorld()->addBurps(NUM_BURPS); }


//=============================
// PLAYER
//=============================

void Player::doSomething(){
    if (isDead()) return;
    else if (isJumping()){
        jumpStep();
    } else if (isFrozen()){
        _freezeTimer--;
    } else if (!isOnGround()){
        fall();
    } else {
        int ch;
        if (getWorld()->getKey(ch)){
            switch (ch){
                case KEY_PRESS_LEFT:
                    moveHorizontal(left);
                    break;
                case KEY_PRESS_RIGHT:
                    moveHorizontal(right);
                    break;
                case KEY_PRESS_SPACE:
                    //at this point we know the player is not jumping, frozen, and is on floor
                    _jumpTimer = JUMP_DURATION;
                    jumpStep();
                    break;
                case KEY_PRESS_UP:
                    climb(up);
                    break;
                case KEY_PRESS_DOWN:
                    climb(down);
                    break;
                case KEY_PRESS_TAB:
                    if (getWorld()->getBurps() > 0) burp();
                    
            }
        }
    }
    
    
}

void Player::burp(){
    getWorld()->playSound(SOUND_BURP);
    
    int newX, newY;
    getPositionInThisDirection(getDirection(), 1, newX, newY);
    
    getWorld()->spawn(new Burp(getWorld(), newX, newY, getDirection()));
    getWorld()->useBurp();
}

void Player::jumpStep(){
    int dir = up;
    if (_jumpTimer == 1) dir = down;
    else if (_jumpTimer != JUMP_DURATION) dir = getDirection();
    
    int newX, newY;
    getPositionInThisDirection(dir, 1, newX, newY);
    if (getWorld()->solidAt(newX, newY)) _jumpTimer = 0;
    else {
        moveTo(newX, newY);
        _jumpTimer--;
        if (getWorld()->climbableAt(newX, newY)) _jumpTimer = 0;
        
        getWorld()->playSound(SOUND_JUMP);
    }
}

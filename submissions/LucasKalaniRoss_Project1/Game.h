#ifndef GAME_H

#define GAME_H

class Room;

class Game{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nBees);
    ~Game();

        // Mutators
    void play();

  private:
    Room* m_room;
};

#endif //for GAME_H

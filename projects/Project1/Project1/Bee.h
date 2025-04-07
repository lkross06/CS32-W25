#ifndef BEE_H

#define BEE_H

class Room;

class Bee{
    public:
          // Constructor
      Bee(Room* rp, int r, int c);

          // Accessors
      int  row() const;
      int  col() const;

          // Mutators
      void move();
      bool getSwatted(int dir);

    private:
      Room* m_room;
      int   m_row;
      int   m_col;
      int   m_health;
};

#endif //for BEE_H

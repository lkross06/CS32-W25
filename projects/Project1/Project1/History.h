#ifndef HISTORY_H

#define HISTORY_H

#include "globals.h"

class History{
  public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
  private:
    int m_nRows;
    int m_nCols;
    int m_recordGrid[MAXROWS][MAXCOLS];
    
    //TODO:
    // store records as a string for infinite space??
    // implement "h" feature in game
};


#endif //for HISTORY_H

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include "History.h"
#include "globals.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  History implementations
///////////////////////////////////////////////////////////////////////////

History::History(int nRows, int nCols){
    m_nRows = nRows;
    m_nCols = nCols;
    
    for (int r = 0; r < nRows; r++){
        for (int c = 0; c < nCols; c++){
            m_recordGrid[r][c] = 0;
        }
    }
}

bool History::record(int r, int c){
    if (r < 1 || r > m_nRows) return false;
    if (c < 1 || c > m_nCols) return false;
    
    m_recordGrid[r - 1][c - 1] += 1;
    return true;
}

void History::display() const{
    clearScreen();
    for (int r = 0; r < m_nRows; r++){
        for (int c = 0; c < m_nCols; c++){
            char x = ('A' - 1) + m_recordGrid[r][c];
            if (x == 'A' - 1) x = '.';
            if (x > 'Z') x = 'Z';
            
            cout << x;
        }
        cout << endl;
    }
    cout << endl;
}


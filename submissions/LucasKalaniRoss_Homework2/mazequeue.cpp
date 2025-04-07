#include <iostream>
#include <queue>
#include <string>
using namespace std;

const char UNCHECKED = '.';
const char CHECKED = '*';

class Coord{
  public:
    Coord(int r, int c) : m_row(r), m_col(c) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
  private:
    int m_row;
    int m_col;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    queue<Coord> q; //perform a breadth-first search using a stack
    
    q.push(Coord(sr, sc));
    maze[sr][sc] = CHECKED;
    
    //if queue runs out then all paths from (sr,sc) have been checked and lead to dead ends
    while (!q.empty()){
        
        Coord c = q.front();
        q.pop();
        
        cout << "(" << c.r() << ", " << c.c() << ")" << endl;
        
        if (c.r() == er && c.c() == ec) return true;
        
        //north
        if (c.r() - 1 >= 0 && maze[c.r() - 1][c.c()] == UNCHECKED){
            q.push(Coord(c.r() - 1, c.c()));
            maze[c.r() - 1][c.c()] = CHECKED;
        }
        //south
        if (c.r() + 1 < nRows && maze[c.r() + 1][c.c()] == UNCHECKED){
            q.push(Coord(c.r() + 1, c.c()));
            maze[c.r() + 1][c.c()] = CHECKED;
        }
        //east
        if (c.c() + 1 < nCols && maze[c.r()][c.c() + 1] == UNCHECKED){
            q.push(Coord(c.r(), c.c() + 1));
            maze[c.r()][c.c() + 1] = CHECKED;
        }
        //west
        if (c.c() - 1 >= 0 && maze[c.r()][c.c() - 1] == UNCHECKED){
            q.push(Coord(c.r(), c.c() - 1));
            maze[c.r()][c.c() - 1] = CHECKED;
        }
    }
    
    return false;
}

int main(){
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}

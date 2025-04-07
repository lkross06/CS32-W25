bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    //base cases for checking invalid tiles
    if (sr < 0 || sr >= nRows) return false;
    if (sc < 0 || sc >= nCols) return false;
    if (maze[sr][sc] != '.') return false;
    
    //base case for finding finish
    if (sr == er && sc == ec) return true;
    maze[sr][sc] = '*';
    
    //north
    if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec)) return true;
    
    //south
    if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec)) return true;
    
    //east
    if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec)) return true;
    
    //west
    if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec)) return true;
    
    return false;
}

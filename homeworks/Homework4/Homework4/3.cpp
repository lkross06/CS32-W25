//#include "Sequence.h"  // class template from problem 2
//
//class Coord
//{
//  public:
//    Coord(int r, int c) : m_row(r), m_col(c) {}
//    Coord() : m_row(0), m_col(0) {}
//    double r() const { return m_row; }
//    double c() const { return m_col; }
//    Coord operator>(const Coord& rhs) const { return *this; }
//  private:
//    double m_row;
//    double m_col;
//};
//
//int main()
//{
//    Sequence<int> si;
//    si.insert(50);               // OK
//    Sequence<Coord> sc;
//    sc.insert(0, Coord(50,20));  // OK
//    sc.insert(Coord(40,10));     // error!
//}

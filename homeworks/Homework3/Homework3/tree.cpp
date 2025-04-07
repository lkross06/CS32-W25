// Return the number of ways that all n1 elements of a1 appear in
// the n2 element array a2 in the same order (though not necessarily
// consecutively).  We decree that the empty sequence (i.e. one where
// n1 is 0) appears in a sequence of length n2 in 1 way, even if n2
// is 0.  For example, if a2 is the 7 element array
//    10 50 40 20 50 40 30
// then for this value of a1     the function must return
//    10 20 40                        1
//    10 40 30                        2
//    20 10 40                        0
//    50 40 30                        3
int countIsIn(const double a1[], int n1, const double a2[], int n2){
    //handle base cases
    if (n1 - 1 < 0) return 1; //a branch that reached the end of a1
    if (n2 - 1 < 0) return 0; //a unfinished branch that reached the end of a2
    
    int count = 0; //count how many branches reach the end of a1
    //iterate to the end of the array for the specified a1 value
    //and opening branches for subsequent a1 values to be analyzed
    while (n1 - 1 >= 0 && n2 - 1 >= 0){
        if (a1[n1 - 1] == a2[n2 - 1]) count += countIsIn(a1, n1 - 1, a2, n2 - 1);
        n2 -= 1; //keep going down n2
    }
    
    //return the number of branches that reached the end of a1
    return count;
}

// Exchange two doubles
void exchange(double& x, double& y)
{
  double t = x;
  x = y;
  y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > separator come before all the other elements,
// and all the elements whose value is < separator come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= separator, or n if there is no such element, and firstLess is
// set to the index of the first element that is < separator, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > separator
//   * for firstNotGreater <= i < firstLess, a[i] == separator
//   * for firstLess <= i < n, a[i] < separator
// All the elements > separator end up in no particular order.
// All the elements < separator end up in no particular order.
void separate(double a[], int n, double separator,
                                     int& firstNotGreater, int& firstLess)
{
  if (n < 0)
      n = 0;

    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > separator
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == separator
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < separator

  firstNotGreater = 0;
  firstLess = n;
  int firstUnknown = 0;
  while (firstUnknown < firstLess)
  {
      if (a[firstUnknown] < separator)
      {
          firstLess--;
          exchange(a[firstUnknown], a[firstLess]);
      }
      else
      {
          if (a[firstUnknown] > separator)
          {
              exchange(a[firstNotGreater], a[firstUnknown]);
              firstNotGreater++;
          }
          firstUnknown++;
      }
  }
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
    if (n <= 1) return; //base case
    
    int mid = n / 2; //integer division will truncate decimate (rounds down)
    int fNG = 0; //first not greater
    int fL = 0; //first less

    separate(a, n, a[mid], fNG, fL);
    
    //order the front half of the array
    if (fNG != n) order(a, fNG);
    //order the back half of the array (offset the pointer and length)
    if (fL != n) order(a + fL, n - fL);
}

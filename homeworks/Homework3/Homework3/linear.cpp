
  // Return true if the somePredicate function returns true for at
  // least one of the array elements; return false otherwise.
bool anyTrue(const double a[], int n)
{
    if (n - 1 < 0) return false; //base case
    if (somePredicate(a[n - 1])) return true;
    
    return anyTrue(a, n - 1); //go from index n-1 to 0
}

  // Return the number of elements in the array for which the
  // somePredicate function returns true.
int countTrue(const double a[], int n)
{
    if (n - 1 < 0) return 0; //base case
    if (somePredicate(a[n - 1])) return countTrue(a, n - 1) + 1; //add 1 to total
    return countTrue(a, n - 1); // don't add 1 to total
}

  // Return the subscript of the first element in the array for which
  // the somePredicate function returns true.  If there is no such
  // element, return -1.
int firstTrue(const double a[], int n)
{
    if (n - 1 < 0) return -1; //base case
    
    //see if there are any previous elements satisfying condition
    int f = firstTrue(a, n - 1);
    
    if (somePredicate(a[n - 1])){
        if (f != -1 && f < n - 1) return f; //if there's a smaller subscript satisfying condition
        return n - 1; //this is the smallest subscript satisfying condition
    }
    
    return f; //keep going down
}

  // Return the subscript of the smallest element in the array (i.e.,
  // return the smallest subscript m such that a[m] <= a[k] for all
  // k from 0 to n-1).  If the function is told that no doubles are to
  // be considered in the array, return -1.
int positionOfMinimum(const double a[], int n)
{
    if (n - 1 == 0) return a[n - 1]; //base case 1 (single element array)
    if (n - 1 < 0) return -1; //base case 2 (no element array)
    
    //find the smallest in the rest of the array
    int smallest = positionOfMinimum(a, n - 1);
    
    //if this is smaller than rest of the array return this instead
    if (a[n - 1] <= a[smallest]) return n - 1;
    
    return smallest;
}

  // If all n1 elements of a1 appear in the n2 element array a2, in
  // the same order (though not necessarily consecutively), then
  // return true; otherwise (i.e., if the array a1 is not a
  // not-necessarily-contiguous subsequence of a2), return false.
  // (Of course, if a1 is empty (i.e., n1 is 0), return true.)
  // For example, if a2 is the 7 element array
  //    10 50 40 20 50 40 30
  // then the function should return true if a1 is
  //    50 20 30
  // or
  //    50 40 40
  // and it should return false if a1 is
  //    50 30 20
  // or
  //    10 20 20
bool isIn(const double a1[], int n1, const double a2[], int n2)
{
    if (n1 - 1 < 0) return true;
    if (n2 - 1 < 0) return false;
    
    //increment down the a1 array and a2 array (don't count duplicates in a2)
    if (a1[n1 - 1] == a2[n2 - 1]) return isIn(a1, n1 - 1, a2, n2 - 1);
    
    //no match, keep going down a2
    return isIn(a1, n1, a2, n2 - 1);
}

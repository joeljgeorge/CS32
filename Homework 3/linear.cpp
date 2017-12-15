/*#include <iostream>
using namespace std;*/

/*bool somePredicate(double x) {
	if (x < 0)
		return false;
	return true;
}*/

// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n)
{
	if (n <= 0)
		return true;
	if (!somePredicate(a[0]))
		return false;
	else
		return allTrue(a + 1, n - 1);
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
	if (n <= 0)
		return 0;
	int count = 0;
	if (!somePredicate(a[0])) {
		count++;
		count += countFalse(a + 1, n - 1);
		return count;
	}
	else
		return countFalse(a + 1, n - 1);
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
	if (n <= 0)
		return -1;
	int currentIndex = n - 1;
	if (!somePredicate(a[n-1]) && firstFalse(a, n-2) == 0) {
		return currentIndex;
	}
	else
		return 0;
	
	  // This is incorrect.
}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
	if (n <= 0)
		return -1;
	if (n == 1)
		return 0;

	double beta = a[n-1];
	int substring = indexOfMin(a, n - 1);
	double alpha = a[substring];

	if (alpha <= beta) {
		return substring;
	}
	else
		return n-1;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0)
		return true;
	if (n2 > n1)
		return false;
	if (n1 <= 0)
		return false;
	bool subsequence = false;
	if (a1[0] == a2[0])
		subsequence = includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	else
		subsequence = includes(a1+1, n1 - 1, a2, n2);
	return subsequence;
}
//TESTING!!!
/*int main() {
	double arr[] = { 0, 4, 5, 6, 3, 7 };
	double arrag[] = {4, 5, 7}; 
	if (allTrue(arr, 6))
		cout << "no negative values" << endl;
	else
		cout << "there are negative values in the array" << endl;
	if (countFalse(arr, 6) == 0)
		cout << "yay" << endl;
	else
		cout << "ugh" << endl;
	cout << firstFalse(arr, 6) << endl;
	cout << indexOfMin(arr, 6) << endl;
	if (includes(arr, 6, arrag, 3))
		cout << "Is a subsequence" << endl;
	else
		cout << "Not a subsequence" << endl;
}*/
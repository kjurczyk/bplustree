#include "bplustree.h"


////////////////////////////////////////////////
// Your program should execute using the following
// For C/C++:
// $ ./ bplustree file_name
// For Java:
// $ java bplustree file_name
// Where file_name is the name of the file that has the input test data.
////////////////////////////////////////////////

int main()
{
  // create a string to hold the filename
  string file_name = "";

	//take the user input to determine the file name
	std::cout << "Please enter the name of the input file: \n";
	std::cin >> file_name;
  std::cout << "file_name is " + file_name;

  // read in what's in the file called file_name
  // create a 1D array into which all of the different instructions can be inserted
  // make sure to delete the array afterwards
  vector<string> textInFile;

  // at the end of the program
  return 0;
}
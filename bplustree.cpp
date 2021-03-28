#include "bplustree.h"


////////////////////////////////////////////////
// Your program should execute using the following
// For C/C++:
// $ ./ bplustree file_name
// For Java:
// $ java bplustree file_name
// Where file_name is the name of the file that has the input test data.
// argc = # of arguments
// argv[i] allows you to access the arguments from the command line
////////////////////////////////////////////////

int main(int argc, char ** argv)
{
  // create a string to hold the filename
  string file_name = "";

	//take the user input to determine the file name
	std::cout << "Please enter the name of the input file: \n";
	std::cin >> file_name;
  std::cout << "file_name is " + file_name;

  //check that the file is openable
	if(fileCanBeOpened(file_name))
	{
		return 1;	//the program will die if you don't enter a valid file name
	}

  // create a 1D array into which all of the different instructions can be inserted
  // make sure to delete the array afterwards
  vector<string> textInFile;

  // read in what's in the file called file_name into the vector
  textInFile = readFile(file_name);

  // at the end of the program
  return 0;
}

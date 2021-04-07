//#include "bplustree.h"
#include "methods.h"
#include <bits/stdc++.h>


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

  // create a new bplustree object
  BPTree * bpt = new BPTree();

  // create an array to hold the values for the command, first, and second numbers
  // the first slot will be the command
  // the second will be the first number
  // the third will be the second number 
  string command[3];

  // variables to hold the name of the input file, output file, and one line of input at a time
  std::ifstream file;
  std::ofstream outputFile;
  string lineOfInput = "";
	
  // check for the file name as an argument
  if(argc > 1)
  {
    file_name = argv[1];
  }
  else  // if no argument exists, wait for the user to enter an argument and use that as the file name
  {
    //take the user input to determine the file name
    //std::cout << "Please enter the name of the input file: \n";
    std::cin >> file_name;
  }

  if(DEBUG) std::cout << "file_name is " + file_name << endl;

  // open the file
  file.open(file_name.c_str());
  outputFile.open("output_file.txt");

  // check if the file actually opened correctly
  if (file.is_open() && outputFile.is_open())
	{
    // getline reads one line of the file and puts it in the lineOfInput string
		while(getline(file, lineOfInput))
    {
      if(DEBUG) std::cout << lineOfInput << endl; // print line out for debugging
      command[0] = getCommand(lineOfInput); // get the command
      command[1] = getFirstNum(lineOfInput);  // get the first number in the command
      command[2] = getSecondNum(lineOfInput); // get the second number, if it exists (like for insert or search(key1, key2))
      
      // now that we've successfully gotten the numbers and the command, it
      // is time to enter everything into the tree
      
      // the tree is accessed and output is entered into the file if output exists (only from the search functions)
      outputFile << doStuffWithCommand(command, *bpt);  
    }

    // we've reached the end of the loop getting input, so close the files
      outputFile.close();
      file.close();
      
	}
  else  // if the file was not able to be opened
  {
    cerr << "File not opened" << endl;
  }
  // at the end of the program
  return 0;
}
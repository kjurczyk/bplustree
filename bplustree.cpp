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

  // create an array to hold the values for the command, first, and second numbers
  // the first slot will be the command
  // the second will be the first number
  // the third will be the second number 
  string command[3];
	
  if(argc > 1)
  {
    file_name = argv[1];
  }
  else
  {
    //take the user input to determine the file name
    //std::cout << "Please enter the name of the input file: \n";
    std::cin >> file_name;
  }

  std::cout << "file_name is " + file_name << endl;

  
  std::ifstream file;
  string lineOfInput = "";

  /*
  //check that the file is openable
	if(!fileCanBeOpened(file_name)) // if the file cannot be opened/doesn't exist
	{
		return 1;	//the program will die if you don't enter a valid file name
	}

  // create a 1D array into which all of the different instructions can be inserted
  // make sure to delete the array afterwards
  vector<string> textInFile;

  // read in what's in the file called file_name into the vector
  textInFile = readFile(file_name);
  
  // go through the file, popping one line at a time
  // figure out what the command is
  // also determine what the numbers pertaining to the command are
  while(textInFile.size() > 0)  // size will be decreasing every time I pop
  {
    std::cout << textInFile.front() << endl;
    textInFile.erase(0);
  }
  */


  // open the file
  file.open(file_name.c_str());

  // check if the file actually opened correctly
  if (file.is_open())
	{
    // getline reads one line of the file and puts it in the lineOfInput string
		while(getline(file, lineOfInput))
    {
      //toReturn.push_back(lineOfInput);  // append line to the vector
      std::cout << lineOfInput << endl; // print line out for debugging
      //command[0] = getCommand(lineOfInput);
      
    }
      // close the file
      file.close();
	}
  else  // if the file was not able to be opened
  {
    cerr << "File not opened" << endl;
  }
  // at the end of the program
  return 0;
}

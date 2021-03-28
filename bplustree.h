#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using std::string;
using namespace std;


// class which holds the key and value, allows editing and deleting
class Pair
{
  // You can use integer as the type of the key and float/double as the type of the value.
  int key;
  float value;
};

//////////////////////////////////////////////////////////////
// checks that the file is valid - if not, it will return 1 and print 
// out that the file was not opened. The 1 will make the program die.
//////////////////////////////////////////////////////////////
bool fileCanBeOpened(string name)	
{
	std::ifstream file;
	file.open(name.c_str());

  // if the file does not open
	if (!file.is_open())
	{
		cerr << "File not opened" << endl;
		return false;
	}

  // otherwise, it correctly opened
	return true;
}

///////////////////////////////////////////////////////////////
// reads in what is in the file into a vector<string>
// returns the vector<string>
// each new line in the file gets its own unit in the vector
///////////////////////////////////////////////////////////////
vector<string> readFile(string name)
{
  std::ifstream file;
  string lineOfInput = "";
  vector<string> toReturn;

  // open the file
  file.open(name.c_str());

  // check if the file actually opened correctly
  if (file.is_open())
	{
    // getline reads one line of the file and puts it in the lineOfInput string
		while(getline(file, lineOfInput))
    {
      toReturn.push_back(lineOfInput);  // append line to the vector
      std::cout << lineOfInput << endl; // print line out for debugging
    }
      // close the file
      file.close();
	}
  else  // if the file was not able to be opened
  {
    cerr << "File not opened" << endl;
  }
  return toReturn;

}

//////////////////////////////////////////////////////////
// gets input from the user and places it into the array in 
// the order that the user input it
//////////////////////////////////////////////////////////
void takeInput(int * arr, int givenL)
{
	int input = 0;
	for(int i = 0; i < givenL; i++)	//it knows how many elements are supposed to be in the array and makes sure that many are entered
	{
		std::cin >> input;
		arr[i] = input;
	}
}
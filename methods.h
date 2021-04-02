#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using std::string;
using namespace std;
#include <locale>         // std::locale, std::tolower
#include "bplustree.h"

///////////////////////////////////////////////////////////////
// return what the command is
////////////////////////////////////////////////////////////
string getCommand(string s)
{
  string toReturn = "";
  // parse until you get to a space or a ()
  for(int i = 0; i < s.length(); i++)
  { 
    // check if the character is a space or a ()
    if(s[i] != ' ' && s[i] != '(')
    {
      // add the character to the string that's making up the command we will be returning
      toReturn = toReturn + s[i];  
    }
    else
    {
      //s = s.substr(i); // get everything starting from that problem character onwards
      break;
    }
  }
  return toReturn;
}

///////////////////////////////////////////////////////////////
// return what the first number after the parentheses is
////////////////////////////////////////////////////////////
string getFirstNum(string s)
{

  int parenLoc = s.find('('); // get the location of the open parentheses
  int commaLoc = s.find(','); // get the location of the second parentheses
  // check if there is a comma that was found. If no comma, it is the closing parentheses we are looking for
  if(commaLoc == std::string::npos) // is there a comma? enter if statement if comma does not exist
  {
    //std::cout << "no comma here!" << endl;
    // if there is no comma, look for the closing parentheses instead
    commaLoc = s.find(')');
  }
  // now we have a starting and an ending value
  // return the substring between the two (from, length of substring)
  //  location of parentheses + 1 so you don't get the parentheses
  // comma location - parentheses location to get the length of the 
  // string, subtract 1 because you don't want to get the comma or parentheses after the number
  return s.substr(parenLoc + 1, commaLoc - parenLoc - 1);
}

///////////////////////////////////////////////////////////////
// return second number, if one exists
// if no second number exists, return null
// remember there might be a space after the comma
////////////////////////////////////////////////////////////
string getSecondNum(string s)
{

  int parenLoc = s.find(')'); // get the location of the open parentheses
  int commaLoc = s.find(','); // get the location of the second parentheses
  // check if there is a comma that was found. If no comma, it is the closing parentheses we are looking for
  if(commaLoc == std::string::npos) // is there a comma? enter if statement if comma does not exist
  {
    // there is no comma, so return a string indicating that no number exists
    return "na";
  }
  // we've found the location of the comma, if it exists
  // we don't want the comma though, so add 1
  commaLoc++;
  // igrnore the space after the comma, if it exists
  if(s[commaLoc] == ' ')
  {
    commaLoc++;
  }
  // now we have a starting and an ending value
  // return the substring between the two (from, length of substring)
  //  location of comma + 1 so you don't get the parentheses
  // parentheses location - comma location to get the length of the 
  // string, subtract 1 because you don't want to get the comma or parentheses after the number
  return s.substr(commaLoc, parenLoc - commaLoc);
}

///////////////////////////////////////////////////////////////
// determines what the command is and calls it with the appropriate
// numbers from the array it was given from the main function
// returns the new tree
////////////////////////////////////////////////////////////
void doStuffWithCommand(string instructions[3], BPTree &bpt)
{
  //std::cout << instructions[0] + " has been called" << endl;
  if(instructions[0].find("Initialize") != std::string::npos) // we assume that initialize is always spelled like this
  {
    //std::cout << "initialize has been called" << endl;
    bpt.initialize(std::stoi(instructions[1]));
  }
  else if(instructions[0].find("Insert") != std::string::npos) // looks like Insert(key, value)
  {
    //std::cout << "insert has been called" << endl;
    // call insert(key, value)
    bpt.insert(std::stoi(instructions[1]), std::stof(instructions[2]));
  }
  else if(instructions[0].find("Delete") != std::string::npos) // delete a key
  {
    bpt.deletePair(std::stoi(instructions[1]));
  }
  else//(instructions[0].find("Search") != std::string::npos)  // the only other option is that this is a search. It returns the value associated with the key
  {
    // send it to the appropriate search function
    if(instructions[2] == "na") // if there is only one value, just send the first
    {
      bpt.search(std::stoi(instructions[1]));
    }
    else
    {
      bpt.search(std::stoi(instructions[1]), std::stof(instructions[2]));
    }
  }
   
}

// returns the value associated with the key
void BPTree::search(int key, int key2)
{
  
  std::cout << "key1: " << key << "key2: " << key2 << endl;
}

// returns the value associated with the key
void BPTree::search(int key)
{
  
  std::cout << "key: " << key << endl;
}

void BPTree::deletePair(int key)
{
  
  std::cout << "key: " << key << endl;
}

// void BPTree::insert(int key, float value)
// {
  
//   std::cout << "key: " << key << ", value: " << value << endl;
// }

// void BPTree::initialize(int m)
// {
//   maxNodes = m;
//   std::cout << "maxNodes set to " << maxNodes << endl;
// }

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
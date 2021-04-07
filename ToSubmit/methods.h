#include "search.h" // methods.h pulls from all the other files in a line

///////////////////////////////////////////////////////////////
// return what the command given in the string is
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
string doStuffWithCommand(string instructions[3], BPTree &bpt)
{
  string toReturn = "";
  //std::cout << instructions[0] + " has been called" << endl;
  if(instructions[0].find("Initialize") != std::string::npos) // we assume that initialize is always spelled like this
  {
    //std::cout << "initialize has been called" << endl;
    bpt.initialize(std::stoi(instructions[1]));
  }
  else if(instructions[0].find("Insert") != std::string::npos) // looks like Insert(key, value)
  {
    // call insert(key, value)
    bpt.insert(std::stoi(instructions[1]), std::stof(instructions[2]));
  }
  else if(instructions[0].find("Delete") != std::string::npos) // delete a key
  {
    bpt.deletePair(std::stoi(instructions[1]));
  }
  else // the only other option is that this is a search. It returns the value associated with the key
  {
    // send it to the appropriate search function
    if(instructions[2] == "na") // if there is only one value, just send the first
    {
      return bpt.search(std::stoi(instructions[1]));
    }
    else
    {
      return bpt.search(std::stoi(instructions[1]), std::stof(instructions[2]));
    }
  }
   
   if(DEBUG) bpt.printTree(); // after every command, print the tree
   if(DEBUG) bpt.printLinkedList(0); // after every command, print the linked list keys starting at the smallest node
   if(DEBUG) bpt.printLinkedListPairs(0); // after every command, print the linked list key(value) pairs starting at the smallest node
   return toReturn; // return the string generated from that command (only the search generates a string)
}
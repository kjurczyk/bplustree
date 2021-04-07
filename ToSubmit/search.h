#include "delete.h"

/////////////////////////////////////////////////////////////////
// returns the value at the key location
/////////////////////////////////////////////////////////////////
std::string BPTree::search(int key) // returns the value at the key location
{
  std::ostringstream ss;
  nodePointer current = determineLeafNode(key);

  // we've found the leaf node at which the key should be located
  for(int i = 0; i < current->numKeys; i++)
  {
    // if there is a key, return the value and print it out
    if(current->keyValues[i].key == key)
    {
      if(DEBUG) cout << current->keyValues[i].value << endl;
      ss << current->keyValues[i].value;
      i = current->numKeys;
    }
  }

  // if we reach this location, there is no key, print out "Null"
  if(DEBUG) cout << "Null" << endl;
  if(ss.str() == "")
  {
    ss << "Null";
  }

  ss << "\n";
  return ss.str();
}

/////////////////////////////////////////////////////////////////
  // bool startingOff = true
  // bool stayIn = true;
  // determine the starting leaf node using key1
  // for(int i = 0; i < leaf->numKeys && stayIn; i++)
  //  if(!startingOff) print a comma
  //  while(i < leaf->numKeys)
  //    if leaf->key[i] > key2, dropOut by making i = leaf->numKeys, stayIn = false;
  //    else, print out the values of keys that are >= key1
  //    i++
  //   if you've dropped out of the while loop, you are at the end of a node
  //   make the starting leaf = startingLeaf->nextLeaf;
// returns/prints the range of values from key1 to key2
/////////////////////////////////////////////////////////////////
std::string BPTree::search(int key1, int key2) // searches for the range of values which correspond to keys that are >key1 and <key2
{
  std::ostringstream ss;
  bool startingOff = true;  // we are just starting off (helps with comma placement)
  bool stayIn = true; // we should continue looking for more numbers

  // determine the starting leaf node using key1
  nodePointer current = determineLeafNode(key1);
  
  if(DEBUG)printNode("starting node", current);
  for(int i = 0; i < current->numKeys && stayIn; i++) // go through all the keys in the current node
  {
    // while(i < leaf->numKeys)
    while(i < current->numKeys)
    {
      if(DEBUG)cout << "in the while loop" << endl;
      // if leaf->key[i] > key2, dropOut by making i = leaf->numKeys, stayIn = false;
      if(current->keys[i] > key2)
      {
        if(DEBUG) cout << "dropping out because key at " << i << " is " << current->keyValues[i].key << endl;
        i = current->numKeys;
        stayIn = false;
      }
      else if(current->keys[i] >= key1)  // else, print out the values of keys that are >= key1
      {
        if(DEBUG)cout << "key at " << i << ": " << current->keyValues[i].key << ", value: " << current->keyValues[i].value << ",  ";
        if(startingOff) // if it's the first number you're printing, don't print a comma in front of it
        {
          if(DEBUG) cout << current->keyValues[i].value;
          //toReturn.append(to_string(current->keyValues[i].value));
          ss << current->keyValues[i].value;
          startingOff = false;
        }
        else  // any numbers coming next should get a comma in front of them
        {
          ss << ", ";
          ss << current->keyValues[i].value;
          if(DEBUG) cout << ", " << current->keyValues[i].value;  // print a comma before any numbers coming next
        }
        
      }
      i++; // i++
    }
   
      //   if you've dropped out of the while loop, you are at the end of a node 
      i = -1;
      //   if the next leaf is not a nullptr, make the starting leaf = startingLeaf->nextLeaf;
      if(current->nextLeaf == nullptr)
      {
        if(DEBUG)cout << "next leaf is a nullptr" << endl;
        stayIn = false;
      }
      else
      {
        current = current->nextLeaf;
        if(DEBUG)printNode("node we are looking at now", current);
      }
  }
  
  // print a new line at the end of the search to start the next thing off on a new line
  if(DEBUG) cout << endl;
  ss << "\n";
  //toReturn.append("\n");
  return ss.str(); //toReturn;
}
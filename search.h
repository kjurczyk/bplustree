#include "delete.h"

// returns the value at the key location
void BPTree::search(int key)
{
  nodePointer current = determineLeafNode(key);
  // we've found the leaf node at which the key should be located
  for(int i = 0; i < current->numKeys; i++)
  {
    // if there is a key, return the value and print it out
    if(current->keyValues[i].key == key)
    {
      cout << current->keyValues[i].value << endl;
      return;
    }
  }
  // if we reach this location, there is no key, print out "Null"
  cout << "Null" << endl;
}

// returns/prints the range of values from key1 to key2
void BPTree::search(int key1, int key2)
{
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
}
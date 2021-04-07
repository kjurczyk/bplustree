


#include <iostream> // enables taking input from the user
#include <fstream>  // enables taking input from files
#include <ostream>  // enables outputting from files
#include <string>   // enables using strings
#include <vector>   // enables using vectors for my strings
using std::string;  // enables using string instead of std::string
using namespace std;// important for something
#include <locale>   // not sure the importance of this, but the program works with it
#include <math.h>   // enables ceiling function
#include <algorithm>// enables some math
#include <array>    // at some point I used array, this may be extraneous now
#include <sstream>  // enables me to change numbers to strings easily

// global variable used for debugging purposes. Set this to true to print out what the tree is 
// looking like at different points in time and which point you are at in the code
bool DEBUG = false;

/////////////////////////////////////////////////////////////////////////
// class which holds the key and value, allows editing and printing
/////////////////////////////////////////////////////////////////////////
class Pair
{
  public:

  // You can use integer as the type of the key and float/double as the type of the value.
  int key;
  float value;

  Pair(int k, float v)  // initialize with key and value
  {
    key = k;
    value = v;
  }

  Pair()  // initialize key and value to 0
  {
    key = 0;
    value = 0;
  }

  void printPair()  // print the pair in the form key(value)
  {
    cout << key << "(" << value << ")";
  }
};

////////////////////////////////////////////////////////////////////
// class that holds a pointer to the root node of a b+ tree
// also holds all the operations that can be performed on the tree
// Same structure as B-trees
// Dictionary pairs are in leaves only. Leaves form a
// doubly-linked list.
// Remaining nodes have following structure:
//      j a0 k1 a1 k2 a2 …kj aj
//        • j = number of keys in node.
//        • ai is a pointer to a subtree.
//        • ki <= smallest key in subtree ai and > largest in ai-1
////////////////////////////////////////////////////////////////////
class BPTree
{
private:
  // a structure in the BPTree class - aka one node within the class
  struct nodeObject
  {                 
    int numKeys = 0;  // number of keys in this node
    int numKids = 0;  // number of children in the node (numKids - 1 = numKeys)
    nodeObject *prevLeaf = nullptr; // holds a pointer to the previous leaf, if this is a leaf node - otherwise we won't use this
    nodeObject *nextLeaf = nullptr; // holds a pointer to the next leaf, if this is a leaf node - otherwise we won't use it
    // we also won't know until later how many keys will be stored in here
    int *keys;           // should be empty if this is a leaf node
    Pair *keyValues;     // should be empty if this is an index node
    nodeObject **children; // an array which holds pointers to the children - set it to null when you can
    bool isLeaf;  // true if this node is a leaf
    bool isRoot;  // true if this node is a root
  };

  typedef nodeObject *nodePointer; //define pointers to node objects - makes it easier for me later on
  nodePointer root; // pointer to the root node
  // least ceil(m/2) children, and at most m children
  int minKids = 0; // at least ceil(m/2) children
  int maxKids = 1; // at most m children
  // there can be between ceil(m/2)-1 keys and m-1 keys
  int minKeys = 0;  // ceil(m/2)-1 keys
  int maxKeys = 1;  // m-1 keys

public:
// methods
  void deletePair(int key); // deletes a pair from a node and is responsible for calling all other functions pertaining to delete
  std::string search(int key);  // searches for the value which corresponds to key
  std::string search(int key, int key2);  // searches for the range of values which correspond to keys that are >key1 and <key2
  bool deleteKeyAndKeyValuePair(nodePointer toBeDeletedFrom, int key);  // deletes a key and keyValue pair from a leaf node
  bool deleteKey(nodePointer toBeDeletedFrom, int key); // deletes the key from a node & decreases numKey
  bool deleteKeyValuePair(nodePointer toBeDeletedFrom, int key, int numKeyValues);  // deletes a keyValue from a leaf node
  int determineChildNum(nodePointer parent, nodePointer child); // determines the number of the child in the parent's children array
  void insertAsReplacement(nodePointer issue, nodePointer parentOfIssue, nodePointer sibling, bool isBigger); // replaces keys/values in one node with keys/values with another node
  void recalculateParentKeys(nodePointer parent); // recalculates the keys in the given node using the array of children
  void deleteNode(nodePointer node, nodePointer parent);  // deletes a node from its parent (over-writes that child pointer with another to make the parent forget it existed)
  nodePointer getLegalIndexSibling(nodePointer index);    // returns a nodePointer to the best sibling to borrow from (built for index nodes because leaf nodes have sibling pointers)
  void insertParentKeysIntoIndexNode(nodePointer parent, nodePointer receivingNode);  // pull the parent's keys down into the index node given
  void insertChildrenIntoIndexNode(nodePointer node, nodePointer receiver); // insert children from a given node into the receiving node which has too few children
  void percolateUpwards(nodePointer parent);  // checks if the parent is legal and if not, redoes the merge
  nodePointer sharedElder(nodePointer a, nodePointer b);  // finds the youngest shared elder of a and b (e.g. if a and b are cousins, a pointer to the grandparent would be returned)
  void mergeWithSibling(nodePointer node, nodePointer parent, nodePointer sibling, nodePointer parentOfSibling);  // merge a sibling into a node, and make sure that both parents are ok
  void updateGrandparent(nodePointer node); // update the keys of the grandparent to make sure they are alright

  ///////////////////////////////////////////////////////
  // initialize the m-way tree
  ///////////////////////////////////////////////////////
  void initialize(int m)  // initializes the m-way tree
  {
    //m (like m-way tree) - every node except root can have at most m children
    maxKids = m;
    minKids = ceil(maxKids/ 2.0); // at least ceil(m/2) children
    
    // there can be between ceil(m/2)-1 keys and m-1 keys unless you are the root, then your minKeys = 2
    minKeys = minKids - 1;
    if(minKeys == 0)
    {
      minKeys++;
    }

    maxKeys = maxKids - 1;  // keys is 1 less than max children
    root = nullptr; // initalize root to nullptr
    if(DEBUG)cout << "New " << maxKids << "-way tree was initialized" << endl;
  }

  //////////////////////////////////////////////////////////
  // insert a key-value pair into a leaf node
  // and attach it to the root if it is a new leaf node
  //////////////////////////////////////////////////////////
  void insert(int key, float value) // insert a key-value pair into a leaf node
  {
    if(DEBUG)cout << "INSERT" << endl;
    // create a new nodePointer and 
    // determine the leaf node into which the key-value pair should be inserted(key)
    nodePointer current = determineLeafNode(key);
    if(DEBUG)cout << "need to insert a new node here: " << endl;
    if(DEBUG)printNode("insert new node here", current);

    // if the nodePointer is currently nullptr, there is no root yet, so make one
    //  initialize new node
    //  newNode is a leaf and a root
    //  root = newNode
    if(current == nullptr)
    {
      current = initializeNode();
      if(DEBUG)cout << "just initialized a node as a root node" << endl;
      // there is no root yet, so this is a root and a leaf
      current->isLeaf = true; 
      current->isRoot = true;
      root = current; // set the root to be this new node you just created
    }
    
    // this happens regardless of whether it's a new root or not
    // insert the key-value pair into the leaf node you found at the numKey position
    // insert the key into the leaf node you found at the numKey position
    // increase numKeys by 1
    // re-sort the key-value pairs in the leaf by key
    // re-sort the keys in the leaf
    current->keyValues[current->numKeys] = * new Pair(key,value);
    current->keys[current->numKeys] = key;
    current->numKeys++;
    sortKeyValues(current->keyValues, current->numKeys);
    sortKeys(current->keys, current->numKeys);

    if(DEBUG)printNode("after adding the key and sorting", current);

    // if what you did was legal (numKeys < maxKeys), you are done
    if(current->numKeys <= maxKeys) 
    {
      return;
    }

    // else the numKeys > maxKeys, you need to:
    //  split the node in half
    //    create a large leaf node by initializing it
    //    set isLeaf for the large leaf to true. It's not a root
    //    this large leaf node receives (maxKids+1)/2 key-value pairs
    //    this large leaf node receives (maxKids+1)/2 keys
    //    this large leaf increases numKeys with each pair it gets
    //  this current leaf node can keep all the key-value pairs, just update numKeys to be 
    //  how many numKeys it's actually supposed to have, namely maxKids/2
    
      if(DEBUG)cout << "NumKeys > maxKeys" << endl;

      nodePointer largeLeaf = initializeNode();
      largeLeaf->isLeaf = true;
      largeLeaf->isRoot = false;
      
      int smallLeafKeys = (maxKids)/2;
      if(DEBUG)cout << "maxkids: " << maxKids << endl;
      if(DEBUG)cout << "smallLeafKeys: " << smallLeafKeys << endl;
      int largeLeafKeys = maxKids - smallLeafKeys;
      if(DEBUG)cout << "LargeLeafKeys: " << largeLeafKeys << endl;
      for(int i = 0; i < largeLeafKeys; i++)
      {
        if(DEBUG)cout << i << endl;
        largeLeaf->keyValues[i] = current->keyValues[i+smallLeafKeys];
        largeLeaf->keys[i] = current->keys[i+smallLeafKeys];
        if(DEBUG)cout << "large leaf keys[" << i << "] is " << largeLeaf->keys[i] << endl;
        largeLeaf->numKeys++;
        
      }
      if(DEBUG)printNode("large", largeLeaf);
      current->numKeys = smallLeafKeys;
    
    
    // run the stepAfter the split(current Leaf, largeLeaf)
    // -999 indicates that there is no specific number that needs 
    // to be inserted into the parent, just use the default one
    stepAfterSplit(current, largeLeaf, -999); 

    // find the parent
    nodePointer parent = findParent(current);

    // if you reach here, you have definitely inserted a leaf node
    // iterate through the leaf nodes of the new parent, and assign every middle child
    // to what its new siblings are
    assignSiblingsDuringInsert(parent);
    // after you've gone through the entire sibling array of this parent, just check if more siblings popped up somewhere
    // start at the leftmost leaf and work your way right
    checkAllSiblings(determineLeafNode(0), determineLeafNode(99999)); // may be extraneous for some locations, but is necessary for edge cases
    if(DEBUG)printLinkedListGivenPointer(parent->children[0]);
  }

  /////////////////////////////////////////////////////////////////////
  // checks all the sibling pointers, starting at the smallest node
  // and makes sure they are set correctly
  // does the same in the opposite direction, from the largest node
  /////////////////////////////////////////////////////////////////////
  void checkAllSiblings(nodePointer first, nodePointer last)  // checks all the sibling pointers, starting at the smallest node
  {
    while(first->nextLeaf != nullptr)
    {
      if(DEBUG)printNode("current first", first);
      if(first->nextLeaf->prevLeaf != first)
      {
        first->nextLeaf = first->nextLeaf->prevLeaf;
        first->nextLeaf->prevLeaf = first;
      } // otherwise, leave them alone and move to the next one
      first = first->nextLeaf;
    }
    while(last->prevLeaf != nullptr)
    {
      if(DEBUG)printNode("current last", last);
      if(last->prevLeaf->nextLeaf != last)
      {
        // if they are not the same
        last->prevLeaf = last->prevLeaf->nextLeaf;
        last->prevLeaf->nextLeaf = last;
      }// otherwise, leave them alone and move to the next one
      last = last->prevLeaf;
    }
  }

  /////////////////////////////////////////////////////////////////////////
  // assign siblings
  // if it's the first child, assign the next sibling
  //  also look backwards and possibly re-assign the prev sibling
  //  if your previousNode != nullptr, you have someone behind you who might have someone in front of them
  //    look at the previous node's next node. If the previous node's next node is not you, make that your previous node instead
  //
  // else if it's the last child, assign the previous sibling
  // also look forward and possibly re-assign the next sibling
  //  if your nextNode != nullptr, you have someone in front of you who might have someone behind them
  //    look at the next node's previous node. If the next node's previous node is not you, make that your next node instead
  //
  // else it is a middle child
  // simply assign the next and previous nodes accordingly
  /////////////////////////////////////////////////////////////////////////
  void assignSiblingsDuringInsert(nodePointer parent) // when you insert a new node, assign the siblings to each other (nextLeaf, prevLeaf)
  {
    for(int i = 0; i < parent->numKids; i++)  // go through the children array in of the parent node given
    {
      if(i == 0)  // if it's the first child
      {
        if(DEBUG)cout << "assigned child at " << i << " from if" << endl;
        parent->children[i]->nextLeaf = parent->children[i+1];
        if(parent->children[i]->prevLeaf != nullptr)
        {
          if(parent->children[i]->prevLeaf->nextLeaf != parent->children[i])
          {
            parent->children[i]->prevLeaf = parent->children[i]->prevLeaf->nextLeaf;
          }
        }
      }

      else if(i == parent->numKids-1)  // if it's the last child
      {
        if(DEBUG)cout << "assigned child at " << i << " from else if" << endl;
        parent->children[i]->prevLeaf = parent->children[i-1];
        if(parent->children[i]->nextLeaf != nullptr)
        {
          if(parent->children[i]->prevLeaf->nextLeaf != parent->children[i])
          {
            parent->children[i]->nextLeaf = parent->children[i]->nextLeaf->prevLeaf;
          }
        }
      }

      else  // else it is a middle child
      {
        if(DEBUG)cout << "assigned child at " << i << " from else" << endl;
        parent->children[i]->nextLeaf = parent->children[i+1];
        parent->children[i]->prevLeaf = parent->children[i-1];
      }
    }

  }

  ///////////////////////////////////////////////////////////////////////////////////
  // determine the appropriate node of a given key (used for insert, search, delete)
  // nodePointer determineLeafNode(key)
  //  if root = nullptr, return nullptr
  //  otherwise:
  //  new nodePointer called seeker = root
  //  while(seeker is not a leaf node)
  //    seeker = determine which child you need to travel down(seeker, key)
  //    hope that it didn't return nullptr
  //  return the seeker because you know it is a root because it came out of the while loop
  ///////////////////////////////////////////////////////////////////////////////////
  nodePointer determineLeafNode(int key)  // determine the appropriate node of a given key (used for insert, search, delete)
  {
    if(DEBUG)cout << "DETERMINE-LEAF-NODE" << endl;
    if(DEBUG) cout << "determine leaf node, key: " << key << endl;
    if(root == nullptr) 
    {
      return nullptr;
    }
    else
    {
      nodePointer seeker = root;
      if(DEBUG)printNode("seeker", seeker);
      if(DEBUG)cout << "keys of seeker" << seeker->numKeys << endl;
      while(seeker->isLeaf == false)
      {
        if(DEBUG)cout << "seeking which child to travel down" << endl;
        seeker = determineWhichChildToTravelDown(seeker, key);
        if(DEBUG)printNode("current seeker", seeker);
        if(DEBUG)cout << "seeker->isLeaf " << seeker->isLeaf << endl;
        if(DEBUG)cout << "determined which child to travel down" << endl;
      }
      if(DEBUG)cout << "found the leaf node and returning it" << endl;
      return seeker;
    }
  }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // stepAfterSplit(original node, largeNode)
    // now that you have split the node correctly, check if the original node used to be the root (isRoot)
    //  if it was the root, initialize a new node
    //    set isRoot = true
    //    set isLeaf = false
    //    the current leaf node you've been working on becomes non-root (isRoot = false)
    //      set it to non-root whether or not you've been working on a root
    //    assign the small node and the large node as the two children (it's a new root node so it only has 2 children)
    //    make this new node the new root
    // else if it was not the root, you need to find the parent of the current leaf
    //  the current leaf stays in the array of children of its parent
    //  the parent receives a new child and a new key into its arrays (namely the large node)
    //  sort the parent's child array and the parent's key array
    //  the parent's numKey and numChildren also increases by 1
    //  if numKey < maxKeys, you are safe and you are done
    //  else if the numKey is now larger than it is supposed to be (aka numKey >= maxKeys)
    //    pass this now-messed-up parent to be fixed by being split
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void stepAfterSplit(nodePointer original, nodePointer large, int numToBeMerged) // now that you have split the node correctly, check if the original node used to be the root (isRoot). if it was not the root, you need to find the parent of the current leaf
    {
      if(DEBUG)cout << "STEP-AFTER-SPLIT" << endl;
      if(DEBUG)printNode("original", original);
      if(DEBUG)printNode("largenode", large);

      nodePointer newNode;  // make a new node
      if(original->isRoot == true)  // was the original given node the root? if yes, make a new node and make it the root
      {
        newNode = initializeNode();
        newNode->isRoot = true;
        newNode->isLeaf = false;
        original->isRoot = false;
        newNode->children[0] = original;
        newNode->children[1] = large;
        if(numToBeMerged != -999)
        {
          newNode->keys[newNode->numKeys] = numToBeMerged; // if the index node split, use the given number as the last key
        }
        else
        {
          newNode->keys[0] = large->keys[0];
        }
        
        newNode->numKeys++;
        newNode->numKids = 2;
        root = newNode;
        if(DEBUG)printNode("new Root created", root);
      }
      else  // this was an index node but not a root, so merge the parent of the given node with the large to be merged
      {
        newNode = findParent(original);
        newNode->children[newNode->numKids] = large;
        newNode->numKids++;
        if(numToBeMerged != -999) // if the index node split, use the given number as the last key
        {
          newNode->keys[newNode->numKeys] = numToBeMerged;
        }
        else
        {
          newNode->keys[newNode->numKeys] = large->keys[0]; // the smallest key from large
        }
        
        newNode->numKeys++;
        sortChildren(newNode->children, newNode->numKids);  // sort the children array from smallest to largest
        sortKeys(newNode->keys, newNode->numKeys);  // sort the children array from smallest to largest

        // this is an index node, so it has children. If the children are leaves, make sure that they know 
        // who their siblings are
        if(newNode->children[0]->isLeaf)  // if one child is a leaf, they are all children
        {
          assignSiblingsDuringInsert(newNode);
          // after you've gone through the entire sibling array of this parent, just check if more siblings popped up somewhere
          // start at the leftmost leaf and work your way right
          checkAllSiblings(determineLeafNode(0), determineLeafNode(99999)); // may be extraneous for some locations, but is necessary for edge cases
        }

        if(newNode->numKeys <= maxKeys)  // you are done
        {
          if(DEBUG)printNode("newNode from stepAfterSplit", newNode);
          return;
        }
          
        else  // if the new node has too many keys to deal with, update the parent (it's >=)
        {
          if(DEBUG)cout << "too many keys... now updating parent" << endl;
          updateParent(newNode);
          if(DEBUG)printNode("updatedParent looks like this", newNode);
        }
      }
      
    }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // updateParent
  // we know that this is only called if numKey >= maxKeys
  // the parent has too many children and too many keys
  // split the node in half
  //  create a new large index node by initializing it
  //    set isLeaf and isRoot to false for the large index node
  //    the large indexNode receives the last couple keys (maxKeys+1 - (maxKeys+1)/2)
  //    the large indexNode receives the last couple children (maxKids+1 - (maxKids+1)/2)
  //    numKids and numKeys is updated for the large node
  //  the current node doesn't have its children or keys changed
  //  the current node has its numKids =(maxKids+1)/2 and numKeys =(maxKeys + 1)/2 changed
  // now that you have split the node correctly, run the stepAfterSplit(small/original node, large node)
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  void updateParent(nodePointer toBeUpdated)  // takes a given node and splits it into a smaller and larger node, then calls the step after split so we are left with one node
  {
    if(DEBUG)cout << "UPDATE-PARENT" << endl;
    nodePointer largeIndex = initializeNode();
    largeIndex->isLeaf = false;
    largeIndex->isRoot = false;
    int smallToReceive = (maxKeys+1)/2;
    if(DEBUG)cout << "smallToReceive " << smallToReceive << endl;
    int largeToReceive = maxKeys+1 - smallToReceive;  // made this 1 smaller
    if(DEBUG)cout << "largeToReceive " << largeToReceive << endl;
    int i = 0;
    if(DEBUG)printNode("toBeUpdated", toBeUpdated);
    for(i = 0; i <largeToReceive-1; i++)  // changed from 0 to 1 to not keep the largest number
    {
      largeIndex->keys[i] = toBeUpdated->keys[i+smallToReceive+1];  // added a 1 here
      largeIndex->numKeys++;
      largeIndex->children[i] = toBeUpdated->children[i+smallToReceive+1];  // added a 1 here
      largeIndex->numKids++;
      if(DEBUG)cout << "i: " << i << endl;
      if(DEBUG)printNode("largeIndex currently", largeIndex);
    }
    largeIndex->children[i] = toBeUpdated->children[i+smallToReceive+1];  // one more time to get all the kids
    largeIndex->numKids++;
    if(DEBUG)printNode("updateParent - largeIndex after for loop", largeIndex);

    toBeUpdated->numKeys = smallToReceive;
    toBeUpdated->numKids = smallToReceive+1;
    if(DEBUG)cout << "number being appended is " << toBeUpdated->keys[smallToReceive];
    stepAfterSplit(toBeUpdated, largeIndex, toBeUpdated->keys[smallToReceive]);
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // void initializeNode(node)
  //  numKeys = 0;
  //  keys = new int[maxKeys+1];
  //  keyValues = new Pair[maxKeys+1];     // should be empty if this is an index node
  //  children = new nodePointer[maxKids+1]; // holds pointers to the children - set it to null when you can
  //  int numKids = 0;
  // isLeaf and isRoot, as well as sibling pointers will be set elsewhere
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  nodePointer initializeNode()  // initializes a node with numKeys and also with the sizes of all the arrays - returns the new node
  {
    if(DEBUG)cout << "INITIALIZE-NODE" << endl;
    nodePointer node = new nodeObject();
    node->numKeys = 0;
    node->keys = new int[maxKeys+1];
    node->keyValues = new Pair[maxKeys+1];
    node->children = new nodePointer[maxKids+1];
    node->numKids = 0;
    return node;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // nodePointer findParent(nodePointer)
  // start at the root
  // while(seeker != leaf) because the leaves don't have children to search
  // make a new nodePointer which = the next child to travel down
  // determine which child to travel down by looking at all the children and searching for the first key of the child
  // hope that it didn't return nullptr
  // check if the child is the nodePointer you are looking for
  //  if yes, return the current node (which is the parent of the node it was given)
  //  if no, travel to the child and make that the new seeker
  // if you dropped out of the while loop, there is a problem. Node (and corresponding parent) not found.
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  nodePointer findParent(nodePointer child) // returns the parent of a given node. If the parent could not be found, return nullptr
  {
    if(DEBUG)cout << "FIND-PARENT" << endl;
    nodePointer seeker = root;
    nodePointer traveller = seeker;
    
    while(seeker->isLeaf == false)
    {
      if(DEBUG) printNode("current seeker", seeker);
      traveller = determineWhichChildToTravelDown(seeker, child->keys[0]);
      
      if(DEBUG) printNode("traveller was found to be", traveller);
      //printNode("child being looked for", child);
      if(traveller == child) 
      {
        if(DEBUG)cout << "returning seeker" << endl;
        return seeker;
      }
        
      else
      {
        if(DEBUG)cout << "need to find a new traveller" << endl;
        seeker = traveller;
      }
        
    }
    if(DEBUG)cout << "findParent could not find a parent for the child.";
    return nullptr;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // nodePointer determineWhichChildToTravelDown(parent, keyBeingSearchedFor)
  //  go through all the keys (numKeys) of the parent
  //  at each key, check if  is >, <, or = to the keyBeingSearchedFor
  //    if there is no key at that location (i >= numKeys) return the child at i (there is always 1 more child than numKeys)
  //    if the key > keyBeingSearchedFor, return the child at that key number
  //    if the key <= keyBeingSearchedFor, increase the iterator by 1
  //  we should always have returned before this, but just in case, return nullptr to indicate there was no child
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  nodePointer determineWhichChildToTravelDown(nodePointer parent, int keyBeingSearchedFor)  // goes through the given node and compares the keys with the key being searched for. Finds and returns a pointer to the child which should be travelled down next
  {
    if(DEBUG)cout << "DETERMINE-WHICH-CHILD-TO-TRAVEL-DOWN" << endl;
    int i = 0;
    if(DEBUG)printNode("parent", parent);
    for(i = 0; i < parent->numKeys; i++)
    {
      if(DEBUG)cout << "i: " << i << endl;
      if(i >= parent->numKeys)
      {
        if(DEBUG)cout << "returning the child at " << i << endl;
        return parent->children[i];
      }
      else if(parent->keys[i] > keyBeingSearchedFor)
      {
        if(DEBUG)cout << "returning the child at " << i << endl;
        return parent->children[i];
      }
        
    }
    if(DEBUG)cout << "Returning the child at " << i << endl;
    if(DEBUG)printChildren("parent", parent);
    if(DEBUG)printNode("cchild at 2", parent->children[2]);
    return parent->children[i];
  }


  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // sort an array of children, which contains keys. puts the children in order from smallest to largest
  // go through the array of children and sort them according to their smallest keys
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  void sortChildren(nodePointer *arr, int len)  // go through the array of children and sort them according to their smallest keys
  {
    if(DEBUG) cout << "sorting children" << endl;
    nodePointer min = nullptr;    //minimum value
    int spot = len - 1;           //the spot of the minimum value
    for (int i = 0; i < len; i++) //holds the spot where the next number would go
    {
      spot = i;
      if(DEBUG) cout << "i is: " << i << endl;
      min = arr[i];
      for (int j = i; j < len; j++) //tries to find the next min
      {
        if(DEBUG) cout << "j is: " << j << endl;
        if (min->keys[0] > arr[j]->keys[0]) //check that the arr[j] isn't smaller than the current min, otherwise you flip it
        {
          spot = j;
          min = arr[j];
        }
      }
      arr[spot] = arr[i]; //flip the min and the i you were at
      arr[i] = min;
    }
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // go through the keyValue pairs and sort them according to their smallest keys
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  void sortKeyValues(Pair *arr, int len) // go through the keyValue pairs and sort them according to their smallest keys
  {
    Pair min = arr[0];    //minimum value
    int spot = len - 1;           //the spot of the minimum value
    for (int i = 0; i < len; i++) //holds the spot where the next number would go
    {
      spot = i;
      //std::cout << "i is: " << i << "\n";
      min = arr[i];
      for (int j = i; j < len; j++) //tries to find the next min
      {
        if (min.key > arr[j].key) //check that the arr[j] isn't smaller than the current min, otherwise you flip it
        {
          spot = j;
          min = arr[j];
        }
      }
      arr[spot] = arr[i]; //flip the min and the i you were at
      arr[i] = min;
    }
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // go through the array of keys and sort them according to the smallest keys
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  void sortKeys(int *arr, int len) // go through the array of keys and sort them according to the smallest keys
  {
    int min = arr[0];    //minimum value
    int spot = len - 1;           //the spot of the minimum value
    for (int i = 0; i < len; i++) //holds the spot where the next number would go
    {
      spot = i;
      //std::cout << "i is: " << i << "\n";
      min = arr[i];
      for (int j = i; j < len; j++) //tries to find the next min
      {
        if (min > arr[j]) //check that the arr[j] isn't smaller than the current min, otherwise you flip it
        {
          spot = j;
          min = arr[j];
        }
      }
      arr[spot] = arr[i]; //flip the min and the i you were at
      arr[i] = min;
    }
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//  prints a given node with its name, in the form
// name: [key1 key2 key3 ]
//////////////////////////////////////////////////////////////////////////////////////////////////////////
  void printNode(string name, nodePointer node) //  prints a given node with its name
  {
    if(node == nullptr)
    {
      cout << name << " is blank right now" << endl;
    }
    else
    {
      cout << name << ": [";
      for(int i = 0 ; i < node->numKeys; i++)
      {
        cout << node->keys[i] << " ";
      }
      cout << "]" << endl;
    }
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // prints the given node's key-value pairs, in the form
  // name: [key1(value1) key2(value2) key3(value3) ]
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  void printNodePairs(string name, nodePointer node) // prints the given node's key-value pairs
  {
    if(node == nullptr)
    {
      cout << name << " is blank right now" << endl;
    }
    else
    {
      cout << name << ": [";
      for(int i = 0 ; i < node->numKeys; i++)
      {
        node->keyValues[i].printPair();
        cout << " ";
      }
      cout << "]" << endl;
    }
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // goes through the node and prints all the children on a new line, in the form
  // name has 3 kids
  // child: [key1 key2]
  // child: [key1 key2]
  // child: [key1 key2]
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  void printChildren(string name, nodePointer node) // prints all the children of a given node on a new line
  {
    if(node == nullptr)
    {
      cout << name << " is blank right now" << endl;
    }
    else if(node->numKids == 0)
    {
      cout << "no kids to print" << endl;
    }
    else
    {
      cout << name << " has " << node->numKids << " kids" << endl;
      for(int i = 0; i < node->numKids; i++)
      {
        printNode("child ", node->children[i]);
      }
    }
    
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// print the tree starting at the root, for maximum of 3 levels
//////////////////////////////////////////////////////////////////////////////////////////////////////////
  void printTree()  // print the tree starting at the root, for maximum of 3 levels
  {
    cout << "PRINT-TREE" << endl;
    if(root == nullptr)
    {
      cout << "root is blank right now" << endl;
    }
    else
    {
      printNode("root", root);
      // level 1
      if(root->numKids > 0)
      {
          printChildren("root", root);
          // level 2
          if(root->children[0]->children[0] != nullptr)
          {
            printChildren("child at 0", root->children[0]);
            printChildren("child at 1", root->children[1]);
            if(root->numKids > 2)
            {
              printChildren("child at 3", root->children[2]);
            }
          }
      }
      
    }
    
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // prints the linked list starting with key1, in the form
  // Linked list starting at key1
  // linked list: [key1 key2]
  // linked list: [key1 key2]
  // linked list: [key1 key2]
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  void printLinkedList(int key1)  // prints the linked list starting with key1
  {
    if(root == nullptr)
    {
      return;
    }
    nodePointer current = determineLeafNode(key1);
    cout << "Linked list starting at " << key1 << endl;
    printNode("linked list",current);
    while(current->nextLeaf != nullptr)
    {
      current = current->nextLeaf;
      printNode("linked list",current);
    }
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// print linked list starting at given nodePointer
//////////////////////////////////////////////////////////////////////////////////////////////////////////
  void printLinkedListGivenPointer(nodePointer startHere) // print linked list starting at given nodePointer
  {
    cout << "Linked list starting at " << endl;
    printNode("linked list",startHere);
    while(startHere->nextLeaf != nullptr)
    {
      startHere = startHere->nextLeaf;
      printNode("linked list",startHere);
    }
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // print linked list starting with key1
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  void printLinkedListPairs(int key1) // print linked list starting with key1
  {
    if(root == nullptr)
    {
      return;
    }
    nodePointer current = determineLeafNode(key1);
    cout << "Linked list starting at " << key1 << endl;
    printNodePairs("linked list",current);
    while(current->nextLeaf != nullptr)
    {
      current = current->nextLeaf;
      printNodePairs("linked list",current);
    }
  }

};
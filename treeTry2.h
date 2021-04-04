#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using std::string;
using namespace std;
#include <math.h>
#include <algorithm>
#include <array>

// class which holds the key and value, allows editing and deleting
class Pair
{
  public:
  // You can use integer as the type of the key and float/double as the type of the value.
  int key;
  float value;

  Pair(int k, float v)
  {
    key = k;
    value = v;
  }

  Pair()
  {
    key = 0;
    value = 0;
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
  {                 //Create the node object with values of data, position, and endPosition
    int numKeys = 0;
    
    nodeObject *prevLeaf = nullptr; // holds a pointer to the previous leaf, if this is a leaf node - otherwise we won't use this
    nodeObject *nextLeaf = nullptr; // holds a pointer to the next leaf, if this is a leaf node - otherwise we won't use it
    // we also won't know until later how many keys will be stored in here
    int *keys;           // should be empty if this is a leaf node
    Pair *keyValues;     // should be empty if this is an index node
    bool isLeaf;
    bool isRoot;
    // we won't know until later how many children there will be, so make an array of child pointers
    nodeObject **children; // holds pointers to the children - set it to null when you can
    int numKids = 0;
  };
  typedef nodeObject *nodePointer; //define pointers to node objects - makes it easier for me later on
  nodePointer rootNode;            // pointer to the root node
  // least ceil(m/2) children, and at most m children
  // there can be between ceil(m/2)-1 keys and m-1 keys
  int minKids = 0; // at least ceil(m/2) children
  int maxKids = 1; // at most m children
  int minKeys = 0;
  int maxKeys = 1;
  //bool hasRoot = false; // true if a root exists already, false if not

public:
  void deletePair(int key); // if all nodes were deleted, set hasRoot to false
  void search(int key);
  void search(int key, int key2);

  // methods
  void initialize(int m)
  {
    //m (like m-way tree) - every node except root can have at most m children
    maxKids = m;
    // at least ceil(m/2) children
    minKids = ceil(maxKids / 2);
    // there can be between ceil(m/2)-1 keys and m-1 keys unless you are the root, then your minKeys = 2
    minKeys = ceil(maxKids / 2) - 1;
    maxKeys = maxKids - 1;
    rootNode = nullptr;
  }

  // insert a key-value pair into a leaf node
  void insert(int key, float value)
  {
    // create a new nodePointer and 
    // determine the leaf node into which the key-value pair should be inserted(key)
    // if the nodePointer is currently nullptr, there is no root yet, so make one
    //  initialize new node
    //  newNode is a leaf and a root
    // root = newNode

    // this happens regardless of whether it's a new root or not
    // insert the key-value pair into the leaf node you found at the numKey position
    // insert the key into the leaf node you found at the numKey position
    // increase numKeys by 1
    // re-sort the key-value pairs in the leaf by key

    // if what you did was legal (numKeys < maxKeys), you are done
    // else if the numKeys >= maxKeys, you need to:
    //  split the node in half
    //    create a large leaf node by initializing it
    //    set isLeaf for the large leaf to true
    //    this large leaf node receives (maxKids+1)/2 key-value pairs
    //  this current leaf node can keep all the key-value pairs, just update numKeys to be 
    //  how many numKeys it's actually supposed to have, namely maxKids/2
    
    // run the stepAfter the split(current Leaf, largeLeaf)
  }

  // nodePointer determineLeafNode(key)
  //  if root = nullptr, return nullptr
  //  otherwise:
  //  new nodePointer called seeker = root
  //  while(seeker is not a leaf node)
  //    seeker = determine which child you need to travel down(seeker, key)
  //    hope that it didn't return nullptr
  //  return the seeker because you know it is a root because it came out of the while loop

  // stepAfterSplit(original node, largeNode)
      // now that you have split the node correctly, check if the original node used to be the root (isRoot)
    //  if it was the root, initialize a new node
    //    set isRoot = true
    //    set isLeaf = false
    //    the current leaf node you've been working on becomes non-root (isRoot = false)
    //      set it to non-root whether or not you've been working on a root
    //    assign the small node and the large node as the two children (it's a new root node so it only has 2 children)
    //    make this new node the new root
    // if it was not the root, you need to find the parent of the current leaf
    //  the current leaf stays in the array of children of its parent
    //  the parent receives a new child and a new key into its child array
    //  sort the parent's child array and the parent's key array
    //  the parent's numKey and numChildren also increases by 1
    //  if numKey < maxKeys, you are safe and you are done
    //  else if the numKey is now larger than it is supposed to be (aka numKey >= maxKeys)
    //    pass this now-messed-up parent to be fixed by being split

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

  // initializeNode
  //  numKeys = 0;
  //  keys = new int[maxKeys+1];
  //  keyValues = new int[maxKeys+1];     // should be empty if this is an index node
  //  children = new nodePointer[maxKids+1]; // holds pointers to the children - set it to null when you can
  //  int numKids = 0;
  // isLeaf and isRoot, as well as sibling pointers will be set elsewhere

  // nodePointer findParent(nodePointer)
  // start at the root
  // while(seeker != leaf) because the leaves don't have children to search
  // make a new nodePointer which = the next child to travel down
  // determine which child to travel down
  // hope that it didn't return nullptr
  // check if the child is the nodePointer you are looking for
  //  if yes, return the current node (which is the parent of the node it was given)
  //  if no, travel to the child and make that the new seeker
  // if you dropped out of the while loop, there is a problem. Node (and corresponding parent) not found.

  // nodePointer determineWhichChildToTravelDown(parent, keyBeingSearchedFor)
  //  go through all the keys (numKeys) of the parent
  //  at each key, check if  is >, <, or = to the keyBeingSearchedFor
  //    if there is no key at that location (i >= numKeys) return the child at i (there is always 1 more child than numKeys)
  //    if the key > keyBeingSearchedFor, return the child at that key number
  //    if the key <= keyBeingSearchedFor, increase the iterator by 1
  //  we should always have returned before this, but just in case, return nullptr to indicate there was no child
};
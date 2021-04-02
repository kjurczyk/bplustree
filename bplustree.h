#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using std::string;
using namespace std;
#include <math.h>

// Now to insert some variables that will always be accessible
// global variables will be listed with g_ in front of name
//BPTree bpt = new BPTree();
int ROOT = 0;
int INDEX = 1;
int LEAF = 2;

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
};

/////////////////////////////////////////////////////////////////
// Remaining nodes have following structure:
//      j a0 k1 a1 k2 a2 …kj aj
//        • j = number of keys in node.
//        • ai is a pointer to a subtree.
//        • ki <= smallest key in subtree ai and > largest in ai-1
////////////////////////////////////////////////////////////////////
class IndexNode
{
  public: 
  // variables
  int numKeys;  // number of keys in the node
  int maxKeys; // the maximum number of keys that can be had
  int allKeys[]; // array of subtrees

  IndexNode(int max)
  {
    maxKeys = max;
    allKeys[maxKeys];
  }
};

/////////////////////////////////////////////////////////////////
// Dictionary pairs are in leaves only. Leaves form a
// doubly-linked list.
/////////////////////////////////////////////////////////////////
/*
class DataNode
{
  public:

  // variables
  Pair * info;

  DataNode()
  {
    info = new Pair();
  }
};
*/

/*
class Node
{
  int typeOfNode; // ROOT, INDEX, or LEAF

  // pointers
  // pointer to parent (if this is the root, point to null)
};
*/

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
    struct nodeObject{  //Create the node object with values of data, position, and endPosition
      int typeOfNode; // ROOT, INDEX, or LEAF
      int numKeys;
      nodeObject *parent = nullptr; // holds a pointer to the parent - set to null on default
      nodeObject *prevLeaf = nullptr; // holds a pointer to the previous leaf, if this is a leaf node - otherwise we won't use this
      nodeObject *nextLeaf = nullptr; // holds a pointer to the next leaf, if this is a leaf node - otherwise we won't use it
      // we also won't know until later how many keys will be stored in here
      vector<int> keys; // should be empty if this is a leaf node
      vector<Pair> keyValues; // should be empty if this is an index node
      
      // we won't know until later how many children there will be, so make an array of child pointers
      nodeObject *children[]; // holds pointers to the children - set it to null when you can
      
    };
    typedef nodeObject * nodePointer; //define pointers to node objects - makes it easier for me later on
    nodePointer rootNode;  // pointer to the root node
    int maxNodes = 1; //m (like m-way tree) - every node except root can have at 
    // least ceil(m/2) children, and at most m children
    // there can be between ceil(m/2)-1 keys and m-1 keys
    int minKids = 0;  // at least ceil(m/2) children
    int maxKids = 1;  // at most m children
    int minKeys = 0;
    int maxKeys = 1;
    //bool hasRoot = false; // true if a root exists already, false if not


  public:
  // all the variables that are stored in the b+ tree, namely
  // bool isRootNode;
  // bool isLeafNode;
  
  


  // methods
  void initialize(int m)
  {
    //m (like m-way tree) - every node except root can have at 
    maxNodes = m;
    // least ceil(m/2) children, and at most m children
    minKids = ceil(maxNodes/2);
    maxKids = maxNodes;
    // there can be between ceil(m/2)-1 keys and m-1 keys unless you are the root, then your minKeys = 2
    minKeys = ceil(maxNodes/2) - 1;
    maxKeys = maxNodes - 1;

  }

  // insert a key/value pair into an array at a specificied location
  // return the new array
  /*
  void insertKeyValueInLocation(float val, Pair vect, int location)
  {
    for(int i = 0)
  } 
  */

  // returns a pointer to the node that the key fits into
  nodePointer findSpot(int key)
  {
    nodePointer seeker = rootNode;  // start at the root node and work down
    // is this a leaf node?
    if(seeker->typeOfNode == LEAF)
    {
      // we can enter the data here
      
    }
    
    return seeker;
  }

  // do what needs to be done to add the node to that spot
  // might need to split the leaf to make room
  void addNode(nodePointer spot, int key, float value)
  {
    // can we place the key into this specific node?
      if(spot->numKeys + 1 <= maxKeys)
      {
        // find correct location in array to insert the key
        int location = 0;
        for(int i = 0; i < maxKeys; i++)
        {
          if(spot->keyValues[i].key > key)
          {
            location = i;
            break;
          }
        }
        // std::vector<Pair>::iterator it = spot->keyValues.begin();
        // it = it + location;
        Pair * p = new Pair(key, value);
        // insert into the key & value arrays
        //insertKeyValueInLocation(key, spot->keyValues, location);
        //spot->keyValues.insert(spot->keyValues.begin() + location, new Pair(key, value));
        spot->keyValues.push_back(*p);
        
        //insertValueInLocation(value, spot->values, location);
        //AM I ALLOWED TO USE VECTORS????
      }
  }

  void insert(int key, float value)
  {
    nodePointer spot = findSpot(key);
    // if this is the root node
    if(spot == NULL)  // this is the first node, so the root pointer points here
    {
      nodePointer rootNode = new nodeObject();
      rootNode->typeOfNode = LEAF;  // this is the root, but it's also a leaf - leaf wins out at beginning
      //rootNode->keys[maxKeys + 1]; // we don't want to get to the +1 keys, but we need it for a 
      // short time to store the data before we can split the node
      rootNode->children[maxKids];
      rootNode->numKeys = 0;
      //rootNode->keyValues[maxKeys + 1];
    }
    else  // we already have a root node so add another BPTree which has a max of maxNodes and is not the root (yet)
    {
      // find the correct node to add this to the tree
      addNode(spot, key, value);
      // add a data node in the correct spot
      // find the correct subtree to go to
      // check all your 
    }
    
    
  }
  void deletePair(int key); // if all nodes were deleted, set hasRoot to false
  void search(int key);
  void search(int key, int key2);

  BPTree()
  {
    rootNode = NULL;
    
  }

  BPTree(int t)
  {
    //rootNode = new IndexNode(maxNodes);
    //typeOfNode = t; // assigns the type of the node here
  }  

};




#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using std::string;
using namespace std;

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

  Pair()
  {
    key = 0;
    value = 0;
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

class Node
{
  int typeOfNode; // ROOT, INDEX, or LEAF

  // pointers
  // pointer to parent (if this is the root, point to null)
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
    struct nodeObject{  //Create the node object with values of data, position, and endPosition
      int typeOfNode; // ROOT, INDEX, or LEAF
      nodeObject *parent = nullptr; // holds a pointer to the parent - set to null on default
      // we won't know until later how many children there will be, so make an array of child pointers
      nodeObject *children[]; // holds pointers to the children - set it to null when you can
      nodeObject *prevLeaf = nullptr; // holds a pointer to the previous leaf, if this is a leaf node - otherwise we won't use this
      nodeObject *nextLeaf = nullptr; // holds a pointer to the next leaf, if this is a leaf node - otherwise we won't use it
    };
    typedef nodeObject * nodePointer; //define pointers to node objects - makes it easier for me later on
    nodePointer * rootNode;  // pointer to the root node
    


  public:
  // all the variables that are stored in the b+ tree, namely
  // bool isRootNode;
  // bool isLeafNode;
  
  
  int maxNodes; //m (like m-way tree) - every node except root can have at 
  // least ceil(m/2) children, and at most m children
  // there can be between ceil(m/2)-1 keys and m-1 keys
  int minKids;  // at least ceil(m/2) children
  int maxKids;  // at most m children

  // methods
  void initialize(int);
  void insert(int key, float value)
  {
    // add a data node in the correct spot
    // find the correct subtree to go to
    // check all your 
    // another BPTree which has a max of maxNodes and is not the root (yet)
  }
  void deletePair(int key);
  void search(int key);
  void search(int key, int key2);

  BPTree()
  {
    rootNode = new IndexNode(maxNodes);
    typeOfNode = ROOT;
  }

  BPTree(int t)
  {
    rootNode = new IndexNode(maxNodes);
    typeOfNode = t; // assigns the type of the node here
  }  

};




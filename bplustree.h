#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using std::string;
using namespace std;


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
  Pair * info;
  int numKeys;  // number of keys in the node


  IndexNode()
  {
    info = new Pair();
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

////////////////////////////////////////////////////////////////////
// class that holds the root node of a b+ tree 
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
  public:
  // all the variables that are stored in the b+ tree, namely
  bool isRootNode;
  IndexNode * rootNode;

  // methods
  void initialize(int);

  BPTree()
  {
    rootNode = new IndexNode();
    isRootNode = false;
  }

  BPTree(bool root)
  {
    rootNode = new IndexNode();
    isRootNode = root;
  }  

};


// Now to insert some variables that will always be accessible
// global variables will be listed with g_ in front of name
BPTree bpt = new BPTree();

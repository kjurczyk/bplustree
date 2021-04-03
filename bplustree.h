#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using std::string;
using namespace std;
#include <math.h>
#include <algorithm>
#include <array>

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
      int numKeys = 0;
      nodeObject *parent = nullptr; // holds a pointer to the parent - set to null on default
      nodeObject *prevLeaf = nullptr; // holds a pointer to the previous leaf, if this is a leaf node - otherwise we won't use this
      nodeObject *nextLeaf = nullptr; // holds a pointer to the next leaf, if this is a leaf node - otherwise we won't use it
      // we also won't know until later how many keys will be stored in here
      int * keys; // should be empty if this is a leaf node
      Pair * keyValues; // should be empty if this is an index node
      bool isRoot = false;  // false by default, but can be set true
      // we won't know until later how many children there will be, so make an array of child pointers
      nodeObject **children; // holds pointers to the children - set it to null when you can
      int numKids = 0;
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
    if(rootNode == nullptr)
    {
      std::cout << "findspot: root node was null" << endl;
      return nullptr;
    }
       
    nodePointer seeker = rootNode;  // start at the root node and work down
    int i = 0;
    //cout << rootNode->keys[0] << endl;
    //cout << "hi" << endl;
    // compare the key to the root node keys - if there are no more keys to go through, drop out
    while(i < seeker->numKeys) // if it's a leaf, that's what we are looking for
    {
      cout << "i: " << i << endl;
      if(seeker->typeOfNode == LEAF)
      {
        i = seeker->numKeys;
      }
      // is the key smaller than the key at i? If the key is smaller, visit the previous child
      else if(key < seeker->keys[i])
      {
        seeker = seeker->children[i];
        // reset i to go through the next seeker
        i = 0;
      }
      else if(i+1 >= seeker->numKeys)  // if the next one will jump out, it must be a larger child
      {
        seeker = seeker->children[seeker->numKids - 1]; // the last child position
        // reset i to go through the next seeker
        i = 0;
      }
      i++;
    }
    // we'll drop out once we hit a leaf node
    //seeker->keys[0];
    // This is a leaf node
    
    cout << "the node at which to insert was found to be:" << endl;
    printNode(seeker);
    
    return seeker;
  }

  // do what needs to be done to add the node to that spot
  // might need to split the leaf to make room
  // might return a new indexNode with a child of the larger values
  // if there is no split that was done, just return null
  void addPair(nodePointer spot, int key, float value)
  {
    Pair * p = new Pair(key, value);
    // can we place the key into this specific node?
      if(spot->numKeys < maxKeys + 1)
      {
      //cout<<"if statement entered" <<endl;
        // find correct location in array to insert the key
        int location = spot->numKeys; // start with the last possible position in the array
        for(int i = spot->numKeys - 1; i >= 0; i--)
        {
          if(spot->keys[i] > key)  // find the correct location and move anything bigger backwards
          {
            location = i;
            spot->keys[i+1] = spot->keys[i];  // move it over by 1 so that the i spot can be overwritten
            spot->keyValues[i+1] = spot->keyValues[i];
          }
          else  // we've reached the smallest location 
          {  
            break;
          }
        }
        // insert into the appropriate location in the array
          spot->keys[location] = key;
          cout << "added key: " << spot->keys[location] << " at location: " << location << endl;
          spot->keyValues[location] = * new Pair(key, value);
          spot->numKeys = spot->numKeys+1;
          std::cout << "\tnumber of keys: " << spot->numKeys << endl;
          //std::cout << "/tthe number of keys in this node is " + spot->numKeys << " test" << endl;
        
        // std::vector<Pair>::iterator it = spot->keyValues.begin();
        // it = it + location;
        
        // insert into the key & value arrays
        //insertKeyValueInLocation(key, spot->keyValues, location);
        //spot->keyValues.insert(spot->keyValues.begin() + location, new Pair(key, value));
        
        /*spot->keyValues[spot->numKeys] = *p;
        spot->keys[spot->numKeys] = key;
        sort(*spot->keys,spot->numKeys+1);*/
        
        printNodeKeys(spot->numKeys, spot->keys);
        
        
        //insertValueInLocation(value, spot->values, location);
        
      }
      // we have added the key and value
      // now check if what we did was legal. If not, fix it
      if(spot->numKeys > maxKeys)
      {
        cout << "this was an illegal add. We need to fix it" << endl;
        // break the leaf node apart
        // determine where to split it
        int smallestLargeSpot = spot->numKeys/2;
        nodePointer smallNode = getSmallestNode(spot->keyValues, smallestLargeSpot);
        cout<< "successfully created a small node:" << endl;
        printNodeKeys(smallNode->numKeys, smallNode->keys);
        nodePointer largeNode = getLargestNode(spot->keyValues, smallestLargeSpot, spot->numKeys);
        cout<< "successfully created a large node:" << endl;
        printNodeKeys(largeNode->numKeys, largeNode->keys);

        // the smallNode should stay where it is, namely get the same parent it had before
        // so, in this case, just make the spot = smallest Node's keys & keyValues & numKeys
        spot->keys = smallNode->keys;
        spot->keyValues = smallNode->keyValues;
        spot->numKeys = smallNode->numKeys;
        // we could probably delete the small node now actually to save space
        // COME BACK HERE AT SOME POINT
        nodePointer pointerNode = new nodeObject();
        //cout << "hi" << endl;
        createNewIndexNode(largeNode, pointerNode);
        //cout<< "made a pointerNode";
        // the largeNode should get a parent pointer pointing to it which has a key value 
        // that is the smallest key value in largeNode
        // send it off to be compared to the next biggest (aka the parent of the current)
        // if there is no parent of the current (spot) that means this one should be the 
        // new parent of everything aka the root
        if(spot->parent == nullptr) // if this is the root
        {
          cout << "this was a root node... no longer" << endl;
          // this needs to be the new root
          rootNode = pointerNode;
          // set the root node to be a root node
          rootNode->typeOfNode = ROOT;
          rootNode->isRoot = true;
          // spot needs to now be set to not be a root
          spot->typeOfNode = LEAF;
          spot->isRoot = false;
          // we already have one child, namely the largeNode from before
          // add spot (aka the smallNode remade) as the smaller pointer
          // so basically, add it on the correct side of the other node
          spot->parent = rootNode;
          rootNode->children[1] = rootNode->children[0];
          rootNode->children[0] = spot; // it's smaller so just move the other one over
          rootNode->numKids = 2;
          // there is also a sibling
          spot->nextLeaf = largeNode;
          largeNode->prevLeaf = spot;
          // print the new children of the root
          cout << "small child" << endl;
          printNode(spot);
          cout << "large child" << endl;
          printNode(largeNode);
        }
        else  // if this is not the root
        {
          cout << "this wasn't the parent so I need to work my way back up" << endl;
          // this wasn't the parent, so work your way up the tree
          // find the parent of spot
          // add the pointer into the parent of spot (if possible)
          addPointerToIndex(spot->parent, pointerNode);
          // add the pointernode to the parent (or try to)
        }
      }
  }

  // add the toAdd nodepointer to p
  // after adding, check if it is legal
  // call to add a pointer to an index or root node that is not a leaf node
  void addPointerToIndex(nodePointer p, nodePointer toAdd)
  {
    std::cout << "\tnumber of keys before adding to index: " << p->numKeys << endl;
    // toAdd only has one possible key because it's a pointer, so just worry about that
    //if(toAdd->key[0]) // add the key in the correct location
    if(p->numKeys < maxKeys + 1)  // if it's possible to add a key to toAdd (it should always be possible, but this is just making sure)
      {
        // find correct location in array to insert the key
        int location = p->numKeys; // start with the last possible position in the array
        for(int i = p->numKeys - 1; i >= 0; i--)
        {
          if(p->keys[i] > toAdd->keys[0])  // find the correct location and move anything bigger backwards
          {
            location = i;
            p->keys[i+1] = p->keys[i];  // move it over by 1 so that the i spot can be overwritten
            p->children[i+1] = p->children[i];  // change the children instead of the values
          }
          else  // we've reached the smallest location 
          {  
            break;
          }
        }
        // insert into the appropriate location in the array
          p->keys[location] = toAdd->keys[0];
          cout << "added key: " << p->keys[location] << " at location: " << location << endl;
          p->children[location] = toAdd->children[0];
          p->numKeys = p->numKeys+1;
          std::cout << "\tnumber of keys: " << p->numKeys << endl;
       
        printNodeKeys(p->numKeys, p->keys);
        
      }
      else
      {
        cout << "there's something wrong with the program" << endl;
      }

      // is the new p pointer legally placed? i.e., is number of keys less than or equal to max keys
      if(p->numKeys > maxKeys)
      {
        cout << "yikes. This is a problem" << endl;
      }
  }

  // returns an index node which has a single child pointer pointing to the largeNode
  void createNewIndexNode(nodePointer largeNode, nodePointer toReturn)
  {
    //cout << "in createnewindexnode" << endl;
    //nodePointer toReturn = new nodeObject();
    
    setUpIndexNode(toReturn);
    //cout << "after setting up index node" << endl;
    largeNode->parent = toReturn; // set the parent of largeNode to be toReturn
    //cout << "set up parent of largeNode" << endl;
    toReturn->children[0] = largeNode;  // set toReturn to be the parent of largeNode
    //cout << "set up child of toReturn" << endl;
    toReturn->keys[0] = largeNode->keys[0];
    toReturn->numKeys = 1;  // this is a new node with only one key at the moment
    //return toReturn;
    //cout << "at the end" << endl;
  }

  void printNodeKeys(int numKeys, int keys[])
  {
    cout << "\tnumKeys: " << numKeys << endl;
    cout << "\t[";
    for (int i = 0; i < numKeys; i++)
    {
      cout << "---" << keys[i];
    }
    cout << "]" << endl;
  }

   void printNode(nodePointer n)
  {
    printNodeKeys(n->numKeys, n->keys);
  }

  void setUpIndexNode(nodePointer toReturn)
  {
    toReturn->typeOfNode = INDEX;
    toReturn->children = new nodePointer[maxKids];
    toReturn->keys = new int[maxNodes];
    toReturn->keyValues = new Pair[maxKeys + 1];
  }

  nodePointer setUpLeafNode(int numKeys)
  {
    nodePointer toReturn = new nodeObject();
    toReturn->typeOfNode = LEAF;
    toReturn->children = new nodePointer[maxKids];
    toReturn->numKeys = numKeys;
    toReturn->keys = new int[maxNodes];
    toReturn->keyValues = new Pair[maxKeys + 1];
    return toReturn;
  }

  nodePointer getSmallestNode(Pair kv[], int smallestLargeSpot)
  {
    nodePointer toReturn = setUpLeafNode(smallestLargeSpot);    

    for(int i = 0; i < smallestLargeSpot; i++)
    {
      toReturn->keys[i] = kv[i].key;
      toReturn->keyValues[i] = kv[i];
      //cout << toReturn->keys[i] << endl;
    }
    return toReturn;
  }

  nodePointer getLargestNode(Pair kv[], int smallestLargeSpot, int numKeys)
  {
    nodePointer toReturn = setUpLeafNode(numKeys - smallestLargeSpot);    

    int pos = 0;
    for(int i = 0; i + smallestLargeSpot< numKeys; i++)
    {
      toReturn->keys[i] = kv[i + smallestLargeSpot].key;
      //cout << "\tjust added: " << toReturn->keys[i] << " in pos " << pos << endl;;
      toReturn->keyValues[i] = kv[i + smallestLargeSpot];
      pos++;
    }
    return toReturn;
  }

  void insert(int key, float value)
  {
    nodePointer spot = findSpot(key);
    // if this is the root node
    if(spot == nullptr)  // this is the first node, so the root pointer points here
    {
      rootNode = new nodeObject();
      rootNode->typeOfNode = LEAF;  // this is the root, but it's also a leaf - leaf wins out at beginning
      //rootNode->keys[maxKeys + 1]; // we don't want to get to the +1 keys, but we need it for a 
      // short time to store the data before we can split the node
      rootNode->isRoot = true;
      rootNode->children[maxKids];
      rootNode->numKeys = 1;
      rootNode->keys = new int[maxNodes];
      rootNode->keys[0] = key;
      rootNode->keyValues = new Pair[maxKeys + 1];
      rootNode->keyValues[0] = * new Pair(key, value);
      std::cout << "root is: " << rootNode->keys[0] << endl;
      std::cout << "number of keys is: " << rootNode->numKeys << endl;
            std::cout << " " << endl;
    }
    else  // we already have a root node so add another BPTree which has a max of maxNodes and is not the root (yet)
    {
      std::cout << "root node was not null, so I am adding a node" << endl;
      // find the correct node to add this to the tree
      addPair(spot, key, value);
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

  void printBPTree()
  {
    printNode(rootNode);
  }

};




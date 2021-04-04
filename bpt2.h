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
    int typeOfNode; // ROOT, INDEX, or LEAF
    int numKeys = 0;
    nodeObject *parent = nullptr;   // holds a pointer to the parent - set to null on default
    nodeObject *prevLeaf = nullptr; // holds a pointer to the previous leaf, if this is a leaf node - otherwise we won't use this
    nodeObject *nextLeaf = nullptr; // holds a pointer to the next leaf, if this is a leaf node - otherwise we won't use it
    // we also won't know until later how many keys will be stored in here
    int *keys;           // should be empty if this is a leaf node
    Pair *keyValues;     // should be empty if this is an index node
    bool isLeaf = true;
    // we won't know until later how many children there will be, so make an array of child pointers
    nodeObject **children; // holds pointers to the children - set it to null when you can
    int numKids = 0;
  };
  typedef nodeObject *nodePointer; //define pointers to node objects - makes it easier for me later on
  nodePointer rootNode;            // pointer to the root node
  int maxNodes = 1;                //m (like m-way tree) - every node except root can have at
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
    //m (like m-way tree) - every node except root can have at
    maxNodes = m;
    // least ceil(m/2) children, and at most m children
    minKids = ceil(maxNodes / 2);
    maxKids = maxNodes;
    // there can be between ceil(m/2)-1 keys and m-1 keys unless you are the root, then your minKeys = 2
    minKeys = ceil(maxNodes / 2) - 1;
    maxKeys = maxNodes - 1;

  }

  // given a parent and key value, sets up a new node
  // nodePointer setUpNode(nodePointer p, int k)
  // {
  //   nodePointer node = new nodeObject();
  //   // usually it's a leaf
  //   // if it's a root, set it up as a root
  //   if(p == nullptr)
  //   {
  //     node->typeOfNode = ROOT;
  //   }
  //   else
  //   {
  //     node->typeOfNode = LEAF;
  //   }
  //     node->numKeys = 1;
  //     node->children = new nodePointer[maxKids+1];
  //     node->keys = new int[maxKeys+1];
  //     node->isLeaf = true;
  //     node->keyValues = new Pair[maxKeys+1];
  //   return node;
  // }

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

  BPTree()
  {
    rootNode = NULL;
  }

  // go through the array of children and sort them according to their smallest keys
  void selectionSort(nodePointer *arr, int len)
  {
    nodePointer min = nullptr;    //minimum value
    int spot = len - 1;           //the spot of the minimum value
    for (int i = 0; i < len; i++) //holds the spot where the next number would go
    {
      spot = i;
      //std::cout << "i is: " << i << "\n";
      min = arr[i];
      for (int j = i; j < len; j++) //tries to find the next min
      {
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

   // go through the array of children and sort them according to their smallest keys
  void selectionSortPair(Pair *arr, int len)
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

     // go through the array of children and sort them according to their smallest keys
  void selectionSortKey(int *arr, int len)
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

  // find the appropriate place to insert it, in this array
  void insert(int key, float value)
  {
    nodePointer spot = findSpot(key); // find the appropriate spot to place the key and value
    if(spot == nullptr)  // if the root doesn't exist yet (it's initialized to NULL when the bpt is created)
    {
      rootNode = new nodeObject();
      rootNode->keys = new int[maxKeys+1];
      rootNode->keys[0] = key;  // we only have 1 key so far
      rootNode->numKeys = 1;  // we only have 1 key so far
      rootNode->keyValues = new Pair[maxKeys+1];
      rootNode->keyValues[0] = *new Pair(key,value);
      rootNode->typeOfNode = ROOT;  // it's both a root and a leaf right now
      rootNode->isLeaf = true;  // it's both a root and a leaf right now
      rootNode->children = new nodePointer[maxKids+1];  // initialize the children array and the 
      cout << "successfully created root" << endl;
    }
    else  // the root already exists.. it might still be the rootnode though
    {
      cout << "test" << endl;
      // we already know the appropriate location to place this is called spot
      if(spot->numKeys < maxKeys) // if there are less keys there than are legal, add it and deal with the consequences later
      {
        cout << "in the if statement" << endl;
        // insert the key and re-sort
        spot->keys[spot->numKeys] = key;
        spot->keyValues[spot->numKeys] = * new Pair(key, value);
        selectionSortKey(spot->keys, spot->numKeys);
        selectionSortPair(spot->keyValues, spot->numKeys);
        spot->numKeys++;
        
      }
      else  // if there are the maximum amount of legal keys, it's more complicated
      { // it's either a leaf or the rootnode as a leaf
        
        nodePointer newLeaf = new nodeObject(); // create a new nodeobject
        int smallestLargeSpot = (maxKeys + 1)/2;
        int proxyNode[maxKeys+1]; // make a proxy that we will use to add the extra key and do the math
        for(int i = 0; i < maxKeys; i++)
        {
          // feed in all of the keys into this proxy where we will be doing the math
          proxyNode[i] = spot->keys[i];
        }
        int location = 0;
        while(key > proxyNode[location] && location < maxKeys)
        {
          location++;  // we'll start at location 1 next time
        }
        // find the place to insert it
        for(int j = maxKeys + 1; j > location; j--)  // iterate through backwards, moving everything up by 1 each time that the number is smaller
        {
          proxyNode[j] = proxyNode[j-1];  // shift it up/right by 1
        }
        // insert the key into the proxy node at whatever location was left

        proxyNode[location] = key;
        spot->numKeys = smallestLargeSpot;  // the new size of spot is the smaller amount of numbers
        newLeaf->numKeys = maxKeys + 1 - ((maxKeys+1)/2); // this is the largeNode size // DOUBLE CHECK THIS
        newLeaf->keys = new int[maxKeys+1];
        newLeaf->keyValues = new Pair[maxKeys+1];
        spot->keyValues[spot->numKeys] = * new Pair(key, value);
        selectionSortPair(spot->keyValues, spot->numKeys);
        // THERE IS SOME WEIRD CODE HERE, IDK WHAT IT DOES

        for(int i = 0; i < spot->numKeys; i++)
        {
          // fill up the keys & keyvalues with the appropriate information
          // this is a leaf, so there's nothing to worry about yet
          spot->keys[i] = proxyNode[i];
          spot->keyValues[i] = spot->keyValues[i];
        }
cout << "newLeaf numkeys: " << newLeaf->numKeys << endl;
        for(int i = 0; i < newLeaf->numKeys; i++)
        {
          //cout << "i: " << i << endl;
          newLeaf->keys[i] = proxyNode[i + spot->numKeys];
          newLeaf->keyValues[i] = spot->keyValues[i + spot->numKeys];
        }
cout << "spot" << endl;
printNode(spot);
cout << "newLeaf" << endl;
printNode(newLeaf);

        if(spot->typeOfNode == ROOT)  // check if spot is currently the root
        {
          cout << "in if statement" << endl;
          // if spot used to be the root, make spot the child of a new root node
          nodePointer newRoot = new nodeObject();
          newRoot->keys = new int[maxKeys+1];
          newRoot->keyValues = new Pair[maxKeys+1];
          newRoot->keys[0] = newLeaf->keys[0];  // the smallest in the large node
          newRoot->children = new nodePointer[maxKids+1];
          newRoot->children[0] = spot;  // make spot the smallest child
          newRoot->children[1] = newLeaf; // make newLeaf the largest child
          newRoot->typeOfNode = ROOT;
          spot->typeOfNode = LEAF;
          spot->isLeaf = true;
          spot->parent = newRoot;
          newLeaf->parent = newRoot;
          newRoot->isLeaf = false;
          newRoot->numKids = 2;
          newRoot->numKeys = 1; // you only have 1 key when it's first made 
          rootNode = newRoot;
          cout << "new root" << endl;
          printNode(newRoot);
        }
        else  // if it's not a root, fix it by moving up the tree
        {
          cout << "working on inserting an internal node" << endl;
          insertInternalNode(newLeaf->keys[0], spot->parent, newLeaf);
        }
        
        // insert the new key into the proxyNode & re-sort
        // spot->keys[spot->numKeys] = key;
        // spot->keyValues[spot->numKeys] = * new Pair(key, value);
        // selectionSortKey(spot->keys, spot->numKeys);
        // selectionSortPair(spot->keyValues, spot->numKeys);
        // spot->numKeys++;
        // int i = 0;
        // int j = 0;
        

       /*
       cout << "this is an illegal add. We need to fix it" << endl;
        // break the leaf node apart
        // determine where to split it
        int smallestLargeSpot = (maxKeys + 1)/2;
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
        spot->numKeys = smallNode->numKeys;*/
      }
      cout << "I just added " << key << " to a " << spot->typeOfNode << endl;
    }
  }

  // insert the child that is being dragged along with the nodepointer at the spot being considered
  // into the internal node above what we were at before
  void insertInternalNode(int key, nodePointer spot, nodePointer childBeingDragged)
  {
    if(spot->numKeys < maxKeys) // if we can easily add another number into this node, go ahead
    {
      int location = 0;
      while(key > spot->keys[location] && location < spot->numKeys)
      {
        location++;
      }
      for(int i = spot->numKeys; i > location; i--)
      {
        spot->keys[i] = spot->keys[i-1];  // shift it right by 1
        // this is an index node, so I don't need to worry about values
      }
      // also make some space for the new child being brought
      for(int i = spot-> numKeys+1; i > location + 1; i--)
      {
        // move everything up by 1
        spot->children[i] = spot->children[i - 1];
      }
      spot->keys[location] = key;
      spot->numKeys++;
      spot->children[location+1] = childBeingDragged;
      spot->numKids++;

    }
    else  // if this is not an easy add - it's not legal to add the pointer here
    {
      nodePointer newInternal = new nodeObject(); // make a new internal node object which will hold the larger stuff
      //insert and re-sort the children
      spot->children[spot->numKids] = childBeingDragged;
      spot->numKids++;
      selectionSort(spot->children, spot->numKids);
      newInternal->isLeaf = false;  // it's an index node
      newInternal->typeOfNode = INDEX;
      spot->numKeys = (maxKeys + 1)/2;  // small node
      newInternal->numKeys = maxKeys - (maxKeys+1)/2;
      for(int i = 0; i < newInternal->numKeys; i++)
      {
        // insert the children from spot into the new internal node
        newInternal->children[i] = spot->children[i + spot->numKeys]; 
      }
      // check if spot used to be the parent
      if(spot->typeOfNode == ROOT)
      {
        // this is the same as in the leaf function
        // if spot used to be the root, make spot the child of a new root node
          nodePointer newRoot = new nodeObject();
          newRoot->keys[0] = newInternal->keys[0];  // the smallest in the large node
          newRoot->children = new nodePointer[maxKids+1];
          newRoot->children[0] = spot;  // make spot the smallest child
          newRoot->children[1] = newInternal; // make newInternal the largest child
          newRoot->typeOfNode = ROOT;
          spot->typeOfNode = INDEX; // might already be set to index, if not set it
          newRoot->isLeaf = false;
          newRoot->numKeys = 1; // you only have 1 key when it's first made
          spot->parent = newRoot;
          newInternal->parent = newRoot; 
          rootNode = newRoot;
      }
      else  // if this was not the root node, it must have been an internal node
      {
        // insert into the next highest internal node
        insertInternalNode(spot->keys[spot->numKeys], spot->parent, newInternal);
      }
    }
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
      if(seeker->isLeaf)
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

};

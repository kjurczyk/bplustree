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
  nodePointer root;            // pointer to the root node
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
    root = nullptr;
    cout << "New " << maxKids << "-way tree was initialized" << endl;
  }

  // insert a key-value pair into a leaf node
  void insert(int key, float value)
  {
    cout << "INSERT" << endl;
    // create a new nodePointer and 
    // determine the leaf node into which the key-value pair should be inserted(key)
    nodePointer current = determineLeafNode(key);
    cout << "need to insert a new node here: " << endl;
    printNode("insert here", current);
    // if the nodePointer is currently nullptr, there is no root yet, so make one
    //  initialize new node
    //  newNode is a leaf and a root
    //  root = newNode
    printTree();
    if(current == nullptr)
    {
      //cout << "hi" << endl;
      current = initializeNode();
      cout << "just initialized a node as a root node" << endl;
      current->isLeaf = true;
      current->isRoot = true;
      root = current;
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

    printNode("after adding the key and sorting", current);

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
    //if(current->numKeys > maxKeys)
    //{
      cout << "NumKeys > maxKeys" << endl;
      nodePointer largeLeaf = initializeNode();
      largeLeaf->isLeaf = true;
      largeLeaf->isRoot = false;
      
      int smallLeafKeys = (maxKids)/2;
      cout << "maxkids: " << maxKids << endl;
      cout << "smallLeafKeys: " << smallLeafKeys << endl;
      int largeLeafKeys = maxKids - smallLeafKeys;
      cout << "LargeLeafKeys: " << largeLeafKeys << endl;
      for(int i = 0; i < largeLeafKeys; i++)
      {
        cout << i << endl;
        largeLeaf->keyValues[i] = current->keyValues[i+smallLeafKeys];
        largeLeaf->keys[i] = current->keys[i+smallLeafKeys];
        cout << "large leaf keys[" << i << "] is " << largeLeaf->keys[i] << endl;
        largeLeaf->numKeys++;
        
      }
      printNode("large", largeLeaf);
      current->numKeys = smallLeafKeys;
    //}
    
    // run the stepAfter the split(current Leaf, largeLeaf)
    stepAfterSplit(current, largeLeaf);
  }

  // nodePointer determineLeafNode(key)
  //  if root = nullptr, return nullptr
  //  otherwise:
  //  new nodePointer called seeker = root
  //  while(seeker is not a leaf node)
  //    seeker = determine which child you need to travel down(seeker, key)
  //    hope that it didn't return nullptr
  //  return the seeker because you know it is a root because it came out of the while loop
  nodePointer determineLeafNode(int key)
  {
    cout << "DETERMINE-LEAF-NODE" << endl;
    cout << "determine leaf node, key: " << key << endl;
    if(root == nullptr) 
    {
      return nullptr;
    }
    else
    {
      nodePointer seeker = root;
      printNode("seeker", seeker);
      cout << "keys of seeker" << seeker->numKeys << endl;
      while(seeker->isLeaf == false)
      {
        cout << "seeking which child to travel down" << endl;
        seeker = determineWhichChildToTravelDown(seeker, key);
        printNode("current seeker", seeker);
        cout << "seeker->isLeaf " << seeker->isLeaf << endl;
        cout << "determined which child to travel down" << endl;
      }
      cout << "found the leaf node and returning it" << endl;
      return seeker;
    }
  }

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
    void stepAfterSplit(nodePointer original, nodePointer large)
    {
      cout << "STEP-AFTER-SPLIT" << endl;
      printNode("original", original);
      printNode("largenode", large);
      nodePointer newNode;
      if(original->isRoot == true)
      {
        newNode = initializeNode();
        newNode->isRoot = true;
        newNode->isLeaf = false;
        original->isRoot = false;
        newNode->children[0] = original;
        newNode->children[1] = large;
        newNode->keys[0] = large->keys[0];
        newNode->numKeys++;
        newNode->numKids = 2;
        root = newNode;
        printNode("new Root created", root);
      }
      else  // this was definitely an index node but not a root
      {
        newNode = findParent(original);
        newNode->children[newNode->numKids] = large;
        newNode->numKids++;
        if(canPullFromLarge)
        newNode->keys[newNode->numKeys] = original->keys[original->numKeys+1];//large->keys[0]; // the smallest key from large
        newNode->numKeys++;
        sortChildren(newNode->children, newNode->numKids);
        sortKeys(newNode->keys, newNode->numKeys);
        if(newNode->numKeys <= maxKeys)  // you are almost done
        {
          printNode("newNode from stepAfterSplit", newNode);
          return;
        }
          
        else  // it's >=
        {
          cout << "too many keys... now updating parent" << endl;
          updateParent(newNode);
          printNode("updatedParent looks like this", newNode);
        }
      }
      
    }

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
  void updateParent(nodePointer toBeUpdated)
  {
    cout << "UPDATE-PARENT" << endl;
    nodePointer largeIndex = initializeNode();
    largeIndex->isLeaf = false;
    largeIndex->isRoot = false;
    int smallToReceive = (maxKeys+1)/2;
    cout << "smallToReceive " << smallToReceive << endl;
    int largeToReceive = maxKeys+1 - smallToReceive;  // made this 1 smaller
    cout << "largeToReceive " << largeToReceive << endl;
    int i = 0;
    printNode("toBeUpdated", toBeUpdated);
    for(i = 0; i <largeToReceive-1; i++)  // changed from 0 to 1 to not keep the largest number
    {
      largeIndex->keys[i] = toBeUpdated->keys[i+smallToReceive+1];  // added a 1 here
      largeIndex->numKeys++;
      largeIndex->children[i] = toBeUpdated->children[i+smallToReceive+1];  // added a 1 here
      largeIndex->numKids++;
      cout << "i: " << i << endl;
      printNode("largeIndex currently", largeIndex);
    }
    largeIndex->children[i] = toBeUpdated->children[i+smallToReceive+1];  // one more time to get all the kids
    largeIndex->numKids++;
    printNode("updateParent - largeIndex after for loop", largeIndex);

    toBeUpdated->numKeys = smallToReceive;
    toBeUpdated->numKids = smallToReceive+1;

    stepAfterSplit(toBeUpdated, largeIndex, );
  }


  // void initializeNode(node)
  //  numKeys = 0;
  //  keys = new int[maxKeys+1];
  //  keyValues = new Pair[maxKeys+1];     // should be empty if this is an index node
  //  children = new nodePointer[maxKids+1]; // holds pointers to the children - set it to null when you can
  //  int numKids = 0;
  // isLeaf and isRoot, as well as sibling pointers will be set elsewhere
  nodePointer initializeNode()
  {
    cout << "INITIALIZE-NODE" << endl;
    nodePointer node = new nodeObject();
    node->numKeys = 0;
    node->keys = new int[maxKeys+1];
    node->keyValues = new Pair[maxKeys+1];
    node->children = new nodePointer[maxKids+1];
    node->numKids = 0;
    return node;
  }

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
  nodePointer findParent(nodePointer child)
  {
    cout << "FIND-PARENT" << endl;
    nodePointer seeker = root;
    nodePointer traveller = seeker;
    
    while(seeker->isLeaf == false)
    {
      traveller = determineWhichChildToTravelDown(seeker, child->keys[0]);
      
      printNode("traveller was found to be", traveller);
      //printNode("child being looked for", child);
      if(traveller == child) 
      {
        cout << "returning seeker" << endl;
        return seeker;
      }
        
      else
      {
        cout << "need to find a new traveller" << endl;
        seeker = traveller;
      }
        
    }
    cout << "findParent could not find a parent for the child.";
    return nullptr;
  }


  // nodePointer determineWhichChildToTravelDown(parent, keyBeingSearchedFor)
  //  go through all the keys (numKeys) of the parent
  //  at each key, check if  is >, <, or = to the keyBeingSearchedFor
  //    if there is no key at that location (i >= numKeys) return the child at i (there is always 1 more child than numKeys)
  //    if the key > keyBeingSearchedFor, return the child at that key number
  //    if the key <= keyBeingSearchedFor, increase the iterator by 1
  //  we should always have returned before this, but just in case, return nullptr to indicate there was no child
  nodePointer determineWhichChildToTravelDown(nodePointer parent, int keyBeingSearchedFor)
  {
    cout << "DETERMINE-WHICH-CHILD-TO-TRAVEL-DOWN" << endl;
    int i = 0;
    printNode("parent", parent);
    for(i = 0; i < parent->numKeys; i++)
    {
      cout << "i: " << i << endl;
      if(i >= parent->numKeys)
      {
        cout << "returning the child at " << i << endl;
        return parent->children[i];
      }
      else if(parent->keys[i] > keyBeingSearchedFor)
      {
        cout << "returning the child at " << i << endl;
        return parent->children[i];
      }
        
    }
    cout << "Returning the child at " << i << endl;
    printChildren("parent", parent);
    printNode("cchild at 2", parent->children[2]);
    return parent->children[i];
  }



  // sort things
  // go through the array of children and sort them according to their smallest keys
  void sortChildren(nodePointer *arr, int len)
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
  void sortKeyValues(Pair *arr, int len)
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
  void sortKeys(int *arr, int len)
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

  void printNode(string name, nodePointer node)
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
      //cout << "btw, numkeys in " << name << " were " << node->numKeys << endl;
    }
    
  }

  // goes through the node and prints all the children on a new line
  void printChildren(string name, nodePointer node)
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

// print the tree starting at the root, for maximum of 3 levels
  void printTree()
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
            if(root->children[0]->children[2] != nullptr)
            {
              printChildren("child at 3", root->children[2]);
            }
          }
      }
      
    }
    
  }

};
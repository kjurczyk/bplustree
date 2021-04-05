#include "treeTry4.h"


// find the node that needs to be deleted from
  // delete the key and keyValue pair from the keys array and keyValues array, respectively
// if you have alreadyChecked if this is it, you are done now

  // now that you've deleted from the node, check if the node is still legal
  // a node (other than root node) should contain at least ((maxKeys+1)/2)-1 keys
  //  root node can have 1 key
  // if the node is still legal (numKeys >= minimumKeys), you are done
  // else, borrow from the nearest sibling
  //    determineChildNum
  // if you have your choice of sibling, pull from the bigger one 
  //    (check if sibling would be legal if something were removed - if not don't do this)
  //    if you pull from the bigger sibling, the parent key of that sibling needs to be updated
  //    with the now-smallest value in that sibling. The key at i-1 is the one that changes (since it definitely wasn't the smaller one)
  // else if have to pull from the smaller one
  //    check if the smaller one is legal if we removed a keyValue-pair - otherwise don't do this
  //    if you can pull without problem, no work necessary, just insert the keyValue pair into this node
  // now you are done if you did one of these things

  // now comes the interesting bit. What happens if you have no legal sibling to pull from?
  // check once again if you have a larger sibling. If you do, just delete this node, no questions asked
  //  we will call it "merging" with the sibling node
  //  there is some cleanup work with this, namely, delete the parent->keys[i] (I think?)
  //  also remove the childNum from the parent->children array
  // if instead you are pulling from an illegal smaller sibling (you have no larger sibling)
  //  "merge" with the smaller sibling by deleting your node (parent->keys[i-1]) (I think)
  //  remove the childNum from the parent->children array

  // you've done a whole lot now on the leaf level and even made some changes to the parent
  // maybe you even broke the parent. Make sure to check now
  // is the parent legal (does it have at least numKeys >= minimumKeys?)?
  //  if not, get the next largest sibling from the linked list - ask if it's legal
  //  else, get the next smallest sibling from the linked list - ask if it's legal
  //    if you were able to legally remove just a keyValue pair, 
  //      add a new node to the parent and insert this keyValue pair into that new node
  //      call insertNewNodeAtLocation(pair from sibling, parent)
  // either way, remove the sibling from its current parent->children array
  //    remember that to do this, make a new node that you call the parent of stolenNode
  //    find the parent of the tobeStolenNode
  //    find what number the child is at its current location
  //    deleteChild(parent of stolenNode, stolenNode)
  // insert the stolenNode as a child 

  // if you have no sibling to pull from, you must be the root and you are done
void BPTree::deletePair(int key, bool alreadyChecked) // if all nodes were deleted, set hasRoot to false
{
  // first step is checking that the root is not nullptr - if it is nullptr, return immediately
  if(root == nullptr)
  {
    return;
  }

  // find the node that needs to be deleted from
  nodePointer sacrificialNode = determineLeafNode(key);
  // delete the key and keyValue pair from the keys array and keyValues array, respectively
  deleteKeyAndKeyValuePair(sacrificialNode, key);
// if you have alreadyChecked if this is it, you are done now
  if(alreadyChecked)
  {
    return;
  }
  // now that you've deleted from the node, check if the node is still legal
  // a node (other than root node) should contain at least ((maxKeys+1)/2)-1 keys
  //  root node can have 1 key
  // if the node is still legal (numKeys >= minimumKeys), you are done
  if(sacrificialNode->numKeys >= minKeys)
  {
    return;
  }
  else  // else, borrow from the nearest sibling
  {
  //    determineChildNum
  //int childNum = determineChildNum();
  // if you have your choice of sibling, pull from the bigger one 
  //    (check if sibling would be legal if something were removed - if not don't do this)
  //    if you pull from the bigger sibling, the parent key of that sibling needs to be updated
  // can I pull from the bigger sibling
    if(sacrificialNode->nextLeaf != nullptr && sacrificialNode->nextLeaf->numKeys > minKeys)  // check if there is a nextLeaf and if you could steal from it
    {
      insertAsReplacement(sacrificialNode, findParent(sacrificialNode), sacrificialNode->nextLeaf, true);//, -1)
    }
    else if(sacrificialNode->prevLeaf != nullptr && sacrificialNode->prevLeaf->numKeys > minKeys) // if this is true, you could pull from here
    {
      insertAsReplacement(sacrificialNode, findParent(sacrificialNode), sacrificialNode->prevLeaf, true);
      //    with the now-smallest value in that sibling. The key at i-1 is the one that changes (since it definitely wasn't the smaller one)
      // else if have to pull from the smaller one
      //    check if the smaller one is legal if we removed a keyValue-pair - otherwise don't do this
      //    if you can pull without problem, no work necessary, just insert the keyValue pair into this node
      // now you are done if you did one of these things
    }
  
  
    // check once again if you have a larger sibling. 
    else if(sacrificialNode->nextLeaf != nullptr) // you have no legal sibling to pull from, pull from the larger sibling if it exists
    {
      deleteLeafNode(sacrificialNode);  //If you do, just delete this node, no questions asked
  //  we will call it "merging" with the sibling node
  //  there is some cleanup work with this, namely, delete the parent->keys[i] (I think?)
  //  also remove the childNum from the parent->children array
    }
  // now comes the interesting bit. What happens if you have no legal sibling to pull from?

  // if instead you are pulling from an illegal smaller sibling (you have no larger sibling)
  //  "merge" with the smaller sibling by deleting your node (parent->keys[i-1]) (I think)
  //  remove the childNum from the parent->children array

  // you've done a whole lot now on the leaf level and even made some changes to the parent
  // maybe you even broke the parent. Make sure to check now
  // is the parent legal (does it have at least numKeys >= minimumKeys?)?
  //  if not, get the next largest sibling from the linked list - ask if it's legal
  //  else, get the next smallest sibling from the linked list - ask if it's legal
  //    if you were able to legally remove just a keyValue pair, 
  //      add a new node to the parent and insert this keyValue pair into that new node
  //      call insertNewNodeAtLocation(pair from sibling, parent)
  // either way, remove the sibling from its current parent->children array
  //    remember that to do this, make a new node that you call the parent of stolenNode
  //    find the parent of the tobeStolenNode
  //    find what number the child is at its current location
  //    deleteChild(parent of stolenNode, stolenNode)
  // insert the stolenNode as a child 

  // if you have no sibling to pull from, you must be the root and you are done
  }
}

// delete the node from the tree by making the parent forget it exists
// recalculate the parent's keys array
// also remove this node from the linked list
// and check if the parent is still legal
void BPTree::deleteLeafNode(nodePointer node)
{
  nodePointer parent = findParent(node);
  bool found = false;
  for(int i = 0; i < parent->numKids-1; i++)
  {
    if(parent->children[i] == node)
    {
      found = true;
    }
    if(found)
    {
      parent->children[i] = parent->children[i+1];
    }
  }
  // we've also removed a key from the parent, so subtract 1
  parent->numKids--;
  recalculateParentKeys(parent);
}

// insertNewNodeAtLocation(Pair pair, nodePointer parent, sibling, isBigger, -1)
// you received a pair to insert into the children array of parent
// initialize a new node
// this new node is a leaf and not a root
// insertAsReplacement(new node, parent)
// previous sibling is something
// next sibling is something else
// insert the new node into the parent->children[numKids]
// parent->numKids++;
// sort the parent->children array
// return this node
/////////maybe don't need this
// pair->key goes into the new node's key[0]
// pair->numKeys++;
// pair goes into the new node's keyValues[0]

// deleteChild(nodePointer parent, int num)
//  goes through the children of the parent, ending at numKids-1
//  if i >= num, replace it with the next largest (i+1)
//  decrease numKids by 1

// insertAsReplacement(node, parent, sibling, isBigger, childNumOfSibling) // if it's the bigger sibling 
//  if you are here, you know it's legal to pull from sibling and place it into the node
//  get the smallest keyValue and key from sibling and place into
//  node at numKeys location (we've already deleted the old one and updated numKeys at this point)
//  increase nummKeys by 1
//  sort keys and keyValues with numKeys
//  if !isBigger sibling (aka it's the smaller sibling)
//    delete the largest key and keyValue pair from the sibling using numKeys-1 location
//    subtract 1 from numkeys
//  if it's the bigger sibling (isBigger)
//    delete the smallest key and keyValue pair from the sibling
//    if(childNumOfSibling != -1)
//    update the parent so that the key at childNumOfSibling-1 = bigger sibling's now smallest number
//    subtract 1 from numkeys
void BPTree::insertAsReplacement(nodePointer node, nodePointer parent, nodePointer sibling, bool isBigger)//, int childNumOfSibling)
{
  //  if you are here, you know it's legal to pull from sibling and place it into the node
  //  get the smallest keyValue and key from sibling (if the sibling is bigger) and place into
  //  node at numKeys location (we've already deleted the old one and updated numKeys at this point)
  if(isBigger)  // if the sibling is bigger than this node
  {
    node->keys[node->numKeys] = sibling->keys[0];
    node->keyValues[node->numKeys] = sibling->keyValues[0];
    node->numKeys++;

    // recalculate the keys of the parent
    recalculateParentKeys(parent);

    //    delete the smallest key and keyValue pair from the sibling & subtract 1 from numkeys
    deleteKeyAndKeyValuePair(sibling, sibling->keys[0]);
    //    if(childNumOfSibling != -1)
    //if(childNumOfSibling != -1)
    //{
      //    update the parent so that the key at childNumOfSibling-1 = bigger sibling's now smallest number
      //updateLargerLender(findParent(sibling), childNumOfSibling);
      recalculateParentKeys(findParent(sibling));
    //}
    
  }
  
  else if(!isBigger) // if you are borrowing from a smaller sibling, if !isBigger sibling (aka it's the smaller sibling)
  {
    node->keys[node->numKeys] = sibling->keys[sibling->numKeys-1];
    node->keyValues[node->numKeys] = sibling->keyValues[sibling->numKeys-1];
    node->numKeys++;//  increase nummKeys by 1
    // sort these because you are placing a smaller number at the end
    sortKeys(node->keys, node->numKeys);//  sort keys and keyValues with numKeys
    sortKeyValues(node->keyValues, node->numKeys);

    // recalculate the keys of the parent
    recalculateParentKeys(parent);
    
    // delete the largest key and keyValue pair from the sibling using numKeys-1 location & subtract 1 from numkeys
    node->numKeys--;  // can just do this and we will next time overwrite this number
    //updateSmallerLender(findParent(sibling), childNumOfSibling);
    recalculateParentKeys(findParent(sibling));
  }
}

void BPTree::recalculateParentKeys(nodePointer parent)
{
  // iterate through your keys and replace the key at i+1
  for(int i = 0; i < parent->numKeys; i++)
  {
    parent->keys[i] = parent->children[i+1]->keys[0];
  }
}

/*
// update the parent so that the key at childNumOfSibling-1 = bigger sibling's now smallest number
void BPTree::updateLargerLender(nodePointer parent, int childNumOfSibling)
{
  // if the childNumOfSibling was 0, we don't care about the key anyways, so we can just return
  if(childNumOfSibling == 0)
  {
    return;
  }
  else
  {
    parent->keys[childNumOfSibling-1] = parent->children[childNumOfSibling]->keys[0];
  }
}

// delete the largest key and keyValue pair from the sibling using numKeys-1 location & subtract 1 from numkeys
void BPTree::updateSmallerLender(nodePointer parent, int childNumOfSibling)
{
  // if the childNumOfSibling was 0, we don't care about the key, so we can just return
  if(childNumOfSibling == 0)
  {
    return;
  }
  else
  {
    parent->keys[childNumOfSibling-1] = parent->children[childNumOfSibling]->keys[0];
  }
}
*/

// returns true if it was able to delete, false if it was unable to delete - should never be false for this assignment
// bool deleteKeyAndKeyValue(node, key)
// in the same loop, do the same for the array of Pairs
// return false if deleteKeyValuePair returned false (we don't care so much about just the key)
bool BPTree::deleteKeyAndKeyValuePair(nodePointer toBeDeletedFrom, int key)  // toBeDeletedFrom is a leaf
{
  if(deleteKey(toBeDeletedFrom, key) && deleteKeyValuePair(toBeDeletedFrom, key, toBeDeletedFrom->numKeys+1)) // needs to be +1 because numKeys is subtracted by 1 in the deleteKey operation
  {
    return true;
  }
  else
  {
    return false;
  }
}

// returns true if it was able to delete, false if it was unable to delete - should never be false for this assignment
// bool deleteKey(node, key)
// find the key in the keys array and shuffle any nodes that were to the right of it onto it
// decrease the numKeys by 1
bool BPTree::deleteKey(nodePointer toBeDeletedFrom, int key)
{
  bool found = false;
  for(int i = 0; i < toBeDeletedFrom->numKeys-1; i++)
  {
    if(toBeDeletedFrom->keys[i]==key) // if you find the key you are looking for
    {
      found = true;
    }
    if(found) // if you've already come accross the key you are looking for, over-write the number at i with i+1
    {
      toBeDeletedFrom->keys[i] = toBeDeletedFrom->keys[i+1];
    }
  }
  // if you get through everything and you still haven't found it, maybe it was the last number?
  if(!found)
  {
    if(toBeDeletedFrom->keys[toBeDeletedFrom->numKeys-1] == key)
    {
      found = true;
    }
  }
  if(found) // if you found it, you did the hard part of removing the extra key
  {
    toBeDeletedFrom->numKeys--;
    return true;
  }
  else
  {
    return false;
  }
}

// returns true if it was able to delete, false if it was unable to delete - should never be false for this assignment
// bool deleteKeyValuePair (node, key)
// find the pair key in the keyValues array and shuffle any nodes that were to the right of it onto it
// return false if it couldn't find it
bool BPTree::deleteKeyValuePair(nodePointer toBeDeletedFrom, int key, int numKeyValues)
{
  bool found = false;
  for(int i = 0; i < toBeDeletedFrom->numKeys-1; i++)
  {
    if(toBeDeletedFrom->keyValues[i].key==key) // if you find the key you are looking for
    {
      found = true;
    }
    if(found) // if you've already come accross the key you are looking for, over-write the number at i with i+1
    {
      toBeDeletedFrom->keyValues[i] = toBeDeletedFrom->keyValues[i+1];
    }
  }
  // if you get through everything and you still haven't found it, maybe it was the last number?
  if(!found)
  {
    if(toBeDeletedFrom->keyValues[numKeyValues-1].key == key)
    {
      found = true;
    }
  }
  return found;
}

// nodePointer getSibling
//  find the parent and save its location
//  determine what your child number is in relation to the parent by calling determineChildNum
//  is there a legal larger sibling? (the sibling has more than min nodes)
//    if yes, pull the smallest key from there and update the key in the parent by removing the key
//    return the sibling

// returns the number of the child in the parent children array
// int determineChildNum(parent, child)
//  for(int i = 0; i < parent->numKeys; i++)
//    if the parent->children[i] = the child, you have found
//      the correct number - return i
int BPTree::determineChildNum(nodePointer parent, nodePointer child)
{
  for(int i = 0; i < parent->numKeys; i++)
  {
    if(parent->children[i] == child)
    {
      return i;
    }
  }
  return -1;  // if the child was not found, return -1
}
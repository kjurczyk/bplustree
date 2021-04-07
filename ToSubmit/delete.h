#include "tree.h"


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
  // if all nodes were deleted, set hasRoot to false

///////////////////////////////////////////////////////////////////////////////////
// deletes a pair from a node and is responsible for calling all other functions pertaining to delete
///////////////////////////////////////////////////////////////////////////////////
void BPTree::deletePair(int key) // deletes a pair from a node and is responsible for calling all other functions pertaining to delete
{
  if(DEBUG) cout << "---------------------------------------------------------" << endl;
  
  // first step is checking that the root is not nullptr - if it is nullptr, return immediately
  if(root == nullptr)
  {
    return;
  }

  // find the node that needs to be deleted from
  nodePointer sacrificialNode = determineLeafNode(key);
  // delete the key and keyValue pair from the keys array and keyValues array, respectively
  if(DEBUG) printNodePairs("sacrificial node pairs", sacrificialNode);
  deleteKeyAndKeyValuePair(sacrificialNode, key);
  
// if you have alreadyChecked if this is it, you are done now
  // if(alreadyChecked)
  // {
  //   return;
  // }
  // now that you've deleted from the node, check if the node is still legal
  // a node (other than root node) should contain at least ((maxKeys+1)/2)-1 keys
  //  root node can have 1 key
  // if the node is still legal (numKeys >= minimumKeys), you are done
  if(sacrificialNode->numKeys >= minKeys)
  {
    if(DEBUG) cout << "numKeys: " << sacrificialNode->numKeys << " and minKeys: " << minKeys << endl;
    if(DEBUG) cout << "sacrificialNode->numKeys >= minKeys so I am done" << endl;
    return;
  }
  else  // else, borrow from the nearest sibling
  {
    //    determineChildNum
    //int childNum = determineChildNum();
    nodePointer parent = findParent(sacrificialNode);
    // if you have your choice of sibling, pull from the bigger one 
    //    (check if sibling would be legal if something were removed - if not don't do this)
    //    if you pull from the bigger sibling, the parent key of that sibling needs to be updated
    // can I pull from the bigger sibling
    if(sacrificialNode->nextLeaf != nullptr && sacrificialNode->nextLeaf->numKeys > minKeys)  // check if there is a nextLeaf and if you could steal from it
    {
      if(DEBUG) cout << "I found a larger leaf to replace mine." << endl;
      insertAsReplacement(sacrificialNode, parent, sacrificialNode->nextLeaf, true);//, -1)
    }
    else if(sacrificialNode->prevLeaf != nullptr && sacrificialNode->prevLeaf->numKeys > minKeys) // if this is true, you could pull from here
    {
      if(DEBUG) cout << "I found a smaller leaf to replace mine." << endl;
      insertAsReplacement(sacrificialNode, parent, sacrificialNode->prevLeaf, false);
      //    with the now-smallest value in that sibling. The key at i-1 is the one that changes (since it definitely wasn't the smaller one)
      // else if have to pull from the smaller one
      //    check if the smaller one is legal if we removed a keyValue-pair - otherwise don't do this
      //    if you can pull without problem, no work necessary, just insert the keyValue pair into this node
      // now you are done if you did one of these things
    }
    
    // now comes the interesting bit. What happens if you have no legal sibling to pull from?
    // check once again if you have a larger sibling. 
    else if(sacrificialNode->nextLeaf != nullptr) // you have no legal sibling to pull from, pull from the larger sibling if it exists
    {
      // "merging" with the sibling node
      //  there is some cleanup work with this, namely, delete the parent->keys[i]
      //  also remove the childNum from the parent->children array
      nodePointer parentOfSibling = findParent(sacrificialNode->nextLeaf);
      if(DEBUG) cout << "I found a larger leaf to replace mine - it's not strictly legal but oh well." << endl;
      //deleteLeafNode(sacrificialNode, parent);  //If you do, just delete this node, no questions asked
      mergeWithSibling(sacrificialNode, parent, sacrificialNode->nextLeaf, parentOfSibling);
      if(DEBUG) cout << "after merging with larger sibling1" << endl;
      return;
    }
    else if(sacrificialNode->prevLeaf != nullptr) // if instead you are pulling from an illegal smaller sibling (you have no larger sibling)
    {
      nodePointer parentOfSibling = findParent(sacrificialNode->prevLeaf);
      //  "merge" with the smaller sibling by deleting your node (parent->keys[i-1])
      //  remove the childNum from the parent->children array
      if(DEBUG) cout << "I found a smaller leaf to replace mine - it's not strictly legal but oh well." << endl;
      //deleteLeafNode(sacrificialNode, parent);
      mergeWithSibling(sacrificialNode, parent, sacrificialNode->prevLeaf, parentOfSibling);
      if(DEBUG) cout << "after merging with smaller sibling2" << endl;
      return;
    }
    else  // there should be no way to get here, but if you end up here, assume you made it here in error and return
    {
      if(DEBUG) cout << "You should not have reached this else statement in the delete function." << endl;
      return; // you should never get here because there should always be at least 1 sibling and
      // now we have checked all the siblings - something should have previously existed
    }
    
    if(DEBUG) cout << "made it out of the if else jungle" << endl;
    // if there is a grandparent
    // make sure that the grandparent has the most up-to-date info on its keys
    if(!parent->isRoot) // if the parent is not the root, there must be a parent of it. Update it
    {
      if(DEBUG) cout << "updating grandparent" << endl;
      if(DEBUG) printNode("parent is not the root. parent for reference", parent);
      if(DEBUG) printTree();
      if(DEBUG) printNode("grandparent should be", findParent(parent));
      updateGrandparent(findParent(parent));
      if(DEBUG) cout << "finisehd updating grandparent" << endl;
    }
  
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// merge with the sibling of the node, meaning that you pull the info from the sibling into the node
// after that, you delete the location of the sibling in the parent 
// sibling will be deleted, so pull everything into node
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BPTree::mergeWithSibling(nodePointer node, nodePointer parent, nodePointer sibling, nodePointer parentOfSibling) // merge a sibling into a node, and make sure that both parents are ok
{
  if(DEBUG) printNode("node being merged", node);

  if(node->isLeaf)  // if the given node is a leaf
  {
    // if these are leaf nodes you are dealing with, take the keys and keyValue Pairs of the sibling into yourself
    for(int i = 0; i < sibling->numKeys; i++)
    {
      node->keys[i+node->numKeys] = sibling->keys[i];
      node->keyValues[i+node->numKeys] = sibling->keyValues[i];
      node->numKeys++;
      sibling->numKeys--;
    }
    // recalculate the order of the keys and key values in the node
    sortKeys(node->keys, node->numKeys);
    sortKeyValues(node->keyValues, node->numKeys);
    if(DEBUG) printNode("after sucking the sibling dry, this node", node);
    if(DEBUG) printNode("and the sucked sibling is", sibling);
  }
  else  // otherwise the node is an index node and has children
  {
    // take the children. Don't take the keys quite yet, since you will be figuring out what keys you want after you
    // know how many children you have
    if(DEBUG) cout << "current node has " << node->numKids << " kids" << endl;
    if(DEBUG) printNode("cchild at 0", node->children[0]);
    int originalNumKids = sibling->numKids; // necessary because numKids changes during the function
    for(int i = 0; i < originalNumKids; i++)  // iterate through all the children
    {
      if(DEBUG) cout << "i+numkids: " << i << endl;
      node->children[node->numKids] = sibling->children[i];
      if(DEBUG) printNode("just added as a child", node->children[node->numKids]);
      node->numKids++;
      sibling->numKids--;
    }
    if(DEBUG) printChildren("after adding", node);
    if(DEBUG) cout << "after for loop" << endl;
    if(DEBUG) cout << "current node has " << node->numKids << " kids" << endl;

    // sort the kids
    sortChildren(node->children, node->numKids);
    if(DEBUG) printChildren("after sorting", node);
    if(DEBUG) cout << "after sorting children" << endl;

    // update how many keys the node has
    node->numKeys = node->numKids-1;
    sibling->numKeys = sibling->numKids-1;

    // update the keys that the node has
    recalculateParentKeys(node);
    if(DEBUG) cout << "after recalcuating node" << endl;
    if(DEBUG) printNode("recalculated node", node);
  }


  // recalculate keys belonging to the parent now
  recalculateParentKeys(parent);
  if(DEBUG) printNode("parent after recalculation",parent);
  if(DEBUG) cout << "after recalcuating parent2" << endl;
  if(DEBUG) printTree();
  if(DEBUG) printNode("sibling for reference", sibling);
  if(DEBUG) printNode("parent of sibling", parentOfSibling);
  if(DEBUG) cout << "after print node" << endl;

  // check if your sibling's parent is a nullptr. If it is, you must be in the root right now.
  // make the current node the root and set it as such
  // you are done!
  if(parentOfSibling == nullptr)
  {
    // skip
    if(DEBUG) cout << "nullptr" << endl;
    root = node;
    root->isRoot = true;
    if(DEBUG) printTree();
    if(DEBUG) cout << "I reset the root to the above tree. Now returning." << endl;
    return;
  }
  else if(parentOfSibling->numKeys > 0) // if the parent of the sibling exists, update the keys to be in order with the correct numbers
  {
    if(DEBUG) cout << "in the if statement " << endl;
    if(DEBUG) cout << "numKeys " << parentOfSibling->numKeys << endl;
    recalculateParentKeys(parentOfSibling);
    if(DEBUG) printNode("recalculated parentOfSibling", parentOfSibling);
  }
 
  // debug statments
  if(DEBUG) printTree();
  if(DEBUG) printNode("sibling for reference", sibling);
  if(DEBUG) printNode("parent of sibling", parentOfSibling);
  if(DEBUG) cout << "after recalcuating parents3" << endl;

  // now that you've taken all the info out of the sibling and placed it into the node, effectively merging them
  // delete the sibling from its parent by making the parent of the sibling forget it exists
  if(parentOfSibling != nullptr)
  {
    // delete the sibling from its parent (make its parent forget it exists)
    deleteNode(sibling, parentOfSibling);
    if(DEBUG) cout << "successfully removed the blank sibling from the parent" << endl;

    // is the parent of the sibling ok? if the parent of the sibling has too few keys now, check if the root needs to be updated (it's a grandparent for sure if you've reached here)
    if(parentOfSibling->numKeys >= minKeys)
    {
      if(DEBUG) cout << "finished with merging the sibling. Going to do one more thing." << endl;
      updateGrandparent(root);
      return; //    ADDED THIS HERE
    }
    if(parentOfSibling->isRoot) // check if your parent is the root. IF so, make this node the new root
    {
      // delete the root and make the sibling the new root
      root = node;
      node->isRoot = true;
      if(DEBUG) printTree();
      if(DEBUG) cout << "I reset the root to the above tree. Now returning." << endl;
      return;
    }
    else if(parentOfSibling->numKeys >= minKeys)  // probably unnecessary, but it is a catch statement just in case for all the other legal parents of siblings that made it to this point
    {
      if(DEBUG) cout << "the parent had an ok number of keys, so I am returning" << endl;
      return;
    }
    else  // if the parent of the sibling has an issue for some reason, call this function again to fix it
    {
      if(DEBUG) cout << "the parent of the sibling has too few keys. We need to see if there is a sibling of it we can pull from" << endl;
      nodePointer siblingV2 = getLegalIndexSibling(parentOfSibling);  // find the best sibling sharing a parent
      // merge the sibling with this node - assume that you can't just take a key from another one, because you 
      // would need to take the child to go with it, and you already know which children you have because
      // to reach here you are an index node
      if(DEBUG) printNode("siblingV2", siblingV2);
      mergeWithSibling(parentOfSibling, findParent(parentOfSibling), siblingV2, findParent(siblingV2));
    }
  }
  
  
  

}

/////////////////////////////////////////////////////////////////////////////
// updates the grandparent's keys to look at the 
// update the keys of the grandparent to make sure they are alright
// only call this function if you are sure the node is a grandparent (aka it has at least one child that has at least 1 child)
/////////////////////////////////////////////////////////////////////////////
void BPTree::updateGrandparent(nodePointer node) // update the keys of the grandparent to make sure they are alright
{
  if(DEBUG) printNode("grandparent being updated", node);
  //nodePointer childAtIPlus1;
  if(node == nullptr)
  {
    // just update the root and call it done
    for(int i = 0 ; i < root->numKeys; i++)
    {
      if(DEBUG) cout << "i: " << i << endl;
      // look at the child at i and update the appropriate key
      root->keys[i] = root->children[i+1]->children[0]->keys[0];  // I know this exists because this one is a grandparent
    }
  }
  else
  {
    for(int i = 0 ; i < node->numKeys; i++)
    {
      if(DEBUG) cout << "i: " << i << endl;
      // look at the child at i and update the appropriate key
      node->keys[i] = node->children[i+1]->children[0]->keys[0];  // I know this exists because this one is a grandparent
    }
  }
  
}

/////////////////////////////////////////////////////////////////////////
// checks if the parent is legal and if not, redoes the merge in an upwards direction
/////////////////////////////////////////////////////////////////////////
void BPTree::percolateUpwards(nodePointer parent) // checks if the parent is legal and if not, redoes the merge
{
  if(DEBUG) printNode("parent", parent);
  if(parent == root)  // the root has different rules on how many keys are allowed in it
  {
    return;
  }
  // the parent node is empty, or close to it!
  // merge with the sibling (it's the sibling of the parent this time)
  nodePointer siblingOfParent = getLegalIndexSibling(parent);
  if(DEBUG) printNode("siblingOfParent",siblingOfParent);
  // pull the parent's parent key into the siblingOfParent
  nodePointer parentOfParent = findParent(parent);
  insertParentKeysIntoIndexNode(parentOfParent, siblingOfParent); // the parent of the parent is sucked dry
  
  insertChildrenIntoIndexNode(parent, siblingOfParent); // the parent is sucked dry
  // is the parent of the parent deficient now? Yes
  if(parentOfParent->isRoot == true)  // if this is the root, just delete the root and make the siblingOfParent the new root
  {
    parentOfParent->isRoot = false;
    root = siblingOfParent;
    root->isRoot = true;
    // now you are done!
    return;
  }
  else
  {
    percolateUpwards(parentOfParent);
    // if this wasn't the root, redo all of this by percolating upwards
  }
}

/////////////////////////////////////////////////////////////////////////
// insert the children of a given node into another index node
/////////////////////////////////////////////////////////////////////////
void BPTree::insertChildrenIntoIndexNode(nodePointer node, nodePointer receiver) // insert children from a given node into the receiving node which has too few children
{
  for(int i = 0; i < node->numKids; i++)
  {
    receiver->children[receiver->numKids+i] = node->children[i];
    receiver->numKids++;
  }
  node->numKids = 0;
  sortChildren(receiver->children, receiver->numKids);
}

/////////////////////////////////////////////////////////////////////////
// insert the parents keys into the sibling's keys array
/////////////////////////////////////////////////////////////////////////
void BPTree::insertParentKeysIntoIndexNode(nodePointer parent, nodePointer receiver) // pull the parent's keys down into the index node given
{
  for(int i = 0; i < parent->numKeys; i++)  // go through all the keys in the parent and update the key to be correct
  {
    receiver->keys[receiver->numKeys+i] = parent->keys[i];
    receiver->numKeys++;
  }
  parent->numKids = 0;
  sortKeys(receiver->keys, receiver->numKeys);  // sort the keys from lowest to highest
}

/////////////////////////////////////////////////////////////////////////
// getLegalIndexSibling(index)
// returns the larger sibling of the index node if it exists. If that doesn't exist, returns
// the smaller sibling (one of those 2 must exist though, so there is no third case)
// nodePointer getSibling
//  find the parent and save its location
//  determine what your child number is in relation to the parent by calling determineChildNum
//  is there a legal larger sibling? (the sibling has more than min nodes)
//    if yes, pull the smallest key from there and update the key in the parent by removing the key
//    return the sibling
/////////////////////////////////////////////////////////////////////////
BPTree::nodePointer BPTree::getLegalIndexSibling(nodePointer index) // returns a nodePointer to the best sibling to borrow from (built for index nodes because leaf nodes have sibling pointers)
{
  if(DEBUG) cout << "getLegalIndexSibling" << endl;
  nodePointer parent = findParent(index); // find the parent of the node given
  if(DEBUG) printNode("parent",parent);
  nodePointer bestSibling = nullptr;  // start by setting the best sibling to nullptr
  if(DEBUG) printNode("bestSibling", bestSibling);
  if(DEBUG) cout << "parent->numKeys: " << parent->numKeys << endl;
  for(int i = 0; i < parent->numKeys; i++) // you can go from numKeys because there are numKeys children (aka numKids-1)
  {
 
    // if the current bestSibling is nullptr, make bestSibling the first child you see
    if(parent->children[i] != index && bestSibling == nullptr)
    {
      bestSibling = parent->children[i];
      if(DEBUG) printNode("found first bestSibling", bestSibling);
    }
    // whenever the current child we are looking at is smaller than the index, make the current child the bestSibling
    // because it's an improvement
    if(parent->children[i] == index)
    {
      // when the index is = the child we are looking at, check if there is a child that exists after that
      // grab it if the child exists, otherwise, don't do anything
      // don't bother continuing with the iteration if you reached through the index already
      bestSibling = parent->children[i+1];
      if(DEBUG) printNode("returning bestSibling", bestSibling);
      return bestSibling;
    }
    else  // it must be smaller, because as soon as it's equal we drop out
    {
      bestSibling = parent->children[i];
    }
  }
  if(DEBUG) printNode("returning bestSibling1", bestSibling);
  return bestSibling;
}

/////////////////////////////////////////////////////////////////////////
// delete the node from the tree by making the parent forget it exists
// you are deleting an entire node from the parent
// recalculate the parent's keys array
// also remove this node from the linked list
// and check if the parent is still legal
// can delete a leaf or an index node, it'll figure it out either way
/////////////////////////////////////////////////////////////////////////
void BPTree::deleteNode(nodePointer node, nodePointer parent)   // deletes a node from its parent (over-writes that child pointer with another to make the parent forget it existed)
{
  if(DEBUG) printNode("node being deleted", node);
  if(DEBUG) printNode("parent of node being deleted", parent);
  //nodePointer parent = findParent(node);
  
  bool found = false;
  for(int i = 0; i < parent->numKids-1; i++)
  {
    if(DEBUG) cout << "i: " << i << endl; 
    //printNode("parent->children[i]", parent->children[i]);
    if(parent->children[i] == node)
    {
      if(DEBUG) cout << "found" << endl;
      found = true;
    }
    if(found)
    {
      if(DEBUG) printNode("sibling", parent->children[i+1]);
      parent->children[i] = parent->children[i+1];  // over-write the child to be erased with the bigger children
    }
  }
  if(DEBUG) cout << "end of for loop" << endl;
  // we've also removed a key from the parent, so subtract 1
  parent->numKids--;
  // the number of keys will also decrease by 1
  parent->numKeys = parent->numKids - 1;
  recalculateParentKeys(parent);
  if(DEBUG) printNode("parent looks like this after recalculation*", parent);
  if(DEBUG) printChildren("parent", parent);
  if(DEBUG)cout << "finished recalculating parent" << endl;

  // remove the node from the linked list
  // need to link its prev->next and next->prev
  // only do this extra step if the original node being deleted was a leaf node
  if(node->isLeaf)
  {
    if(node->nextLeaf != nullptr )
    {
      if(node->prevLeaf != nullptr)
      {
        node->nextLeaf->prevLeaf = node->prevLeaf;
        node->prevLeaf->nextLeaf = node->nextLeaf;
      }
      else  // else the node at the prev leaf is null, so make nexleaf's prevleaf = null
      {
        node->nextLeaf->prevLeaf = nullptr;
      }
    }
    else if(node->prevLeaf != nullptr)  // we only get here if the next leaf is null
    {
      node->prevLeaf->nextLeaf = nullptr;
    }
  }
  
  if(DEBUG) cout << "got to the end of the linked list" << endl;
  // going back to the parent, is the parent still legal? That isn't under the leaf node's jurisdiction
}

//////////////////////////////////////////////////////////////////////
// find youngest shared elder
// given two nodes that are on the same level
//////////////////////////////////////////////////////////////////////
BPTree::nodePointer BPTree::sharedElder(nodePointer a, nodePointer b) // finds the youngest shared elder of a and b (e.g. if a and b are cousins, a pointer to the grandparent would be returned)
{
  nodePointer aParent = findParent(a);  // find the parent of the first node
  nodePointer bParent = findParent(b);  // find the parent of the second node
  if(aParent == nullptr || bParent == nullptr)  // if either one is null, return the root (we must be near the top)
  {
    return root;
  }
  if(aParent == bParent)  // return the parent of one of them, if they have the same parent
  {
    return aParent;
  }
  else  // percolate your way upwards
  {
    return sharedElder(aParent, bParent);
  }
}



//////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////
void BPTree::insertAsReplacement(nodePointer node, nodePointer parent, nodePointer sibling, bool isBigger)  // replaces keys/values in one node with keys/values with another node
{
  //  if you are here, you know it's legal to pull from sibling and place it into the node
  //  get the smallest keyValue and key from sibling (if the sibling is bigger) and place into
  //  node at numKeys location (we've already deleted the old one and updated numKeys at this point)
  if(isBigger)  // if the sibling is bigger than this node
  {
    node->keys[node->numKeys] = sibling->keys[0];
    node->keyValues[node->numKeys] = sibling->keyValues[0];
    node->numKeys++;
    if(DEBUG) cout << "successfully changed the node's numKeys to " << node->numKeys << endl;

    // recalculate the keys of the parent
    recalculateParentKeys(parent);

    //    delete the smallest key and keyValue pair from the sibling & subtract 1 from numkeys
    deleteKeyAndKeyValuePair(sibling, sibling->keys[0]);
  
    //update the parent so that the key at childNumOfSibling-1 = bigger sibling's now smallest number
    //updateLargerLender(findParent(sibling), childNumOfSibling);
    recalculateParentKeys(findParent(sibling));
    if(DEBUG) cout << "recalculated stuff" << endl; 
    
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

/////////////////////////////////////////////////////////////////
// recalculates the keys in the given node using the array of children
/////////////////////////////////////////////////////////////////
void BPTree::recalculateParentKeys(nodePointer parent) // recalculates the keys in the given node using the array of children
{
  // iterate through your keys and replace the key at i+1
  if(DEBUG) cout << "numKids-1: " << parent->numKids-1 << endl;
  for(int i = 0; i < parent->numKids-1; i++)
  {
    if(DEBUG) cout << "i: " << i << endl;
    parent->keys[i] = parent->children[i+1]->keys[0];
  }
 
}

/////////////////////////////////////////////////////////////////
// returns true if it was able to delete, false if it was unable to delete - should never be false for this assignment
// bool deleteKeyAndKeyValue(node, key)
// in the same loop, do the same for the array of Pairs
// return false if deleteKeyValuePair returned false (we don't care so much about just the key)
/////////////////////////////////////////////////////////////////
bool BPTree::deleteKeyAndKeyValuePair(nodePointer toBeDeletedFrom, int key)  // toBeDeletedFrom is a leaf - deletes a key and keyValue pair from a leaf node
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

/////////////////////////////////////////////////////////////////
// returns true if it was able to delete, false if it was unable to delete - should never be false for this assignment
// bool deleteKey(node, key)
// find the key in the keys array and shuffle any nodes that were to the right of it onto it
// decrease the numKeys by 1
/////////////////////////////////////////////////////////////////
bool BPTree::deleteKey(nodePointer toBeDeletedFrom, int key) // deletes the key from a node & decreases numKey
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

/////////////////////////////////////////////////////////////////
// returns true if it was able to delete, false if it was unable to delete - should never be false for this assignment
// bool deleteKeyValuePair (node, key)
// find the pair key in the keyValues array and shuffle any nodes that were to the right of it onto it
// return false if it couldn't find it
/////////////////////////////////////////////////////////////////
bool BPTree::deleteKeyValuePair(nodePointer toBeDeletedFrom, int key, int numKeyValues) // deletes a key and keyValue pair from a leaf node given a specific number of key values
{
  bool found = false;
  for(int i = 0; i < toBeDeletedFrom->numKeys; i++)
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

/////////////////////////////////////////////////////////////////
// returns the number of the child in the parent children array
// int determineChildNum(parent, child)
//  for(int i = 0; i < parent->numKeys; i++)
//    if the parent->children[i] = the child, you have found
//      the correct number - return i
/////////////////////////////////////////////////////////////////
int BPTree::determineChildNum(nodePointer parent, nodePointer child) // determines the number of the child in the parent's children array
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
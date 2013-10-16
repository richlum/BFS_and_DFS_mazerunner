/*
 * StackNode is a container for a pair of pointers, one to the item of interest, one to the next
 * StackNode.   It is used by Stack Class to implement a linked list for use as a Stack.
 * Stack class has the methods to maintain and use the stack.
 */

#include "StackNode.h"
using namespace std;


StackNode::StackNode(MazeNode* item, StackNode* nextNode ){
	theItem = item;
	next = nextNode;
}


StackNode::~StackNode(){};


MazeNode* StackNode::getItem(){
	return theItem;
}

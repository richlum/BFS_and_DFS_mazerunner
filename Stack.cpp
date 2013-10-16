/*
 * Implements Stack methods for implementation a stack as a link list of 
 * StackNodes.  The StackNode is a container object with a pair of pointers
 * one to the item of interest and one to the next stacknode.
 *
 * core implementation of push pop and size methods for stack behaviour
 *
 */


#include "Stack.h"
#include "StackNode.h"
#include <iostream>

#include <cstdio>  // for using NULL
using namespace std;

Stack::Stack(){
	qty = 0;
	head = NULL;
}

Stack::~Stack(){
	while (qty>0){
		pop();
		qty--;
	}
}

int Stack::size(){
	return qty;
}

/*
 * implementation of stack push method to load MazeNode* into stack.
 */
void Stack::push(MazeNode* item){
#ifdef DEBUG
	currNode->print(cerr);
	cerr << "\t dbg\n";
#endif
	// place new item at head of stack and have it point to the old head of stack
	head = new StackNode(item, head);
	qty++; //for size reporting
	Stack::pushCount++;  // for statistic gathering
	if (qty > maxSize){
		Stack::maxSize = qty;
	}
}

/*
 * implementation of stack pop method to load MazeNode into stack.
 * @return MazeNode* that was on top of the stack and removes it from the stack.
 */

MazeNode* Stack::pop(){
	StackNode* top = head;
	if (top == NULL){
		cout << "attempt to pop empty stack" << endl;
		return NULL;
	}

	MazeNode* result = top->getItem();
	head = head->next;  // set head to the second on stack
	qty--;
	Stack::popCount++;
	delete top;
	return result;
}

/*
 * utility to see current stack contents for debugging. 
 */
void Stack::dumpStack(){
	StackNode* top = head;
	cerr << "STACK CONTAINS { ";
	while (top != NULL){
		MazeNode* aNode = top->getItem();
		aNode->print(cerr);
		cerr << " ";
		top=top->next;
	}
	cerr << "}  end\n";
}
/*
 * show the stats on the stack that we have collected.
 * push/pop count are representative of number of nodes explored over time.
 * maxsize is an reflection of the maximum number of nodes held in stack
 * at any given point in time.
 */
void Stack::showStats(){
	cout << "------Stack Usage Statistics ---------" << endl;
	cout << "\tpushCount = " <<  Stack::pushCount <<endl;
	cout << "\tpopCount = " <<  Stack::popCount <<endl;
	cout << "\tmaxSize = " <<  Stack::maxSize <<endl;
	cout << "------Stack Usage Statistics ---------" << endl;
}
int Stack::pushCount = 0;
int Stack::popCount = 0;
int Stack::maxSize = 0;

/*
 * CircularArray.cpp
 *
 *  Created on: 2011-01-13
 *      Author: rlum
 *
 *  A circular array largely based on sample code provided in CS221 classes.
 *  Implemented as an array of pointers to MazeNodes.
 *  Includes dynamic resizing when full queue is detected by creating a new
 *  array double the current size, then copying elements from start to new 
 *  array start to end.
 *
 *
 */

#include "CircularArray.h"
#include <iostream>
using namespace std;


/*
 * Syntax notes to self:
 * foo *fooArray[100]  // 
 *
 * foo **fooptr // pointer to pointer to foo
 * fooptr = new foo*[100]  // array of 100 pointers to foo
 */
CircularArray::CircularArray() {
	arraysize = initialSize;
	this->Q =  new MazeNode* [arraysize];
	front = 0;
	back = 0;


}

CircularArray::~CircularArray() {
	while (!is_empty()){
		MazeNode* temp = deque();
		delete temp;
	}  // delete any mazenodes that we have on hand.
	delete Q;
}
/**
 * enqueue places a MazeNode into the circular array queue by
 * placing new nodes at the back of the queue.
 *
 */
void CircularArray::enqueue(MazeNode* item){
	if (is_full()){
		expandQ();
	}
	Q[back] = item;
	back = (back+1)%arraysize;
	// track some perfomance statistics
	pushCount++;
	if ((CircularArray::pushCount-CircularArray::popCount)> CircularArray::maxSize){
		CircularArray::maxSize = (CircularArray::pushCount-CircularArray::popCount);
	}
}
/**
 * expandQ creates a new CircularArray that is double the size
 * of the current queue.  It copies all the values from the front
 * to the back into the new queue with the new front being 0 of the
 * newQ
 */
void CircularArray::expandQ(){
	cout << endl<< "Expanding Array";

	MazeNode **newQ =  new MazeNode*[arraysize*2];
	for (int i=0;i<arraysize;i++){
		newQ[i]=Q[(front+i)%arraysize];
	}
	front =0;
	back = arraysize; //reset Q ptrs for new Q
	arraysize = arraysize*2; // double arraysize
#ifdef DEBUG
	cout << "OldQ Addr = "<< Q << endl;
#endif
	delete Q;
	Q=newQ;
#ifdef DEBUG
	cout << "new Q Addr "<< Q <<endl;
	cout << "ArraySize = "<< arraysize << endl;
#endif
}

/**
 * deque takes the node at the front and returns it for processing
 * since queue works at the back and deque works at the front, this
 * provides a FIFO service routine for nodes and enables a breadth
 * first search strategy so the immediate neighbours are processed
 * before newly added nodes.
 */
MazeNode* CircularArray::deque(){
	if (is_empty()){
		cout << "attempt to dequeue empty list" << endl;
		throw new exception ();
	}
	MazeNode* result = Q[front];
	front = (front+1)%arraysize;
	CircularArray::popCount++;  // for statistics reporting
	return result;
}

bool CircularArray::is_empty(){
	return (front==back);
}

bool CircularArray::is_full(){
	return front == (back+1)%arraysize;
}
/*
 * showStats prints out the statistics around the queue usage.
 * pushCount should be >= popCount, both indicating number of nodes visited.
 * maxSize is the maximum number of elements in the queue at any given point in 
 * time.
 *
*/
void CircularArray::showStats(){
	cout << "----------Circular Array Usage Statistics ----------" << endl;
	cout << "\tPushCount = " << CircularArray::pushCount << endl;
	cout << "\tPopCount = " << CircularArray::popCount << endl;
	cout << "\tMax Array Size used = " << CircularArray::maxSize << endl;
	cout << "----------Circular Array Usage Statistics ----------" << endl;
}

// static counters initialized here for performance statistic reporting.
int CircularArray::pushCount=0;
int CircularArray::popCount=0;
int CircularArray::maxSize=0;

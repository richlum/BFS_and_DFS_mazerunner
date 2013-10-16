/*  THIS IS A COMMENT....:>  hows that for a useful comment...
 * BFSMazeRunner.cpp
 *
 *  Created on: 2011-01-14
 *      Author: rlum
 *
 *  Breadth First Search MazeRunner.  Heavily based on RandomMazeRunner with primary
 *  difference being the method by which we select the next node to be visited.
 *  BFS is built using a Queue for First In First Out ordering so that all local
 *  neighbours are processed before the next deeper layer of neighbours.
 *
 */
//#define DEBUG

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include "Maze.h"
#include "MazeRunner.h"
#include "RandomMazeRunner.h"
#include "CircularArray.h"
#include "BFSMazeRunner.h"
#include "Stack.h" // still needed for print stack

BFSMazeRunner::BFSMazeRunner() {
	initClass();
	//	cArray = new CircularArray();

}

BFSMazeRunner::~BFSMazeRunner() {

}

/**
 * solveMaze takes in a maze, starts searching from start node
 * to all neighbours in a breadth first fashion using a queue  to
 * determine processing order. As the queue (implememted
 * as a CircularArray) is FIFO - the oldest added nodes are
 * processed first yielding BFS strategy.
 * @param maze - maze to be searched. 
 * @pre maze must be initialized before calling solveMaze as 
 * this method relies on the state of the
 * mazenodes to not have marked parents and all nodes visitation
 * state to be NotVisited.
 */
void BFSMazeRunner::solveMaze (Maze *maze,ostream& solutionOutput)
{

	MazeNode *currNode;
	CircularArray* todoList = new CircularArray();
	currNode=maze->getStartMazeNode();

	if(currNode->getVisitationState()!=MazeNode::NotVisited){
		cerr << "Maze needs to be initialized before searching" << endl;
	}// all nodes should be notvisited at start otherwise someone
	// needs to clean up after themselves.
	currNode->setVisitationState(MazeNode::VisitInProgress);
	// do the search
	while ((currNode!=NULL)&&(!currNode->isExitNode())) {
#ifdef DEBUG
		currNode->print(cerr);
		cerr << "\t dbg\n";
		cerr << "\t" << currNode->getNumNeighbors() << " num neighbours\n";
		cerr << "\t" << currNode->getVisitationState() << " visitation state\n";
#endif
		// get all the reachable neighbours to current node
		// add them to the todo list if we havent "Visited" before.
		MazeNodeIterator* mit = currNode->getNeighbors();
		while(mit->hasNext()){
			MazeNode* nxt = mit->next();
			if ((nxt->getVisitationState()==MazeNode::NotVisited)){
				todoList->enqueue(nxt) ;
				if (nxt->getPathParent()==NULL)
					nxt->setPathParent(currNode);
				nxt->setVisitationState(MazeNode::VisitInProgress);
				//mark node as being visited.
			}
		}
		// once we have all the the immediate neighbours queued for
		// processing, mark this node as completed = Visited.
		currNode->setVisitationState(MazeNode::Visited);
		currNode = todoList->deque();
		// assumes that there is a solution.  If there is no 
		// solution then we will try to deque and empty queue as there
		// will be no more reachable nodes to explore.
		// currently deque will throw an exception if
		// we try to deque and empty queue to prevent silent
		// endless loop that will occur here.
	}

	// yaay!!!  we solved it!
	// print out our solution
	solutionOutput << "BFS Solution------\n"; 
	// use the stack to reverse the order of the solution from exit first to startNode first.
	Stack* printStack = new Stack();
	while (currNode != maze->getStartMazeNode()) {
		printStack->push(currNode);
		currNode = currNode->getPathParent();
	}
	// print out the startnode as part of the solution path
	currNode->print(solutionOutput);
	currNode = printStack->pop();
	while(!currNode->isExitNode()){
		// print this node in the solution
		currNode->print(solutionOutput);
		solutionOutput << " "; // separate by spaces
		currNode = printStack->pop();
	}
	// and the entry node
	currNode->print(solutionOutput);
	solutionOutput << "\n";

	delete printStack;

	todoList->showStats();
	//delete todoList;

}


void BFSMazeRunner::initClass()
{
	// place holder based on RandomMazeRunner.


#ifdef DEBUG
	cout << " we are debugging and in BFSMazeRunner" << endl;
#else

#endif
}

/*
 * DFSMazeRunner.cpp
 *
 *  Created on: 2011-01-14
 *      Author: rlum
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
#include "DFSMazeRunner.h"
#include "Stack.h" // still needed for print stack

DFSMazeRunner::DFSMazeRunner() {
	initClass();
	cArray = new CircularArray();

}

DFSMazeRunner::~DFSMazeRunner() {

}


void DFSMazeRunner::solveMaze (Maze *maze,ostream& solutionOutput)
{

	MazeNode *currNode;
	CircularArray* todoList = new CircularArray();
	currNode=maze->getStartMazeNode();

	if(currNode->getVisitationState()!=MazeNode::NotVisited){
		cerr << "Maze needs to be initialized before searchicng" << endl;
	}// all nodes should be notvisited at start otherwise someone
	// needs to clean up after themselves.
	currNode->setVisitationState(MazeNode::VisitInProgress);
	while (!currNode->isExitNode()) {
#ifdef DEBUG
		currNode->print(cerr);
		cerr << "\t dbg\n";
	    cerr << "\t" << currNode->getNumNeighbors() << " num neighbours\n";
	     cerr << "\t" << currNode->getVisitationState() << " visitation state\n";
#endif
	     MazeNodeIterator* mit = currNode->getNeighbors();
		while(mit->hasNext()){
		   	MazeNode* nxt = mit->next();
		   	if ((nxt->getVisitationState()==MazeNode::NotVisited)){
		   		todoList->enqueue(nxt) ;
		   		nxt->setPathParent(currNode);
		   		nxt->setVisitationState(MazeNode::VisitInProgress);
		   	}
		}
		currNode->setVisitationState(MazeNode::Visited);
		currNode = todoList->deque();
		// assumes that there is a solution
		// will end up silently looping if ther isnt.
	}

	  // yaay!!!  we solved it!
	  // print out our solution
	  solutionOutput << "DFS\n"; // print out the type of solution
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


}



MazeNode *DFSMazeRunner::pickNextNode(MazeNode *currNode, CircularArray* todoList)
{
  MazeNode *nextNode;
  int numNeighbors,pickNum;
  numNeighbors=currNode->getNumNeighbors();
  MazeNodeIterator* mit = currNode->getNeighbors();
#ifdef DEBUG
     currNode->print(cerr);
     cerr << "\t PicknextNode (currNode)\n";
#endif
  while (mit->hasNext()){
	  MazeNode* next = mit->next();
#ifdef DEBUG
     next->print(cerr);
     cerr << "\t mit->next()\n";
     cerr << "\t" << next->getNumNeighbors() << " num neighbours\n";
     cerr << "\t" << next->getVisitationState() << " visitation state\n";
     cerr << "\t end mit->next()\n";

#endif

	  if (next->getVisitationState() == MazeNode::NotVisited){
		  todoList->enqueue(next);
		  next->setVisitationState(MazeNode::VisitInProgress);
	  }// queue up unvisited neighbours for visiting
  }
  delete mit;

#ifdef DEBUG
  cerr << "todolist->size() " << todoList->size()<< endl;
  todoList->dumpStack();
#endif
  return todoList->deque();
}


void DFSMazeRunner::initClass()
{
	// place holder based on RandomMazeRunner.


#ifdef DEBUG
	cout << " we are debugging and in DFSMazeRunner" << endl;
#else

#endif
  }

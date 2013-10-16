/*
 * BFSMazeRunner.cpp
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
#include "Stack.h"
#include "BFSMazeRunner.h"

BFSMazeRunner::BFSMazeRunner() {
	initClass();
	stack = new Stack();

}

BFSMazeRunner::~BFSMazeRunner() {

}

void BFSMazeRunner::solveMaze (Maze *maze,ostream& solutionOutput)
{
  MazeNode *currNode;
  Stack* todoList = new Stack();
#ifdef DEBUG
  cout << "MazeNode::VisitInProgress = " << MazeNode::VisitInProgress << endl;
  cout << "MazeNode::NotVisited = " << MazeNode::NotVisited << endl;
  cout << "MazeNode::Visited = " << MazeNode::Visited << endl;
#endif

  currNode=maze->getStartMazeNode();
  //todoList->push(currNode);
  while (!currNode->isExitNode()) {

    // for debugging
#ifdef DEBUG

     currNode->print(cerr);
     cerr << "\t dbg\n";
     cerr << "\t" << currNode->getNumNeighbors() << " num neighbours\n";
     cerr << "\t" << currNode->getVisitationState() << " visitation state\n";
     MazeNodeIterator* mit = currNode->getNeighbors();
     while(mit->hasNext()){
    	 MazeNode* nxt = mit->next();
    	 cerr << "\t\t" ;
    	 nxt->print(cerr);
    	 cerr << " v:" <<nxt->getVisitationState() ;
    	 cerr << " neigbour\n";
     }
     cerr << "\t end dbg\n";
     delete mit;
#endif
    // make a note that we have started the visit
    currNode->setVisitationState(MazeNode::VisitInProgress);
    // pick a next node
    MazeNode *nextNode;
    nextNode=pickNextNode(currNode,todoList);
#ifdef DEBUG
	 nextNode->print(cerr);
     cerr << "\t pickedNextNode\n";
     cerr << "\t" << nextNode->getNumNeighbors() << " num neighbours\n";
     cerr << "\t" << nextNode->getVisitationState() << " visitation state\n";
     mit = nextNode->getNeighbors();
     while(mit->hasNext()){
    	 MazeNode* nxt = mit->next();
    	 cerr << "\t\t" ;
    	 nxt->print(cerr);
    	 cerr << " neigbour\n";
     }
     cerr << "\t end dbg\n";
     delete mit;

#endif
    // make a note that this node is the parent of the next node (so
    // that we can later reconstruct the path we followed to the
    // solution).
    //
    // ONLY change the parent if it's not already set, however.
    // (Otherwise, we can end up with a parent loop!)
    if (nextNode->getPathParent() == NULL)
      nextNode->setPathParent(currNode);
    // make a node that we have completed the visit
    currNode->setVisitationState(MazeNode::Visited);
    // and go to the next node
    currNode=nextNode;
  }
  // yaay!!!  we solved it!
  // print out our solution
  solutionOutput << "BFS\n"; // print out the type of solution
//  solutionOutput << "(Solution is backward.  Oops!\n"
//		 << "221 students: see RandomMazeRunner.cpp for "
//		 << "some thoughts about that.\n";

  // Use a stack to reverse the order of output
  // Note that currNode is already the exit node (at the point we left
  // the loop above).
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
#ifdef DEBUG
    cout<< "V: " << currNode->getVisitationState() << endl;
#endif
    // get to the next node
    currNode = printStack->pop();
  }
  // and the entry node
  currNode->print(solutionOutput);
  solutionOutput << "\n";

  delete printStack;

}


MazeNode *BFSMazeRunner::pickNextNode(MazeNode *currNode, Stack* todoList)
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
		  todoList->push(next);
		  next->setVisitationState(MazeNode::VisitInProgress);
	  }// queue up unvisited neighbours for visiting
  }
  delete mit;

#ifdef DEBUG
  cerr << "todolist->size() " << todoList->size()<< endl;
  todoList->dumpStack();
#endif
  return todoList->pop();
}


void BFSMazeRunner::initClass()
{
	// place holder based on RandomMazeRunner.


#ifdef DEBUG
	cout << " we are debugging and in BFSMazeRunner" << endl;
#else

#endif
  }

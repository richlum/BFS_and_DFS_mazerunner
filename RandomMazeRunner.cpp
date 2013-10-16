/* 221 STUDENTS: this is a useful file; see comments in RandomMazeRunner.h */

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

#include "Maze.h"
#include "MazeRunner.h"
#include "RandomMazeRunner.h"
#include "Stack.h"
#include "StackNode.h"

RandomMazeRunner::RandomMazeRunner ()
{
  // Call the static class initializer. It will run only the first
  // time it's called.
  initClass();
}

RandomMazeRunner::~RandomMazeRunner ()
{
}

void RandomMazeRunner::solveMaze (Maze *maze,ostream& solutionOutput)
{
  MazeNode *currNode;

  currNode=maze->getStartMazeNode();
  while (!currNode->isExitNode()) {

    // for debugging
    // currNode->print(cerr);
    // cerr << "\n";

    // make a note that we have started the visit
    currNode->setVisitationState(MazeNode::VisitInProgress);

    // pick a next node
    MazeNode *nextNode;
    nextNode=pickNextNode(currNode);

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

  // Use a stack to reverse the order of output
  // Note that currNode is already the exit node (at the point we left
  // the loop above).
  Stack* myStack = new Stack();
  while (currNode != maze->getStartMazeNode()) {
	  myStack->push(currNode);
	  currNode = currNode->getPathParent();
  }
  // print out the startnode as part of the solution path
  currNode->print(solutionOutput);
  currNode = myStack->pop();
  while(!currNode->isExitNode()){
    // print this node in the solution
    currNode->print(solutionOutput);
    solutionOutput << " "; // separate by spaces

    // get to the next node
    currNode = myStack->pop();
  }
  // and the entry node
  currNode->print(solutionOutput);
  solutionOutput << "\n";

  delete myStack;
//  cout << " testing creation of mySTack" << endl;
//  Stack* myStack = new Stack();
//  cout << myStack->size() << endl;
//  cout << "done with myStack"<< endl;
}

MazeNode *RandomMazeRunner::pickNextNode(MazeNode *currNode)
{
  MazeNode *nextNode;
  int numNeighbors,pickNum,r;
  numNeighbors=currNode->getNumNeighbors();
  r=rand();
  pickNum=(int)((double)(numNeighbors)*(double)(rand())/((double)(RAND_MAX)+1.0));
  assert(pickNum>=0 && pickNum<numNeighbors);

  // now get the node corresponding to that number
  MazeNodeIterator *iter;
  int i;
  iter=currNode->getNeighbors();
  for (i=0; i<=pickNum; i++) {
    assert(iter->hasNext());
    nextNode=iter->next();
  }
  delete iter;

  return nextNode;
}

void RandomMazeRunner::initClass()
{
  static bool called = false;

  if (!called) {
    // Ensure that this function is called only once per program
    called = true;

#ifdef DEBUG
    // Initialize the randomizer to a constant for testing purposes.
    // Zero is a pretty, round constant.
    srand(0);
#else
    // Initialize the randomizer with time (which changes pretty frequently).
    srand(time(NULL));
#endif
  }
}

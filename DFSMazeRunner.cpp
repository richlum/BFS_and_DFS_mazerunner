/*
 * DFSMazeRunner.cpp
 *
 *  Created on: 2011-01-14
 *      Author: rlum
 * 
 * Depth First Search algrothim to explore maze.   Heavily based on RandomMazeRunners with modifications
 * primarily to the order of which node is next explored.   Uses a Last In First Out stack to  
 * ensure that the node that is furthest away is then next one to be visited.
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
#include "Stack.h"
#include "DFSMazeRunner.h"

DFSMazeRunner::DFSMazeRunner() {
	initClass();
	stack = new Stack();

}

DFSMazeRunner::~DFSMazeRunner() {

}
/**
 * solveMaze takes in a maze, starts searching from start node
 * to all neighbours in a depth first fashion using a stack  to
 * determine processing order. As the stack is LIFO - the most recently
 * added nodes are processed first yielding DFS strategy.
 * 
 * @param maze - maze to be searched.  
 * @pre maze must be initialized before
 * calling solveMaze as this method relies on the state of the
 * mazenodes to not no parents and all nodes visitation
 * state to be NotVisited.
 */
void DFSMazeRunner::solveMaze (Maze *maze,ostream& solutionOutput)
{
	MazeNode *currNode;
	Stack* todoList = new Stack();
	currNode=maze->getStartMazeNode();


	if(currNode->getVisitationState()!=MazeNode::NotVisited){
		cerr << "Maze needs to be initialized before searching" << endl;
	}// all nodes should be notvisited at start otherwise someone
	// needs to clean up after themselves.

	// do the search
	currNode->setVisitationState(MazeNode::VisitInProgress);
	while ((currNode!=NULL)&&(!currNode->isExitNode())) {
//		 NULL signals nothing left to search - either bug or unreachable ExitNode
#ifdef DEBUG
		cerr << "currNode : "
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
		// pick a next node to visit
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
		// make a note that this node is the parent of the next node
		// this function was copied to pickNextNode and is used
		// here in case we get a node that wasn't given to us by
		// pickNextNode
		// ONLY change the parent if it's not already set, however.
		// (Otherwise, we can end up with a parent loop!)
		if (nextNode!=NULL){
			if (nextNode->getPathParent() == NULL)
				nextNode->setPathParent(currNode);
			// make a note that we have completed the visit
			currNode->setVisitationState(MazeNode::Visited);
			// and go to the next node
			currNode=nextNode;
		}else{ // we havent found exit node and we have nothing left on stack to search
			currNode = NULL; // flag that stack is now empty
		}
	}// end while (currNode(!isExitNode)
	if (currNode!=NULL){
		// yaay!!!  we solved it!
		// Note that currNode is already the exit node (at the point we left
		// the loop above). Use Stack to output solution from begin to end since
		// we have the exitNode in hand.
		Stack* printStack = new Stack();
		while (currNode != maze->getStartMazeNode()) {
			printStack->push(currNode);
			currNode = currNode->getPathParent();
		}
		// print out the startnode as part of the solution path
		cout << "DFS Solution----" << endl;
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

	}else { //no solution found.
		// its possible maze has unconnected nodes. find all unvisited nodes
		// to allow manual verification that we have visited all reachable nodes.
		MazeNodeIterator* mit = maze->getAllMazeNodes();
		int unvisited = 0;
		cout <<"DFS - no solution found" << endl;
		cout <<"The following MazeNodes were not visited" << endl;
		while (mit->hasNext()){
			MazeNode* aMazeNode = mit->next();
			if (aMazeNode->getVisitationState() == MazeNode::NotVisited){
				unvisited++;
				aMazeNode->print(cout);
			}
		}
		cout << endl << "End of List of unvisted MazeNodes" << endl;
	}

	todoList->showStats();    
}

/**
 * pickNextNode - takes the current node and a stack as input and
 * visits all the neighbours to the current node and places them
 * on the stack for later processing.  returns the top of stack as
 * the next one to visit.
 * @param currNode - the current node being visited
 * @param todoList - a stack containing nodes to be processed
 * @return the next MazeNode for processing.
 * @return todoList is updated to contain all the neighbours nodes to
 * 	currNode.  Sets VisitationState neighbourNodes=VisitInProgress
 * indicating that node is already in the stack for processing.
 * Also updates the MazeNode pathParent for replay of solution path
 *
 */
MazeNode *DFSMazeRunner::pickNextNode(MazeNode *currNode, Stack* todoList)
{
	//MazeNode *nextNode;
	int numNeighbors;  // ,pickNum;
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
		// set the parent node for replay of solution later.
		// only do this if NULL, otherwise loops will form.
		if (next->getPathParent() == NULL)
			next->setPathParent(currNode);
	}
	delete mit;

#ifdef DEBUG
	cerr << "todolist->size() " << todoList->size()<< endl;
	todoList->dumpStack();
#endif
	if (todoList->size() > 0 ){
		return todoList->pop();
	}
	return NULL;

}


void DFSMazeRunner::initClass()
{
	// place holder based on RandomMazeRunner.
	// not required as we have no global random number generator to build/seed.


#ifdef DEBUG
	cout << " we are debugging and in DFSMazeRunner" << endl;
#else

#endif
}

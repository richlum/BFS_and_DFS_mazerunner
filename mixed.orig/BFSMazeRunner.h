/*
 * BFSMazeRunner.h
 *
 *  Created on: 2011-01-14
 *      Author: rlum
 */


#ifndef BFSMAZERUNNER_H_
#define BFSMAZERUNNER_H_

#include "Stack.h"

using namespace std;

class BFSMazeRunner : public MazeRunner{
protected:
  // helper functions
  MazeNode *pickNextNode(MazeNode *currNode, Stack* todoList);

  // static initialization (currently seeds random number generator).
  // executes once per run of the program, regardless of how many
  // times it's called.
  static void initClass();
public:
	BFSMazeRunner();
	~BFSMazeRunner();

	void solveMaze (Maze *maze,ostream& solutionOutput);
private:
	Stack* stack;
};

#endif /* BFSMAZERUNNER_H_ */

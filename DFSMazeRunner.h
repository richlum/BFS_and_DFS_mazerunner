/*
 * DFSMazeRunner.h
 *
 *  Created on: 2011-01-14
 *      Author: rlum
 */


#ifndef DFSMAZERUNNER_H_
#define DFSMAZERUNNER_H_

#include "Stack.h"

using namespace std;

class DFSMazeRunner : public MazeRunner{
protected:
  // helper functions
  MazeNode *pickNextNode(MazeNode *currNode, Stack* todoList);

  // static initialization (currently seeds random number generator).
  // executes once per run of the program, regardless of how many
  // times it's called.
  static void initClass();
public:
	DFSMazeRunner();
	~DFSMazeRunner();

	void solveMaze (Maze *maze,ostream& solutionOutput);
private:
	Stack* stack;
};

#endif /* DFSMAZERUNNER_H_ */

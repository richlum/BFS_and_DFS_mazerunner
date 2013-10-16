
/*
 * DFSMazeRunner.h
 *
 *  Created on: 2011-01-14
 *      Author: rlum
 */


#ifndef DFSMAZERUNNER_H_
#define DFSMAZERUNNER_H_

#include "CircularArray.h"

using namespace std;

class DFSMazeRunner : public MazeRunner{
protected:
  // helper functions
  MazeNode *pickNextNode(MazeNode *currNode, CircularArray* todoList);

  static void initClass();
public:
	DFSMazeRunner();
	~DFSMazeRunner();

	void solveMaze (Maze *maze,ostream& solutionOutput);
private:
	CircularArray* cArray;
};

#endif /* DFSMAZERUNNER_H_ */

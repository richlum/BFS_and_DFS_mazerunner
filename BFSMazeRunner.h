
/*
 * BFSMazeRunner.h
 *
 *  Created on: 2011-01-14
 *      Author: rlum
 */


#ifndef BFSMAZERUNNER_H_
#define BFSMAZERUNNER_H_

#include "CircularArray.h"

using namespace std;

class BFSMazeRunner : public MazeRunner{
protected:
  // helper functions
  //MazeNode *pickNextNode(MazeNode *currNode, CircularArray* todoList);

  static void initClass();
public:
	BFSMazeRunner();
	~BFSMazeRunner();

	void solveMaze (Maze *maze,ostream& solutionOutput);
private:
//	CircularArray* cArray;
};

#endif /* BFSMAZERUNNER_H_ */

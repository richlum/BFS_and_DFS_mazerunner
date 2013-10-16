/*
 * CircularArray.h
 *
 *  Created on: 2011-01-13
 *      Author: rlum
 */

#ifndef CIRCULARARRAY_H_
#define CIRCULARARRAY_H_

#include "Maze.h"

using namespace std;

class CircularArray {
public:
	CircularArray();
	~CircularArray();
	void enqueue(MazeNode* );
	MazeNode* deque();
	bool is_empty();
	bool is_full();
	void showStats();
private:
	int arraysize;
	MazeNode** Q;
	//pointer to a pointer to MazeNode aka pointer to array of pointers to mazenodes
	//required to enable pointer to switch to different arrays for dynamic inflation of Q
	int front ;
	int back ;
	void expandQ();
	// set array size arbitrarily low to exercise auto inflation code for testing
	static const int initialSize = 5;
	// some counters for peeking at activity required to reach solution.
	static int pushCount;
	static int popCount;
	static int maxSize;
};

#endif /* CIRCULARARRAY_H_ */

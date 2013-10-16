#ifndef STACKNODE_H_
#define STACKNODE_H_
#include "Maze.h"


class StackNode {
public:

	StackNode(MazeNode*, StackNode* );
	~StackNode();
	MazeNode* getItem();
	StackNode* next;
private:
	
	MazeNode* theItem;
};

#endif /* STACKNODE_H_ */


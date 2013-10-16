#ifndef STACK_H_
#define STACK_H_

#include "Maze.h"
#include "StackNode.h"

using namespace std;


class Stack {
public:
	Stack( );
	~Stack();
	int size();

	void push(MazeNode* );
	MazeNode* pop();

	void dumpStack();
	void showStats();
private:

	StackNode* head;
	int qty;


	static int pushCount;
	static int popCount;
	static int maxSize;
};



#endif /* STACK_H_ */


/* 
*
* Modified to 
* 
* add SolutionStart and SolutionExit with red circle so that maze progress could
* be observed in context with start and finish points in view
*
* Added explicit display of link to parent node after node set to Visited state to 
* help user see what is a parent vs what is a neighbour.
* 
* moved pause between node paints rather than at start as it seemed more useful to
* allow user control of timing between node paints rather than just once at the 
* beginning,
 */

#include <iostream>
#include <cassert>
#include <unistd.h>

#include "Maze.h"
#include "SquareMaze.h"
#include "VisualizeSquareMazeRunner.h"

extern "C" {
#include "GPKernel.h" // some graphics stuff borrowed from a previous course
}

const int VisualizeSquareMazeRunner::PixelsPerNode=40;
const int VisualizeSquareMazeRunner::BackgroundColor=MED_GRAY; // this shows up very little, as you can see
const int VisualizeSquareMazeRunner::WallColor=WHITE;
const int VisualizeSquareMazeRunner::VisitedColor=BLUE;
const int VisualizeSquareMazeRunner::VisitInProgressColor=YELLOW;
const int VisualizeSquareMazeRunner::NotVisitedColor=MED_GRAY;
const int VisualizeSquareMazeRunner::SolutionColor=PINK;
const int VisualizeSquareMazeRunner::StartEndColor=RED;
const int VisualizeSquareMazeRunner::LinkColor=BLACK;

VisualizeSquareMazeRunner::VisualizeSquareMazeRunner (SquareMaze *_maze)
{
  pauseTimeInSec=0.05; // default

  maze=_maze;
  maze->setChangeListener(this);

  // set up our window
  int windowWidth,windowHeight;
  windowWidth=maze->getWidth()*PixelsPerNode;
  windowHeight=maze->getHeight()*PixelsPerNode;
  openPackage(windowWidth+1,windowHeight+1);

  drawInitialWindow();
  flush(); // flush graphics operations to screen

}
VisualizeSquareMazeRunner::~VisualizeSquareMazeRunner ()
{
  closePackage();
}
void VisualizeSquareMazeRunner::drawInitialWindow (void)
{
  int windowWidth,windowHeight;
  windowWidth=maze->getWidth()*PixelsPerNode;
  windowHeight=maze->getHeight()*PixelsPerNode;

  setPenColor(BackgroundColor);
  drawRect(0,0,windowWidth,windowHeight,0);
  
  // draw boundary lines
  setPenColor(WallColor);
  drawLine(0,0,windowWidth,0,1);
  drawLine(windowWidth,0,windowWidth,windowHeight,1);
  drawLine(windowWidth,windowHeight,0,windowHeight,1);
  drawLine(0,windowHeight,0,0,1);

  // draw walls
  int x,y;
  for (y=0; y<maze->getHeight(); y++) {
    for (x=0; x<maze->getWidth(); x++) {
      SquareMaze::SquareMazeNode *node=maze->getNodeAt(x,y);
      if (!node->canGoLeft()) {
	drawLine(x*PixelsPerNode,y*PixelsPerNode,x*PixelsPerNode,(y+1)*PixelsPerNode,1);
      }
      if (!node->canGoUp()) {
	drawLine(x*PixelsPerNode,y*PixelsPerNode,(x+1)*PixelsPerNode,y*PixelsPerNode,1);
      }
    }
  }


  MazeNode* startNode = maze->getStartMazeNode();


  // draw squares
  for (y=0; y<maze->getHeight(); y++) {
	  for (x=0; x<maze->getWidth(); x++) {
		  SquareMaze::SquareMazeNode *node=maze->getNodeAt(x,y);
		  drawNode(x,y,getColor(node->getVisitationState()));
		  if (node->isExitNode()) { // capture exit node location
			  xfin = x;
			  yfin = y;
		  }
		  if (startNode == node){
			  xstart = x;
			  ystart = y;
		  }
	  }
  }
	// NEW: Draw start and stop nodes
  drawSpNode(xfin,yfin,VisualizeSquareMazeRunner::StartEndColor);
  drawSpNode(xstart,ystart, VisualizeSquareMazeRunner::StartEndColor );
}

int VisualizeSquareMazeRunner::getColor (MazeNode::VisitationState visitationState)
{
  switch (visitationState) {
  case MazeNode::Visited:
    return VisitedColor;
    break;
  case MazeNode::NotVisited:
    return NotVisitedColor;
    break;
  case MazeNode::VisitInProgress:
    return VisitInProgressColor;
    break;
  }
  assert(0); // illegal or unknown value
  return BackgroundColor; // keep compiler happy
}
void VisualizeSquareMazeRunner::drawNode (int x,int y,int color)
{
  setPenColor(color);
  drawRect(x*PixelsPerNode+1,y*PixelsPerNode+1,PixelsPerNode-1,PixelsPerNode-1,0);
	// draw start and stop nodes  whenever we update any other nodes.
  drawSpNode(xfin,yfin,VisualizeSquareMazeRunner::StartEndColor);
  drawSpNode(xstart,ystart,VisualizeSquareMazeRunner::StartEndColor);
  pause(); // added pause here to allow user control of inter node pause draw times.
}

/**
 * NEW METHOD : 20110125
 * call this with the coordinates of node and parentnode.  Note matrix row and colum 
 * are both zero based.
 * @param xOne is x coordinate of current node (in matrix columns)
 * @param yOne is y coordinate of current node (in matrix rows)
 * @param xTwo is the x coordinate of the parent node (in matrix columns)
 * @param yTwo is the y coordinate of the parent node (in matrix rows)
 */
void VisualizeSquareMazeRunner::drawParentLink(int xOne, int yOne, int xTwo, int yTwo, int color)
{
	setPenColor(color);
	if (xOne == xTwo){ // vertical line
		if (yOne > yTwo){
			drawRect (xOne*PixelsPerNode + PixelsPerNode/2, yTwo*PixelsPerNode + PixelsPerNode/2,
				4, PixelsPerNode,0);
		}else{
			drawRect (xOne*PixelsPerNode + PixelsPerNode/2, yOne*PixelsPerNode + PixelsPerNode/2,
							4, PixelsPerNode,0);
		}
	}else{ // horizontal line
		if (xOne>xTwo){
			drawRect (xTwo*PixelsPerNode + PixelsPerNode/2, yOne*PixelsPerNode + PixelsPerNode/2,
						 PixelsPerNode,4,0);
		}else{
			drawRect (xOne*PixelsPerNode + PixelsPerNode/2, yOne*PixelsPerNode + PixelsPerNode/2,
									 PixelsPerNode,4,0);
		}
	}

}
/*
NEW METHOD: 20110125
Paint circles at the start and end node.  
Given a Node, there seems to be no support for finding its maze co-ordinates so 
this brute forces it from the maze side by iterating through all maze nodes until
the start and stop nodes are identified.
*/
void  VisualizeSquareMazeRunner::drawSpNode(int x,int y,int color)
{
	setPenColor(color);
	drawOval(x*PixelsPerNode+5,y*PixelsPerNode+5,PixelsPerNode-7,PixelsPerNode-7,0);

}
void VisualizeSquareMazeRunner::visitationStateChanged (MazeNode *_changedNode)
{
  // well, we know this is really a SquareMazeNode, since that's all we work on
  SquareMaze::SquareMazeNode *node=(SquareMaze::SquareMazeNode *)_changedNode;
	// add logic to draw parent links when we are marking node visited.
  drawNode(node->getX(),node->getY(),getColor(node->getVisitationState()));
  int pX;
  int pY;
  if (node->getPathParent()!=NULL){
	  // doesnt seem to be an obvious way to get xy given a node other than
	  // exhaustive search.
	  MazeNode* parentNode = node->getPathParent();
	  for (int y=0; y<maze->getHeight(); y++) {
		  for (int x=0; x<maze->getWidth(); x++) {
			  SquareMaze::SquareMazeNode *node=maze->getNodeAt(x,y);
			  if (parentNode == node){
				  pX = x;
				  pY = y;
			  }
		  }
	  }
	if (node->getVisitationState()==MazeNode::Visited) 
	  drawParentLink(node->getX(),node->getY(),pX,pY,LinkColor );
  }
  flush();
  pause(); // this is original but doesnt seem to have desired effect of pausing
           // between node updates?
}



void VisualizeSquareMazeRunner::setPauseTime (double _pauseTimeInSec)
{
  pauseTimeInSec=_pauseTimeInSec;
}
void VisualizeSquareMazeRunner::pause (void)
{
  usleep((unsigned long)(pauseTimeInSec*1000000.0));
}
void VisualizeSquareMazeRunner::startSolutionPath (void)
{
  drawInitialWindow(); // just in case we've already drawn a previous solution
}
void VisualizeSquareMazeRunner::addNextInSolutionPath (int x,int y)
{
  drawNode(x,y,SolutionColor);
}
void VisualizeSquareMazeRunner::doneSolutionPath(void)
{
  flush();
}
void VisualizeSquareMazeRunner::waitForMouseClick (void)
{
  eventRecord e;
  int type;
  do {
    type = getNextEvent(&e);
    if (type == EXPOSE) {
      flush();
    }
  } while (type != MOUSE);

}

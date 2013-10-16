Student Name #1:    Richard Lum	
Student ugrad login #1:  i0r7

Student Name #2:
Student ugrad login #2:

Team name (for fun!):  Dr Noonien Soong

Acknowledgment that you understand and have followed the course's
collaboration policy
(http://www.ugrad.cs.ubc.ca/~cs221/current/syllabus.shtml#conduct):

[[Put your names here again]]
Richard Lum  

----------------------------------------------------------------------

Approximate hours on project:
60 hours 
----------------------------------------------------------------------

For teams, rough breakdown of work:
me = 30%
myself = 30%
I = 40%  
----------------------------------------------------------------------

Acknowledgment of assistance:
http://www.gidforums.com/t-12539.html  :  pointer array syntax
http://www.delorie.com/gnu/docs/gdb/gdb_toc.html#SEC_Contents : gdb general resource
http://www.comeaucomputing.com/techtalk/templates/#whylinkerror : templates
	working standalone prototype for stack but unable to successfully integrate to project
http://www.cplusplus.com/reference/  : general c++ api reference	
http://tronche.com/gui/x/xlib/graphics/  : for some light reading on xlib
Essex (lab TA)  : assistance with pointers and makefile
matygo : general discussion topics.
texts : koffman (Object Abstaction Datastructures and design using c++),
		Eckels (Thinking in C++)
		Strousoup (the C++ Programming Language)
		
----------------------------------------------------------------------

Files in your submission:

[Add documentation to this list:]
	BFSMazeRunner.[h,cpp]- Breadth first search using CircularArray as Queue 
	DFSMazeRunner.[h,cpp]- Depth first search using a Stack (Linked List of StackNode)
	CircularArray.[h,cpp] - Circular Array with dynamic resizing that double size on each limit
	Stack.[h,cpp] - base pointer and method for linked list of StackNode
	StackNode.[h,cpp] - hold pointer to a MazeNode and next StackNode
	solns/   captured solutions, maze0-4,verified by hand, others to get sense of scaling
		and impact of openess
	
* README.txt: this file
* runmaze.cpp

* Makefile


----------------------------------------------------------------------

High-level description of MazeRunners, Stack, and Queue, including any
problems or surprises:

The two maze runners, DFS and BFS are nearly identical except for implementation of 
picknextnode. DFSMazeRunner detects unreachable exits and reports all unvisited nodes
on failure to find a connection so that unvisited nodes can be manually verified.  
Just to try out the c++ exceptions, BFSMazeRunner uses 
the attempt of popping empty stack as the indicator that we have an unreachable exit
and will throw an exception when there is no reachable exit node. 
It could have used same mechanism implemented in DFSMazeRunner.  
Test Data for this is inputs/mazeNoSoln.txt   

The DFSMazeRunner seems to produce the longest paths while BFSMazeRunner always 
produces the shortest.  The RandomMazeRunner often produces paths very similar 
to BFSMazeRunner.  Running runmaze for inputs/mazeB.txt provides a large enough
maze that is open enough for the search pattern differences to be more obvious.
The alternating down up path for dfs vs the circular spread pattern of bfs can
be seen in the visualation.

Stack is essentially the anchor point for linked list of StackNode and has
methods to manage the list StackNode

CircularArray is largely based on class notes with minor implementation adaptions.
Included the ability to dynamically resize the Array if capacity is reached. It was
implemented as a pointer to an Array of pointers to MazeNode to allow dynamic replacement 
of the existing array with a larger array.  Initial size was set to an arbitrarly 
small number to exercise the automatic array expansion.

----------------------------------------------------------------------

Example BFS << DFS maze:
	all BFS runs were always the shortest solution path or tied to be the shortest solution path.
	The random solutions tended to be closer to the BFS as the probablity of 
	moving another step deeper is inversely proportional to the number of neighbours so
	there is a peference for localized exploration for randommaze runner.
	
	In terms of exploration, the DFSMazeRunner explores the most of the maze when the
	exit is closest it can be to entry node without being on the first path chosen.
	This allows for the deep paths to wind around the maze and having the close exit places
	it first in the stack and is buried underneath all the other further away nodes that 
	get processed on a LIFO basis.  Example is in inputs/maze4DFS.txt
	
Example DFS << BFS maze:
	Depth first searches were quite often the longest solutions but if we place the startNode
	in the center of the maze and the exitnode in the corner of the maze DFS will beat BFS if 
	we pick the correct corner for exit placement (the one that is earliest in the DFS search 
	path).  Since BFS exploration model is essentially circlular in nature, expanding much like
	a puddle would around the center. Non Square matrices where the exit node is on the long 
	side outside of the radius would tend to favor DFS over BFS as long as exit is in the DFS
	initial search path.  example of square matrix inputs/maze4BFS2.txt shows
	a case where this implementation of BFS explores about double that of DFS.  The exit node 
	is placed on the wrong side relative to the initial DFS search path, it commonly doubles the 
	number of nodes searched by DFS. 
	
	The two search methods also report statistics on the maximum size of the
	queue/stack as well as the number of push/queues and pop/dequeues.  Using the
	count of pushes and pops as representing the amount of exploration of the maze 
	required to find the exit, confirms the discussion above.
----------------------------------------------------------------------

Answers to questions:

	For the DFS, if we removed the protective statement the prevented enqueing a node
	that has already been visited would result in the stack blowing up as DFS loads
	as many neighbours as it finds before processing a node.  The stack in the DFS
	would run out of memory long before the queue in the BFS as BFS processes elements
	on a first in first out basis and tends to keep the total number of nodes being
	visited at any given point in time much smaller than DFS does.  This can be seen
	in the maxsize fields reported for each and the rate of relative growth can be seen
	in the 100 and 200 size maze runs included in the solution examples where we see
	the size of the DFS stack grown more than an order of magnitude larger than the 
	BFS Queue for 100x100 matrix.  
	
	The BFS would run faster than DFS should the maze runners be required to work on 
	large mazes that could force disk swap to be used. As mentioned above the 
	size of the DFS stack is much large than a BFS queue and would hit the memory limits
	much earlier.
	
	Note Bonus work items:
	- auto resizing of circular array. Default initial size is 5 and it doubles on each
	time it reaches limit.
	- phase 1 of visualization mod by painting the start node and exit node red. The safest 
	place to ensure the nodes were not cleared was to embed the drawing of the start and 
	exit nodes each time VisualizeSquareMazeRunner::drawNode.  Likely overkill to refresh
	the start/end node on each node draw.  Added to class members xstart,
	ystart, xfin, yfin to capture the start and end node locations in terms of maze co-ordinates
	that would be globally available within the VisualizeSquareMazeRunner.  These are initialized
	from within  VisualizeSquareMazeRunner::drawInitialWindow.
	- phase 2 - parent path visualization.  I was wanting to see the effect of only writing 
	parent path if it was null vs always writing the lastest parent path, so I added parent path 
	trace to visualization. There is a bug somewhere that erases half a parent link. As 
	links straddle two nodes, it seems like there is a competing refresh that overwrites 
	a node that already had a parent link drawn on it, so you will see odd cases where there
	is a half link that should be a full link.  I suspect that I need to understand the event
	model to find/fix this .....quickly running out of time. So decided to stop here. 
	Also note that since the nodes are drawn 1 pixel less than the matrix node size to allow for
	viewing of walls, the refresh only paints the node square, leaving 2 pixel wide black
	marks from the parentlinks behind.  Should repaint entire maze between Runners.
	- phase 3 - not implemented was to do something to see the size of the stack / queue as
	it was being updated.   As I dove into phase 1 and phase 2, it became apparent that I 
	dont have enough information as this would also require a better understanding of the
	event model so that I could get/or send a notification to update what ever graphic
	i came up with for stack/queue representation.  There are partial pieces of code for the
	initial stab at this to draw a second window but its largely uncompleted/untested and I 
	only mention it because you may come across these currently unsed pieces of code.
	- stack and queue statistics to provide concrete reference points for relative number of
	operations and memory requirements for each.
	- tested for ability to detect noreachable exits.
	- tested up to 200x200 size maze.


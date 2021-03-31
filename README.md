# Pathfinding_Challenge_kattis
The Path Finding challenge on Kattis is completed in C++


Path-finding
Implement a path-finding algorithm in C++ that finds and returns a shortest path between two points in a 2-dimensional grid.

Note that this specific problem can only be solved using C++.

API
Your algorithm should provide an implementation of the following function declaration.

int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY, 
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize);
The meaning of the parameters are as follows.

nStartX and nStartY are the 0-based coordinates of the start position.

nTargetX and nTargetY are the 0-based coordinates of the target position.

pMap describes a grid of width nMapWidth and height nMapHeight. The grid is given in row-major order, each row is given in order of increasing x-coordinate, and the rows are given in order of increasing y-coordinate. Traversable locations of the grid are indicated by 1, and impassable locations are indicated by 0. Locations are considered to be adjacent horizontally and vertically but not diagonally.

pOutBuffer is where you should store the positions visited in the found path, excluding the starting position but including the final position. Entries in pOutBuffer are indices into pMap. If there is more than one shortest path from Start to Target, any one of them will be accepted.

nOutBufferSize is the maximum number of entries that can be written to pOutBuffer.

Expected Behaviour
The function must return the length of the shortest path between Start and Target, or −1 if no such path exists. If Start and Target are the same position the function should return 0.

If nOutBufferSize is smaller than the size of the path, the test won’t look at pOutBuffer.

Constraints
You may safely assume that:

1≤nMapWidth,nMapHeight,

0≤nStartX,nTargetX<nMapWidth,

0≤nStartY,nTargetY<nMapHeight,

Both Start and Target are empty locations,

nOutBufferSize≥0.

Examples
unsigned char pMap[] = {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};
int pOutBuffer[12];
FindPath(0, 0, 1, 2, pMap, 4, 3, pOutBuffer, 12);
For this input FindPath must return 3, and the first three positions of pOutBuffer must be populated with {1, 5, 9}.

unsigned char pMap[] = {0, 0, 1, 0, 1, 1, 1, 0, 1};
int pOutBuffer[7];
FindPath(2, 0, 0, 2, pMap, 3, 3, pOutBuffer, 7);
For this input FindPath must return −1.

Additional considerations
When we look at the submitted code we will focus on these areas:

Functionality: Does the code complete the task? Does the code crash on edge cases? Could it work in a multi-threaded environment?

Code Quality: How long is the solution? How readable is the code? Is it well structured? Does it have comments in case that’s needed?

Performance: How well does the solution perform? Any other outstanding features?

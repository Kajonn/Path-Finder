#pragma once

//The function does unfortunatly not pass the Paradox test C. I have not been able to find a 
//test map which fails for me so I'm not sure where the bug is.
//The implementation is using the A star algorithm and divides the map into several grids
//to limit search queue sizes.

//
//The meaning of the parameters are as follows.
//
//nStartX and nStartY are the 0 - based coordinates of the start position.
//
//nTargetX and nTargetY are the 0 - based coordinates of the target position.
//
//pMap describes a grid of width nMapWidth and height nMapHeight.The grid is given in row - major order, each row is given in order of increasing x - coordinate, and the rows are given in order of increasing y - coordinate.Traversable locations of the grid are indicated by 1, and impassable locations are indicated by 0. Locations are considered to be adjacent horizontally and vertically but not diagonally.
//
//pOutBuffer is where you should store the positions visited in the found path, excluding the starting position but including the final position.Entries in pOutBuffer are indices into pMap.If there is more than one shortest path from Start to Target, any one of them will be accepted.
//
//nOutBufferSize is the maximum number of entries that can be written to pOutBuffer.
int FindPath(const int nStartX, const int nStartY,
	const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
	int* pOutBuffer, const int nOutBufferSize);
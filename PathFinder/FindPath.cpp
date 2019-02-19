
#include "FindPath.h"
#include "AStarSearch.h"

int FindPath(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY, const unsigned char * pMap, const int nMapWidth, const int nMapHeight, int * pOutBuffer, const int nOutBufferSize)
{
	if (pMap == nullptr)
		return -1;
	if( nMapWidth<1 || nMapHeight<1 ||
		nStartX < 0 || nStartX > nMapWidth ||
		nTargetX < 0 || nTargetX > nMapWidth ||
		nStartY < 0 || nStartY > nMapHeight ||
		nTargetY < 0 || nTargetY > nMapHeight ) {
		return -1;
	}	

	astarsearch::AStarSearch aStarSearch;
	auto pathList = aStarSearch.FindPath(nStartX, nStartY, nTargetX, nTargetY, pMap, nMapWidth, nMapHeight);

	if (pathList.empty()) {
		return -1;
	}
	else {

		int counter = 0;
		for (const int index : pathList) {
			if (pOutBuffer != nullptr) {
				pOutBuffer[counter] = index;
			}
			counter++;
			if (counter >= nOutBufferSize) {
				break;
			}
		}

	}

	return pathList.size();

}

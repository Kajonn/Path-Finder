#pragma once

#include <list>

namespace astarsearch {

	class AStarSearch {
	public:

		std::list<int> FindPath(const int nStartX, const int nStartY,
			const int nTargetX, const int nTargetY,
			const unsigned char* pMap, const int nMapWidth, const int nMapHeight);

	};
}
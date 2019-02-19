
#include <vector>
#include <queue>
#include <map>
#include <list>
#include <chrono>
#include <iostream>
#include <exception>
#include <memory>
#include <cmath>

#include "AStarSearch.h"

namespace astarsearch {

	template<typename MAPNODE> class HeapMap {
	public:
		inline HeapMap(int size) {
			std::make_heap(heap.begin(), heap.end(), std::greater<QueueNode<MAPNODE>>());
			map.resize(size);
		}
		HeapMap() = delete;

		inline MAPNODE &node(int i) {
			return map.at(i);
		}

		inline MAPNODE &top() const {
			return *(heap.front().node);
		}

		inline void pop() {
			std::pop_heap(heap.begin(), heap.end(), std::greater<QueueNode<MAPNODE>>());
			heap.pop_back();
		}

		inline bool empty() const noexcept {
			return heap.empty();
		}

		inline void resort() {
			std::make_heap(heap.begin(), heap.end(), std::greater<QueueNode<MAPNODE>>());
		}

		inline void push_node(int index) {
			heap.push_back(QueueNode<MAPNODE>( map.at(index) ));
			std::push_heap(heap.begin(), heap.end(), std::greater<QueueNode<MAPNODE>>());
		}

	private:
		template<typename T> struct QueueNode
		{
			T* node;

			QueueNode(T&node_) noexcept :
				node(&node_) {}

			QueueNode() = delete;

			inline void operator=(const QueueNode &other) noexcept {
				node = other.node;
			}

			inline bool operator>(const QueueNode &other) const noexcept {
				return *node > *other.node;
			}
		};

		std::vector<MAPNODE> map;
		std::vector<QueueNode<MAPNODE>> heap;
	};

	inline int getIndex(int x, int y, int width, int height) noexcept {
		if (x >= width || y >= height ||
			x < 0 || y < 0)
			return -1;
		else
			return x + (y*width);
	}

	struct Node
	{
		int index;
		int previousIndex;
		float cost;
		float gCost;
		bool isOpen;

		inline Node() noexcept: index(-1), 
			previousIndex(-1), 
			cost(std::numeric_limits<float>::max()), 
			gCost(std::numeric_limits<float>::max()), 
			isOpen(false) {}

		inline bool operator>(const Node &other) const noexcept {
			return cost > other.cost;
		}

		inline bool operator==(const Node &other) const noexcept {
			return index == other.index;
		}
	};
	
	class NodeBucket {

	public:
		inline NodeBucket(int width, int height) :
			heapMap(width*height),
			isOpen(false)
		{
		}
		NodeBucket() = delete;

		inline void operator=(NodeBucket &other) noexcept {
			heapMap = heapMap;
		}

		inline bool operator>(const NodeBucket &other) const noexcept {
			return heapMap.top().cost > other.heapMap.top().cost;
		}

		HeapMap<Node> heapMap;
		bool isOpen;

	};



	class BucketList {
	public:
		BucketList(int width_, int height_, int bucketsWidth_, int bucketsHeight_) :
			width(width_), height(height_), 
			bucketsWidth(std::min(bucketsWidth_, width_)), bucketsHeight(std::min(bucketsHeight_, height_)),
			nBucketsWidth( std::ceil( (float)width / (float)bucketsWidth) ),
			nBucketsHeight(std::ceil( (float)height / (float)bucketsHeight)),
			heapMap(nBucketsWidth*nBucketsHeight) {
		
			currentBucketIndex = -1;			
			currentBucket = nullptr;
		}

		inline Node &getNode(int index) {
			setCurrent(index);
			currentBucket->heapMap.node(getLocalBucketIndex(index)).index = index;
			return currentBucket->heapMap.node(getLocalBucketIndex(index));
		}

		inline void updateNodeCost(int nodeIndex) {
			setCurrent(nodeIndex);
			currentBucket->heapMap.resort();
			heapMap.resort();

		}

		inline void pushNode(int nodeIndex) {
			setCurrent(nodeIndex);
			currentBucket->heapMap.node(getLocalBucketIndex(nodeIndex)).isOpen = true;
			currentBucket->heapMap.push_node(getLocalBucketIndex(nodeIndex));

			if (!currentBucket->isOpen) {
				heapMap.push_node(currentBucketIndex);
				currentBucket->isOpen = true;
			}

			heapMap.resort();

		}

		inline Node &getTopNode() noexcept{
			
			return heapMap.top().nodeBucket->heapMap.top();

		}

		inline void popTop() {

			heapMap.top().nodeBucket->heapMap.top().isOpen = false;
			heapMap.top().nodeBucket->heapMap.pop();
			if (heapMap.top().nodeBucket->heapMap.empty()) {
				heapMap.top().nodeBucket->isOpen = false;
				heapMap.pop();
			}
		}

		inline bool heapEmpty() const noexcept {
			return heapMap.empty();
		}
	private:
		class Bucket {

		public:

			inline Bucket() noexcept {}
			Bucket(const Bucket&) = delete;
			
			inline Bucket(Bucket&&other) noexcept :
				nodeBucket(std::move(other.nodeBucket)) {
			}

			inline void operator=(Bucket &other) noexcept {
				nodeBucket.swap(other.nodeBucket);
			}

			inline bool operator>(const Bucket &other) const noexcept {
				return nodeBucket->heapMap.top().cost > other.nodeBucket->heapMap.top().cost;
			}

			std::unique_ptr< NodeBucket > nodeBucket;

		};

		inline int getLocalBucketIndex(int nodeIndex) const noexcept {
			const int xbucket = ((nodeIndex) % width) / bucketsWidth;
			const int ybucket = (nodeIndex / width) / bucketsHeight;

			const int x = (nodeIndex % width) - (xbucket*bucketsWidth);
			const int y = (nodeIndex / width) - (ybucket*bucketsHeight);

			return getIndex(x, y, bucketsWidth, bucketsHeight);
		}

		inline int getGlobalBucketIndex(int localIndex, int bucket) const noexcept {
			const int xbucket = bucket % nBucketsWidth;
			const int ybucket = bucket / nBucketsWidth;

			const int x = (localIndex % bucketsWidth) + xbucket;
			const int y = (localIndex / bucketsWidth) + ybucket;

			return getIndex(x, y, width, height);
		}

		inline int getBucket(int nodeIndex) const noexcept {
			//get x bucket, y bucket
			const int xbucket = ((nodeIndex) % width)/bucketsWidth;
			const int ybucket = (nodeIndex / width)/bucketsHeight;
			
			return getIndex(xbucket, ybucket, nBucketsWidth, nBucketsHeight);
			
		}

		inline void setCurrent(int nodeindex) {
			const auto bucketIndex = getBucket(nodeindex);
			if (bucketIndex != currentBucketIndex) {
				auto &bucket = heapMap.node(bucketIndex);
				if (!bucket.nodeBucket.get()) {
					bucket.nodeBucket = std::make_unique<NodeBucket>(bucketsWidth, bucketsHeight);
					//for (int i = 0; i < (bucketsHeight*bucketsWidth); i++)
					//{//Init bucket indices
					//	bucket.nodeBucket->heapMap.node(i).index = getGlobalBucketIndex(i, bucketIndex);
					//}

				}

				currentBucketIndex = bucketIndex;
				currentBucket = bucket.nodeBucket.get();
			}
		}

		const int width, height;
		const int bucketsWidth, bucketsHeight;
		const int nBucketsWidth, nBucketsHeight;
		
		int currentBucketIndex;

		HeapMap<Bucket> heapMap;
		
		NodeBucket* currentBucket;
	};




	class AStarSearchMap {

	public:

		AStarSearchMap(const unsigned char * pMap,
			const int nMapWidth, const int nMapHeight,
			const int nTargetX, const int nTargetY,
			const int nStartX, const int nStartY) noexcept:
			_pMap(pMap),
			_nMapWidth(nMapWidth),
			_nMapHeight(nMapHeight),
			_pMapSize(nMapWidth*nMapHeight),
			_target(nTargetX, nTargetY),
			_start(nStartX, nStartY) {}

		AStarSearchMap() = delete;

		inline int getIndex(int x, int y) const noexcept {
			if (x >= _nMapWidth || y >= _nMapHeight ||
				x < 0 || y < 0)
				return -1;
			else
				return x + (y*_nMapWidth);
		}

		inline unsigned char getTileAt(int index) const noexcept {

			if (validIndex(index)) {
				return _pMap[index];
			}
			else {
				return 0; //Outside grid
			}
		}

		inline int mahattanDistance(int index1, int index2) const noexcept {
			const auto coord1 = getCoord(index1);
			const auto coord2 = getCoord(index2);
			return std::abs(coord1.x - coord2.x) + std::abs(coord1.y - coord2.y);
		}

		inline int mahattanDistanceToTarget(int index1) const noexcept {
			const auto coord1 = getCoord(index1);
			return std::abs(coord1.x - _target.x) + std::abs(coord1.y - _target.y);
		}

		inline bool validIndex(int index) const noexcept  {
			return index >= 0 && index < _pMapSize;
		}

		struct Coord {
			int x;
			int y;

			Coord(int x_, int y_) noexcept : x(x_), y(y_) {}
		};


		inline Coord getCoord(int index) const noexcept {
			return Coord(index%_nMapWidth, index / _nMapWidth);
		}

		inline int getIndex(const Coord &coord) const noexcept {
			return getIndex(coord.x, coord.y);
		}
	private:


		const unsigned char * _pMap;
		const int _nMapWidth;
		const int _nMapHeight;
		const int _pMapSize;
		const Coord _target;
		const Coord _start;
	};




	class AStarSearchImpl {
	public:
		AStarSearchImpl(const unsigned char * pMap,
			const int nMapWidth, const int nMapHeight,
			const int nTargetX, const int nTargetY,
			const int nStartX, const int nStartY,
			const int bucketWidth, const int bucketHeight, float hScoreScale) :
			searchMap(pMap, nMapWidth, nMapHeight, nTargetX, nTargetY, nStartX, nStartY),
			nodeBuckets(nMapWidth, nMapHeight, bucketWidth, bucketHeight),
			_hScoreScale(hScoreScale)
		{
		}

		void checkNeighbour(int parent, int neighbourIndex) {
			
			if (neighbourIndex >= 0 && searchMap.getTileAt(neighbourIndex) == 1) {

				const Node &parentNode = nodeBuckets.getNode(parent);
				Node &neighbourNode = nodeBuckets.getNode(neighbourIndex);

				const float gScore = static_cast<float>(nodeBuckets.getNode(parent).gCost) + 1.0f;

				if (gScore < neighbourNode.gCost) {

					const float hScore = static_cast<float>(searchMap.mahattanDistanceToTarget(neighbourIndex))*_hScoreScale;
					const float totalCost = hScore + gScore;

					neighbourNode.previousIndex = parent;
					neighbourNode.cost = totalCost;
					neighbourNode.gCost = gScore;

					if (neighbourNode.isOpen) {
						//Need to update heap as score has changed
						nodeBuckets.updateNodeCost(neighbourIndex);
					}
					else {
						nodeBuckets.pushNode(neighbourIndex);
					}
				}
			}
		}

		std::list<int> getPath(
			const int nTargetX, const int nTargetY,
			const int nStartX, const int nStartY) {

			std::list<int> path;
			
			const auto startIndex = searchMap.getIndex(nStartX, nStartY);
			const auto targetIndex = searchMap.getIndex(nTargetX, nTargetY);

			int currentIndex = startIndex;
			auto &node = nodeBuckets.getNode(startIndex);
			node.index = startIndex;
			node.isOpen = true;
			node.previousIndex = startIndex;
			node.cost = 0;
			node.gCost = 0;
			nodeBuckets.pushNode(startIndex);

			while (!nodeBuckets.heapEmpty()) {

				Node &bestNode = nodeBuckets.getTopNode();
				nodeBuckets.popTop();

				currentIndex = bestNode.index;
				if (currentIndex == targetIndex)
					break;
								
				const AStarSearchMap::Coord currentCoord = searchMap.getCoord(bestNode.index);

				checkNeighbour(bestNode.index, searchMap.getIndex(currentCoord.x + 1, currentCoord.y));
				checkNeighbour(bestNode.index, searchMap.getIndex(currentCoord.x - 1, currentCoord.y));
				checkNeighbour(bestNode.index, searchMap.getIndex(currentCoord.x, currentCoord.y + 1));
				checkNeighbour(bestNode.index, searchMap.getIndex(currentCoord.x, currentCoord.y - 1));
				
			}

			if (currentIndex == targetIndex) {
				
				int index = currentIndex;
				do {
					path.push_front(index);

					index = nodeBuckets.getNode(index).previousIndex;
				} while (index != startIndex);
			}

			return path;
		}

	private:
		const AStarSearchMap searchMap;
		BucketList nodeBuckets;
		const float _hScoreScale;

	};


	std::list<int> AStarSearch::FindPath(const int nStartX, const int nStartY,
		const int nTargetX, const int nTargetY,
		const unsigned char* pMap, const int nMapWidth, const int nMapHeight)
	{

		std::list<int> path;
		const double mapSize = static_cast<double>(nMapWidth) * static_cast<double>(nMapHeight);
		if (mapSize > (double)std::numeric_limits<int>::max()) {
			std::cerr << "FindPath aborted. Map index larger than max int." << std::endl;
			return path;
		}

		try {

			AStarSearchImpl astarSearchImpl(pMap, nMapWidth, nMapHeight, 
				nTargetX, nTargetY, 
				nStartX, nStartY,
				30,30,1.0f);
			return astarSearchImpl.getPath(nTargetX, nTargetY, nStartX, nStartY);

		}
		catch (const std::exception &e) {
			std::cerr << "Exception thrown during compute path: " << std::endl << e.what() << std::endl;
		}
		catch (const std::string &e) {
			std::cerr << "Exception thrown during compute path: " << std::endl << e.c_str() << std::endl;
		}
		catch (...) {
			std::cerr << "Unknown exception thrown during compute path. " << std::endl;
		}

		return path;
	}

}
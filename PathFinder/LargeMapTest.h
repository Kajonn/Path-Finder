#pragma once

#include <vector>

#include "FindPath.h"

class TestMap {
public:
	TestMap(int width_, int height_) :
		width(width_), height(height_) {
		testMap.resize(width*height);
		std::fill(testMap.begin(), testMap.end(),1);
	}

	inline int getIndex(int x, int y) const {
		return x + (y*width);
	}

	inline void set(int x, int y, char value) {
		testMap[getIndex(x,y)] = value;
	}

	inline char get(int x, int y) const {
		return testMap[getIndex(x, y)];
	}

	int width;
	int height;
	std::vector<unsigned char> testMap;

};

void TestWorstCasePath(int width, int height, bool print = false);

#include "LargeMapTest.h"
#include <iostream>

void setLine(TestMap &map, int startx, int starty, int endx, int endy, int value) {
	for (size_t i = startx; i <= endx; i++)
	{
		for (size_t j = starty; j <= endy; j++)
		{
			map.set(i, j, value);
		}
	}
}


void setRect(TestMap &map, int startx, int starty, int endx, int endy, int value) {

	for (size_t i = startx; i <= endx; i++)
	{
		for (size_t j = starty; j <= endy; j++)
		{
			map.set(i, j, value);
		}
	}
}

void TestWorstCasePath(int width, int height, bool print) {

	TestMap testMap(width, height);

	//Build walls
	//setRect(testMap, 1, height-2, width-2, height-2, 0);
	//setRect(testMap, width - 2, 1, width -2 , height - 2, 0);

	setRect(testMap, 0, height-2, width - 2, height -2, 0);
	setRect(testMap, 0, height/3, width - 2, height/3, 0);
	//setRect(testMap, 1, 1, 1, height - 2, 0);
	//for (size_t i = width / 2 - 10; i < width /2 + 10; i++)
	//{
	//	testMap.set(i, height/3, 0);
	//	testMap.set(i, height-height/3, 0);

	//	//testMap.set(width-2, i, 0);
	//}

	//for (size_t i = height / 2; i < height - 1; i++)
	//{
	//	testMap.set(i, (height/3), 0);
	//	testMap.set(i + 1 , height/2, 0);
	//}

	//const int startX = 0;
	//const int startY = 0;
	//const int endX = width-1;
	//const int endY = height-1;

	const int startX = 0;
	const int startY = height-1;
	const int endX = 0;
	const int endY = 0;


	if (width <= 100 && print) {
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				if (y == startY && x == startX)
					std::cout << "S";
				else if (y == endY && x == endX)
					std::cout << "T";
				else
					std::cout << testMap.get(x, y);
			}
			std::cout << std::endl;
		}
	}

	const int pOutBuffer1Size = 1000;
	std::vector<int> pOutBuffer1;
	pOutBuffer1.resize(pOutBuffer1Size);
	const int foundPathLength = FindPath(startX, startY, endX, endY, testMap.testMap.data(), width, height, pOutBuffer1.data(), pOutBuffer1Size);


	if (foundPathLength == -1) {
		std::cout << "findPath == -1" << std::endl;
	} else {
		if (width <= 100 && print) {
			std::cout << std::endl;
			for (size_t y = 0; y < height; y++)
			{
				for (size_t x = 0; x < width; x++)
				{
					bool foundPath = false;
					for (size_t p = 0; p < foundPathLength; p++)
					{
						auto index = pOutBuffer1[p];
						if (index % width == x &&
							index / width == y) {
							std::cout << 'X';
							foundPath = true;
						}
					}
					if (!foundPath) {
						if (y == startY && x == startX)
							std::cout << "S";
						else if (y == endY && x == endX)
							std::cout << "T";
						else 
							std::cout << testMap.get(x, y);
					}
				}
				std::cout << std::endl;
			}
		}

		std::cout << "Width: " << width << " height: " << height << std::endl;
		std::cout << "Found path with length: " << foundPathLength << std::endl;

	}
}
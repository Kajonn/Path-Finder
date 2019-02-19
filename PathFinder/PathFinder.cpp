
#include "FindPath.h"
#include "LargeMapTest.h"
#include <iostream>
#include <thread>
#include <list>

void test() {
	std::cout << test << std::endl;
}

int main()
{
	unsigned char pMap1[] = { 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1 };
	int pOutBuffer1[12];
	int findPath1 = FindPath(0, 0, 1, 2, pMap1, 4, 3, pOutBuffer1, 12);

	if (findPath1 != 3) {
		std::cout << "findPath1 != 3" << std::endl;
		exit(1);
	}
	int index = 0;
	for (int i : {1, 5, 9}) {
		if (i != pOutBuffer1[index]) {
			std::cout << i << " != " << pOutBuffer1[index] << std::endl;
			exit(1);
		}
		index++;
	}

	unsigned char pMap2[] = { 0, 0, 1, 0, 1, 1, 1, 0, 1 };
	int pOutBuffer2[7];
	int findPath2 = FindPath(2, 0, 0, 2, pMap2, 3, 3, pOutBuffer2, 7);
	if (findPath2 != -1) {
		std::cout << "1 findPath1 != -1" << std::endl;
		exit(1);
	}

	if (FindPath(2, 0, 2, 0, pMap2, 3, 3, pOutBuffer2, 7) != 1) {
		std::cout << "end = start != 1" << std::endl;
		exit(1);
	}

	if (FindPath(2, 0, 2, 1, pMap2, 3, 3, pOutBuffer2, 7) != 1) {
		std::cout << "end = start+1 != 1" << std::endl;
		exit(1);
	}

	unsigned char pMap3[] = { 1, 1 };
	int pOutBuffer3[7];
	int findPath3 = FindPath(0, 0, 1, 0, pMap3, 2, 1, pOutBuffer3, 7);
	if (findPath3 == -1) {
		std::cout << "2 findPath1 != -1" << std::endl;
		exit(1);
	}

	int findPath4 = FindPath(0, 0, 1, 0, nullptr, 2, 1, pOutBuffer3, 7);
	if (findPath4 != -1) {
		std::cout << "3 findPath1 != -1" << std::endl;
		exit(1);
	}

	if (FindPath(0, 0, 0, 0, nullptr, 0, 0, nullptr, 0) != -1) {
		std::cout << "map null ptr != -1" << std::endl;
		exit(1);
	}

	if (FindPath(0, 0, 0, 0, pMap3, 2, 1, nullptr, 0) == -1) {
		std::cout << "out nullptr == -1" << std::endl;
		exit(1);
	}

	if (FindPath(0, 0, 0, 0, pMap3, std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), nullptr, 0) != -1) {
		std::cout << "int max != -1" << std::endl;
		exit(1);
	}

	if (FindPath(0, 0, 99, 99, pMap3, 100, 100, nullptr, 0) != -1) {
		std::cout << "pmap smaller != -1" << std::endl;
		exit(1);
	}

	unsigned char pMapWrong[] = { 0, 0, 2, 0, 1, 1, 1, 0, 1 };
	if (FindPath(0, 0, 1, 0, pMapWrong, 3, 3, nullptr, 0) != -1) {
		std::cout << "pmapWrong != -1" << std::endl;
		exit(1);
	}


	//std::vector<std::thread> threads;
	//for (size_t i = 0; i < 1; i++)
	//{
	//	std::thread thread(TestWorstCasePath,50+(i*2), 20+i, false);
	//	threads.push_back(std::move(thread));
	//}

	//for (std::thread &th : threads) {
	//	th.join();
	//}

	TestWorstCasePath(10, 10, true);
	TestWorstCasePath(20, 20);
	TestWorstCasePath(100, 10, true);
	TestWorstCasePath(60, 60);
	TestWorstCasePath(100, 100);
	TestWorstCasePath(200, 200);
	TestWorstCasePath(300, 300);
	TestWorstCasePath(400, 400);
	TestWorstCasePath(500, 500);
	TestWorstCasePath(600, 600);
	TestWorstCasePath(800, 800);
	TestWorstCasePath(1000, 1000);
	//TestWorstCasePath(1500, 1500);
	//TestWorstCasePath(5000, 5000);

	std::cout << "Completed!" << std::endl;
	std::getchar();
}

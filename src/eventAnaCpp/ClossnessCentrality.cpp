#include "../../include/eventAnalysis/EventAnalysis.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

bool EventAnalysis::closenessCentrality()
{
	const std::string srcFilePref = BasicData::SrcEventWithTimePrefix;
	const std::string tarFilePref = BasicData::TargetAnaResPrefix;

	//const std::vector<std::string> testVec = { "testOne.txt" };
	//for (unsigned i = 0; i < testVec.size(); ++i) {

	for (unsigned i = 1; i < BasicData::VecSrcEventFiles.size(); ++i) {
		std::cout << "handling file: " << BasicData::VecSrcEventFiles.at(i) << std::endl;		/////////////

		if (buildNetworkOne(srcFilePref + BasicData::VecSrcEventFiles.at(i)) == false) {
			std::cout << "build network error: " << BasicData::VecSrcEventFiles.at(i) << std::endl;
			return false;
		}
		
		//data we need
		std::cout << "calculate each event node number begin.." << std::endl;
		unsigned eachEventNodeNum = 0;
		if (calEachEventNodeNum(eachEventNodeNum) == false) {
			std::cerr << "cal each event node number error." << std::endl;
			return false;
		}

		std::cout << "calculate each event closs centra begin.." << std::endl;		/////////////
		std::vector<std::pair<unsigned, double>> resVec;
		if (calEachEventClossCentra(eachEventNodeNum, resVec) == false) {
			std::cerr << "cal each event closs centrality." << std::endl;
			return false;
		}

		std::cout << "write file begin.." << std::endl;			///////////////////////
		if (writeFileClosCentra(tarFilePref, i, resVec) == false) {
			std::cerr << "write file closs centra error." << std::endl;
			return false;
		}
		clearNetworkOne();
	}

	return true;
}
bool EventAnalysis::calEachEventNodeNum(unsigned &nodeNum)
{
	const unsigned maxNodeNum = networkOne.getCapacity();
	unsigned totalNodeNum = 0;
	for (unsigned i = 1; i <= maxNodeNum; ++i) {
		unsigned nodeDegree = 0;
		if (networkOne.getDegreeAsUndi(i, nodeDegree) == false) {
			std::cerr << "get degree error at: " << i << std::endl;
			return false;
		}
		if (nodeDegree != 0)
			totalNodeNum++;
	}
	nodeNum = totalNodeNum;
	return true;
}
bool EventAnalysis::calEachEventClossCentra(const unsigned eachEventNodeNum, std::vector<std::pair<unsigned, double>> &resVec)
{
	const unsigned maxNodeNum = networkOne.getCapacity();

	//std::cout << "max node num = " << maxNodeNum << std::endl;///////////////

	for (unsigned i = 1; i <= maxNodeNum; ++i) {
		if (i % 10000 == 0)
			std::cout << "each node closs centra i = " << i << std::endl;		// test.

		unsigned nodeDegree = 0;
		if (networkOne.getDegreeAsUndi(i, nodeDegree) == false) {
			std::cerr << "get degree error at: " << i << std::endl;
			return false;
		}
		if (nodeDegree == 0)
			continue;

		unsigned sumDis = 0;
		if (calNodeSumDis(i, sumDis) == false) {
			std::cerr << "cal node sum dis error at: " << i << std::endl;
			return false;
		}

		// 处理溢出，分母判定下
		if (sumDis == 0) {
			std::cout << "sum dis = 0, i = " << i << std::endl;
			unsigned degree = 0;
			if (networkOne.getDegreeAsUndi(i, degree) == false) {
				std::cerr << "get degree error at: " << i << std::endl;
				return false;
			}
			std::cout << "degree = " << degree << std::endl;
			getchar();
			continue;
		}

		double closCent = static_cast<double>(eachEventNodeNum) / static_cast<double>(sumDis);
		resVec.push_back(std::make_pair(i, closCent));
	}

	return true;
}
/*
 *	bfs 计算路径长度
 */
bool EventAnalysis::calNodeSumDis(const unsigned startNode, unsigned &sumDis)
{
	const unsigned maxNodeNum = networkOne.getCapacity();
	
	//std::vector<unsigned> visitVec = std::vector<unsigned>(maxNodeNum + 1);
	//std::vector<unsigned> pathVec = std::vector<unsigned>(maxNodeNum + 1);

	int *visitVec;
	int *pathVec;

	try {
		visitVec = new int[maxNodeNum + 1];
		pathVec = new int[maxNodeNum + 1];
	}
	catch (std::exception e){
		e.what();
		return false;
	}

	//visitVec.at(startNode) = 1, pathVec.at(startNode) = 0;
	visitVec[startNode] = 1;
	pathVec[startNode] = 0;

	std::queue<unsigned> visitNodeQue;
	visitNodeQue.push(startNode);

	while (!visitNodeQue.empty()) {
		unsigned u = visitNodeQue.front();
		visitNodeQue.pop();

		// 处理 u 的邻居
		std::vector<unsigned> ajaVec;
		if (networkOne.getAjaOfNodeFromHoriCroLink(u, ajaVec) == false) {
			std::cerr << "get aja of node from hori error: " << u << std::endl;
			return false;
		}

		std::vector<unsigned> ajaReVec;
		if (networkOne.getAjaOfNodeFromVertCroLink(u, ajaReVec) == false) {
			std::cerr << "get aja of node from vert error: " << u << std::endl;
			return false;
		}
		
		for (const auto &elem : ajaVec)
			if (visitVec[elem] == 0) {
				visitVec[elem] = 1;
				visitNodeQue.push(elem);
				pathVec[elem] = pathVec[u] + 1;
			}
			
		for (const auto &elem : ajaReVec) 
			if (visitVec[elem] == 0) {
				visitVec[elem] = 1;
				visitNodeQue.push(elem);
				pathVec[elem] = pathVec[u] + 1;
			}
	}

	unsigned sumPath = 0;
	for (unsigned i = 1; i <= maxNodeNum; ++i)
		sumPath += pathVec[i];

	sumDis = sumPath;

	free(visitVec);
	free(pathVec);

	return true;
}

bool EventAnalysis::writeFileClosCentra(const std::string &tarFilePref, const unsigned eventId,
				const std::vector<std::pair<unsigned, double>> &resVec)
{
	std::stringstream ss;
	ss << "clossCentra_new_";
	ss << eventId;

	std::string fileName;
	ss >> fileName;

	std::fstream outputFile(tarFilePref + fileName, std::ios_base::out);
	if (!outputFile.is_open()) {
		std::cerr << "open output file error." << std::endl;
		return false;
	}

	for (const auto &elem : resVec)
		outputFile << elem.first << "\t" << elem.second << "\n";

	outputFile.close();

	return true;
}
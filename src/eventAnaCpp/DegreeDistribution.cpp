#include "../../include/eventAnalysis/EventAnalysis.h"

#include <iostream>
#include <fstream>
#include <set>
#include <sstream>

/* 
 *	计算 user 网络的度分布
 */

bool EventAnalysis::userDegreeDistribution()
{
	// 计算 user 网络的度分布

	const std::string srcFilePref = BasicData::SrcEventWithTimePrefix;
	const std::string tarFilePref = BasicData::TargetAnaResPrefix;

	const std::string userFileName = "user_k_core_src_network";
	//const std::string eventFileName = "aggregated_event_network";

	std::cout << "process user file: " << srcFilePref + userFileName << std::endl;
	
	if (buildNetworkOne(srcFilePref + userFileName) == false) {
		std::cerr << "build network error. " << std::endl;
		return false;
	}

	const unsigned nodeMax = networkOne.getCapacity();
	std::vector<unsigned> degreeVec = std::vector<unsigned>(nodeMax + 1);

	if (userCalculateEachNodeDegree(degreeVec) == false) {
		std::cerr << "calculate each node degree error." << std::endl;
		return false;
	}

	if (userDegreeDisWriteFileDegreeCentra(tarFilePref, degreeVec) == false) {
		std::cerr << "write file error." << std::endl;
		return false;
	}

	clearNetworkOne();

	return true;
}
/*
*	数据中存在这种自己到自己的转发，因此计算度的时候要区分一下
*	310	310	2010	8	20	23	41	7
*/
bool EventAnalysis::userCalculateEachNodeDegree(std::vector<unsigned> &degreeVec)
{
	const unsigned nodeMax = networkOne.getCapacity();
	for (unsigned i = 1; i <= nodeMax; ++i) {
		unsigned nodeDegree = 0;

		std::vector<unsigned> ajaVec;
		if (networkOne.getAjaOfNodeFromHoriCroLink(i, ajaVec) == false) {
			std::cerr << "get aja of node from hori error: " << i << std::endl;
			return false;
		}

		std::vector<unsigned> ajaReVec;
		if (networkOne.getAjaOfNodeFromVertCroLink(i, ajaReVec) == false) {
			std::cerr << "get aja of node from vert error: " << i << std::endl;
			return false;
		}

		nodeDegree += ajaVec.size();
		nodeDegree += ajaReVec.size();

		degreeVec.at(i) = nodeDegree;
	}

	return true;
}
bool EventAnalysis::userDegreeDisWriteFileDegreeCentra(const std::string &tarFilePref, const std::vector<unsigned> &degreeVec)
{
	std::stringstream ss;
	ss << "user_degree_dis";

	std::string fileName;
	ss >> fileName;

	std::fstream outputFile(tarFilePref + fileName, std::ios_base::out);
	if (!outputFile.is_open()) {
		std::cerr << "open output file error." << std::endl;
		return false;
	}

	unsigned notNullNode = 0;
	double averageDegree = 0.0;

	const unsigned maxNode = networkOne.getCapacity();

	for (unsigned i = 1; i <= maxNode; ++i)
		if (degreeVec.at(i) != 0)
			notNullNode++;

	for (unsigned i = 1; i <= maxNode; ++i) {
		if (degreeVec.at(i) == 0)
			continue;
		
		outputFile << i << "\t" << degreeVec.at(i) << "\n";
		averageDegree += static_cast<double>(degreeVec.at(i)) / static_cast<double>(notNullNode);
	}

	//std::cout << "average degree = " << averageDegree <<  std::endl;
	//std::cout << "notNullNode = " << notNullNode << std::endl;
	//getchar();

	outputFile << "average degree = " << averageDegree << ", not null node = " << notNullNode << std::endl;

	outputFile.close();
	return true;
}
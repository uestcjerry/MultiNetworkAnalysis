#include "../../include/eventAnalysis/EventAnalysis.h"

#include <iostream>
#include <fstream>
#include <set>
#include <sstream>

/*
 *	度中心性排序
 */

bool EventAnalysis::degreeCentrality()
{
	const std::string srcFilePref = BasicData::SrcEventWithTimePrefix;
	const std::string tarFilePref = BasicData::TargetAnaResPrefix;

	for (unsigned i = 0; i < BasicData::VecSrcEventFiles.size(); ++i) {
		std::cout << "handling file: " << BasicData::VecSrcEventFiles.at(i) << std::endl;		/////////////

		if (buildNetworkOne(srcFilePref + BasicData::VecSrcEventFiles.at(i)) == false) {
			std::cout << "build network error: " << BasicData::VecSrcEventFiles.at(i) << std::endl;
			return false;
		}

		// data we need
		const unsigned nodeMax = networkOne.getCapacity();
		std::vector<unsigned> degreeVec = std::vector<unsigned>(nodeMax + 1);
		
		unsigned eachEventNodeNum = 0;			// 事件中节点个数

		std::cout << "calculate node degree begin.." << std::endl;	///////////
		if (calculateEachNodeDegree(degreeVec) == false) {
			std::cerr << "calculate each node degree error." << std::endl;
			return false;
		}

		std::cout << "calculate node number begin.." << std::endl;	////////////
		if (calculateEventNodeNumber(degreeVec, eachEventNodeNum) == false) {
			std::cerr << "calculate event node number error." << std::endl;
			return false;
		}

		std::cout << "write file begin.." << std::endl;	////////////
		
		if (writeFileDegreeCentra(tarFilePref, i, degreeVec, eachEventNodeNum) == false) {
			std::cerr << "write file degree centrality error." << std::endl;
			return false;
		}

		if (writeFileDegreeDis(tarFilePref, i, degreeVec) == false) {
			std::cerr << "write file degree distribution error." << std::endl;
			return false;
		}

		clearNetworkOne();
	}


	return true;
}
/*
 *	数据中存在这种自己到自己的转发，因此计算度的时候要区分一下
 *	310	310	2010	8	20	23	41	7
 */
bool EventAnalysis::calculateEachNodeDegree(std::vector<unsigned> &degreeVec)
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
		
		for (const auto &elem : ajaVec)
			if (elem != i)
				nodeDegree++;

		for (const auto &elem : ajaReVec)
			if (elem != i)
				nodeDegree++;

		//if (networkOne.getDegreeAsUndi(i, nodeDegree) == false) {
		//	std::cerr << "get degree error at: " << i << std::endl;
		//	return false;
		//}
		degreeVec.at(i) = nodeDegree;
	}

	return true;
}
bool EventAnalysis::calculateEventNodeNumber(const std::vector<unsigned> &degreeVec, unsigned &nodeNum)
{
	unsigned totalNum = 0;
	const unsigned nodeMax = networkOne.getCapacity();

	for (unsigned i = 1; i <= nodeMax; ++i)
		if (degreeVec.at(i) != 0)
			totalNum++;
	nodeNum = totalNum;

	return true;
}

/*
 *	度中心性结果
 */
bool EventAnalysis::writeFileDegreeCentra(const std::string &tarFilePref, const unsigned eventId,
					const std::vector<unsigned> &degreeVec, const unsigned eventNodeNum)
{
	std::stringstream ss;
	ss << "event";
	ss << eventId;

	std::string fileName;
	ss >> fileName;

	std::fstream outputFile(tarFilePref + fileName, std::ios_base::out);
	if (!outputFile.is_open()) {
		std::cerr << "open output file error." << std::endl;
		return false;
	}

	const unsigned maxNode = networkOne.getCapacity();
	for (unsigned i = 1; i <= maxNode; ++i) {
		if (degreeVec.at(i) == 0)
			continue;
		double dc = static_cast<double>(degreeVec.at(i)) / static_cast<double>(eventNodeNum - 1);
		
		outputFile << i << "\t" << dc << "\n";
	}

	outputFile.close();

	return true;
}
/*
 *	度分布结果
 */
bool EventAnalysis::writeFileDegreeDis(const std::string &tarFilePref, const unsigned eventId, const std::vector<unsigned> &degreeVec)
{
	const unsigned nodeMax = networkOne.getCapacity();

	std::stringstream ss;
	ss << "degreeDist_";
	ss << eventId;

	std::string fileName;
	ss >> fileName;

	std::fstream outputFile(tarFilePref + fileName, std::ios_base::out);
	if (!outputFile.is_open()) {
		std::cerr << "open output file error. at: " << tarFilePref + fileName << std::endl;
		return false;
	}

	for (unsigned i = 1; i <= nodeMax; ++i)
		if (degreeVec.at(i) > 0)
			outputFile << i << "\t" << degreeVec.at(i) << "\n";

	outputFile.close();

	return true;
}
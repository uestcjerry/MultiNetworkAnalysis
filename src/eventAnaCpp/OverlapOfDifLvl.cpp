#include "../../include/basicDef/BasicData.h"
#include "../../include/eventAnalysis/EventAnalysis.h"

#include <iostream>
#include <fstream>
#include <algorithm>

// ========================== Overlapping of different level (directed edge) ================================== //

/*
*	output format:
*	result commonNumber union-common eventI eventJ
*/
bool EventAnalysis::anaOverlapOfDifLevl()
{
	std::cout << "\n analysis overlapping of different level begin.." << std::endl;
	std::vector<std::pair<double, std::vector<unsigned>>> resultVec;

	std::string srcFilePrefix = BasicData::SrcEventWithTimePrefix;

	for (unsigned i = 0; i < BasicData::VecSrcEventFiles.size(); ++i) {
		std::cout << "\n first for: " << BasicData::VecSrcEventFiles.at(i) << std::endl;	/////////////

		if (buildNetworkOne(srcFilePrefix + BasicData::VecSrcEventFiles.at(i)) == false) {
			std::cerr << "build network one error." << std::endl, getchar();
			clearNetworkOne();
			return false;
		}
		for (unsigned j = i + 1; j < BasicData::VecSrcEventFiles.size(); ++j) {
			std::cout << "\n second for: " << BasicData::VecSrcEventFiles.at(j) << std::endl;	//////////////

			if (buildNetworkTwo(srcFilePrefix + BasicData::VecSrcEventFiles.at(j)) == false) {
				std::cerr << "build network two error." << std::endl, getchar();
				clearNetworkTwo();
				return false;
			}
			//找公共点，存set
			//求 A jiao B, 存边
			//判断其余的边时，只要有一个点不在commonset中，那么它就是(A bing B) - (A jiao B)的边
			// let write
			std::vector<unsigned> commonVec;
			if (getCommonNode(commonVec) == false) {
				std::cerr << "\n get common node error." << std::endl;
				return false;
			}
			std::set<unsigned> commonSet;			// for search
			for (const auto &elem : commonVec)
				commonSet.insert(elem);

			long long commonNumber = 0, unionNumber = 0;
			if (calCommonNumber(commonVec, commonSet, commonNumber) == false) {			//计算A jiao B
				std::cerr << "cal common number error." << std::endl;
				return false;
			}

			if (calUnionNumber(networkOne, commonSet, unionNumber) == false) {		//计算(A bing B)-(A jiao B)
				std::cerr << "cal union network one error." << std::endl;
				return false;
			}
			if (calUnionNumber(networkTwo, commonSet, unionNumber) == false) {
				std::cerr << "cal union network two error." << std::endl;
				return false;
			}
			double resultVal = (double)commonNumber / (double)(commonNumber + unionNumber);
			unsigned resultArr[] = { commonNumber, unionNumber, i, j };
			resultVec.push_back(std::make_pair(resultVal, std::vector<unsigned>(resultArr, resultArr + sizeof(resultArr) / sizeof(unsigned))));

			std::cout << "\n clearNetworkTwo() begin.." << std::endl;
			clearNetworkTwo();
			std::cout << "\n clearNetworkTwo() finish.." << std::endl;
		}

		std::cout << "\n clearNetworkOne() begin.." << std::endl;
		clearNetworkOne();
		std::cout << "\n clearNetworkOne() finish.." << std::endl;

	}

	std::fstream outpufFile(BasicData::TargetAnaResPrefix + "OverlapOfDifferentLevel.result", std::ios_base::out);
	if (!outpufFile.is_open()) {
		std::cerr << "open output file error." << std::endl;
		return false;
	}
	// write to file
	std::sort(resultVec.begin(), resultVec.end(),
		[](const std::pair<double, std::vector<unsigned>> &a, const std::pair<double, std::vector<unsigned>> &b) -> bool
	{	return a.first < b.first; }
	);

	outpufFile << "% Overlapping of different level" << "\n";
	outpufFile << "% output format: result commonNumber union-common eventI eventJ" << "\n";

	for (const auto &elem : resultVec) {
		outpufFile << elem.first << "\t";
		for (const auto &seg : elem.second)
			outpufFile << seg << "\t";
		outpufFile << "\n";
	}

	outpufFile.close();

	std::cout << "\n analysis overlapping of different level finish.." << std::endl;
	std::cout << "\n Press any key to continue.." << std::endl;
	getchar();
	return true;
}
/*
*	calculate commonNumber
*/
template<class T> bool searchVector(const std::vector<T> &vec, const T value)
{
	auto it = vec.begin();
	while (it != vec.end())
		if ((*it) != value)
			it++;
		else
			break;
	if (it != vec.end()) return true;
	else return true;
}
bool EventAnalysis::calCommonNumber(std::vector<unsigned> &commonVec, std::set<unsigned> &commonSet, long long &commonNumber)
{
	std::cout << "cal common number begin.." << std::endl;

	for (const auto &i : commonVec) {
		std::vector<unsigned> lvOneAja;
		if (networkOne.getAjaOfNodeFromHoriCroLink(i, lvOneAja) == false) {
			std::cerr << "get aja of node from hori error." << std::endl;
			return false;
		}
		std::vector<unsigned> lvTwoAja;
		if (networkTwo.getAjaOfNodeFromHoriCroLink(i, lvTwoAja) == false) {
			std::cerr << "get aja of node from hori error." << std::endl;
			return false;
		}
		for (const auto &j : lvOneAja) {
			if (searchVector<unsigned>(lvTwoAja, j) && commonSet.find(j) != commonSet.end())
				commonNumber++;
		}
	}
	std::cout << "cal common number finish.." << std::endl;
	return true;
}
/*
*	calculate unionNumber
*	result = (A bing B) - (A jiao B)
*/
bool EventAnalysis::calUnionNumber(CrossLink &network, std::set<unsigned> &commonSet, long long &unionNumber)
{
	std::cout << "cal union number begin.." << std::endl;

	unsigned capaOfOne = network.getCapacity();
	for (unsigned i = 1; i <= capaOfOne; ++i) {
		if (commonSet.find(i) != commonSet.end())
			continue;
		unsigned ajaSize = 0;
		if (network.getAjaSizeFromHori(i, ajaSize) == false)
			return false;
		if (ajaSize == 0)
			continue;
		std::vector<unsigned> ajaOfNode;
		if (network.getAjaOfNodeFromHoriCroLink(i, ajaOfNode) == false)
			return false;
		for (const auto &j : ajaOfNode)
			if (commonSet.find(j) != commonSet.end())
				unionNumber++;
	}
	std::cout << "cal union number finish.." << std::endl;
	return true;
}

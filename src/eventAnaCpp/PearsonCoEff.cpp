#include "../../include/eventAnalysis/EventAnalysis.h"
#include "../../include/basicDef/BasicData.h"

#include <iostream>
#include <map>
#include <fstream>

// ================================== Pearson co-efficiency ====================================== //
/*
*	output format:
*	result eventI eventJ
*/
bool EventAnalysis::anaDegreeRelaBetLevl()
{
	std::cout << "\n analysis degree relation level begin.." << std::endl;

	std::multimap<double, std::pair<unsigned, unsigned>> resultMap;

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

			// get common node
			std::cout << "\n get common node begin.." << std::endl;		//////////

			std::vector<unsigned> commonVec;										//vector遍历
			if (getCommonNode(commonVec) == false) {
				std::cerr << "get common node error." << std::endl, getchar();
				return false;
			}
			if (commonVec.size() == 0) {
				std::cout << "commonVec size == 0, continue.." << std::endl;
				clearNetworkTwo();
				continue;
			}

			std::set<unsigned> commonSet;											//set方便查找
			for (const auto &u : commonVec)
				commonSet.insert(u);

			// calculate degree
			std::cout << "\n calculate degree begin.." << std::endl;	     //////////////	

			std::vector<std::pair<unsigned, unsigned>> levelA;				// node id -> degree (only common node reserve)
			std::vector<std::pair<unsigned, unsigned>> levelB;				// after getDegree() , both map size are equal
			if (getDegreeOfDifLevlFromCommonVec(commonVec, commonSet, levelA, levelB) == false) {
				return false;
			}

			// calculate Pearson Coefficiency
			std::cout << "\n calculate Pearson Coefficiency begin.." << std::endl;
			double averageA = calAverageFromMap(levelA);
			double averageB = calAverageFromMap(levelB);

			double child = 0.0, mother = 0.0;					// pearson element

			if (levelA.size() != levelB.size()) {
				std::cerr << "error : levelA.size() != levelB.size()" << std::endl, getchar();
				return false;
			}
			for (unsigned k = 0; k < levelA.size() && k < levelB.size(); ++k)
				child += ((double)levelA.at(k).second - averageA) * ((double)levelB.at(k).second - averageB);

			double leftMother = 0.0, rightMother = 0.0;
			for (unsigned k = 0; k < levelA.size(); ++k)
				leftMother += pow(((double)levelA.at(k).second - averageA), 2.0);
			leftMother = sqrt(leftMother);

			for (unsigned k = 0; k < levelB.size(); ++k)
				rightMother += pow(((double)levelB.at(k).second - averageB), 2.0);
			rightMother = sqrt(rightMother);

			mother = leftMother * rightMother;

			double pearsonCoefficiency = child / mother;

			// write file
			std::cout << "\n write file begin.." << std::endl;

			resultMap.insert(std::make_pair(pearsonCoefficiency, std::make_pair(i, j)));

			std::cout << "\n clearNetworkTwo() begin.." << std::endl;
			clearNetworkTwo();
			std::cout << "\n clearNetworkTwo() finish.." << std::endl;
		}

		std::cout << "\n clearNetworkOne() begin.." << std::endl;
		clearNetworkOne();
		std::cout << "\n clearNetworkOne() finish.." << std::endl;
	}

	// write to file
	std::fstream outpufFile(BasicData::TargetAnaResPrefix + "PearsonSampleCoefficiency.result", std::ios_base::out);
	if (!outpufFile.is_open()) {
		std::cerr << "open output file error." << std::endl;
		return false;
	}

	outpufFile << "% Pearson Sample Coefficiency" << "\n";
	outpufFile << "% output format: result eventI eventJ" << "\n";

	for (const auto &elem : resultMap)
		outpufFile << elem.first << "\t" << elem.second.first << "\t" << elem.second.second << "\n";

	outpufFile.close();

	std::cout << "analysis degree relation level finish.." << std::endl;
	std::cout << "Press any key to continue.." << std::endl;
	getchar();
	return true;
}
bool EventAnalysis::getCommonNode(std::vector<unsigned> &commonVec)
{
	unsigned sizeOfA = networkOne.getCapacity(), sizeOfB = networkTwo.getCapacity();

	for (unsigned i = 1, j = 1; i <= sizeOfA && j <= sizeOfB; ++i, ++j) {

		unsigned resOneOut = 0, resOneIn = 0;
		if (networkOne.getAjaSizeFromHori(i, resOneOut) == false || networkOne.getAjaSizeFromVert(i, resOneIn) == false)
			return false;
		unsigned resTwoOut = 0, resTwoIn = 0;
		if (networkTwo.getAjaSizeFromHori(j, resTwoOut) == false || networkTwo.getAjaSizeFromVert(j, resTwoIn) == false)
			return false;

		if ((resOneOut + resOneIn > 0) && (resTwoOut + resTwoIn > 0))
			commonVec.push_back(i);
	}
	return true;
}
bool EventAnalysis::getDegreeOfDifLevlFromCommonVec(std::vector<unsigned> &commonVec, std::set<unsigned> &commonSet,
	std::vector<std::pair<unsigned, unsigned>> &a, std::vector<std::pair<unsigned, unsigned>> &b)
{
	//只计算commonVec中的节点
	for (const auto &i : commonVec) {
		//取A B 两层网络中，i点的邻接和逆邻接表(有向图，度 = 出度+入度)，再过滤
		std::vector<unsigned> ajaVecA, ajaRevVecA;
		if (networkOne.getAjaOfNodeFromHoriCroLink(i, ajaVecA) == false) {
			std::cerr << "get aja of node from hori error: " << i << std::endl;
			return false;
		}
		if (networkOne.getAjaOfNodeFromVertCroLink(i, ajaRevVecA) == false) {
			std::cerr << "get rev aja of node from vert error: " << i << std::endl;
			return false;
		}

		std::vector<unsigned> ajaVecB, ajaRevVecB;
		if (networkTwo.getAjaOfNodeFromHoriCroLink(i, ajaVecB) == false) {
			std::cerr << "get aja of node from hori error : " << i << std::endl;
			return false;
		}
		if (networkTwo.getAjaOfNodeFromVertCroLink(i, ajaRevVecB) == false) {
			std::cerr << "get aja of node from vert error: " << i << std::endl;
			return false;
		}
		// 过滤下，仅仅交集点 commonSet 有效
		unsigned degreeOfA = existTimeInSet(ajaVecA, commonSet) + existTimeInSet(ajaRevVecA, commonSet);
		a.push_back(std::make_pair(i, degreeOfA));
		unsigned degreeOfB = existTimeInSet(ajaVecB, commonSet) + existTimeInSet(ajaRevVecB, commonSet);
		b.push_back(std::make_pair(i, degreeOfB));
	}
	return true;
}
/*
*	return exist time in commonSet
*/
unsigned EventAnalysis::existTimeInSet(std::vector<unsigned> &source, std::set<unsigned> &commonSet)
{
	if (source.size() == 0 || commonSet.size() == 0)
		return 0;
	unsigned res = 0;
	for (const auto &u : source) {
		if (commonSet.find(u) != commonSet.end())
			res++;
	}
	return res;
}
/*
*	return the average of source
*/
double EventAnalysis::calAverageFromMap(std::vector<std::pair<unsigned, unsigned>> &source)
{
	unsigned total = 0;
	for (const auto &elem : source)
		total += elem.second;
	return (double)(total) / (double)(source.size());
}

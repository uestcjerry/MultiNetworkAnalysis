#include "../../include/basicDef/BasicData.h"
#include "../../include/eventAnalysis/EventAnalysis.h"

#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <queue>

/* ================================ connected component ============================================= */

/*
*	计算事件的连通分支数
*	output formata:
*	file: filename
*	total number of connected component
*	size of each \t number
*/
bool EventAnalysis::anaConnectedComponent()
{
	std::cout << "connected component begin.." << std::endl;

	std::string srcFilePrefix = BasicData::SrcEventWithTimePrefix;

	for (unsigned i = 0; i < BasicData::VecSrcEventFiles.size(); ++i) {
		std::cout << "\n first for: " << BasicData::VecSrcEventFiles.at(i) << std::endl;	/////////////

		if (buildNetworkOne(srcFilePrefix + BasicData::VecSrcEventFiles.at(i)) == false) {
			std::cerr << "build network one error." << std::endl, getchar();
			clearNetworkOne();
			return false;
		}

		unsigned arrayLength = networkOne.getCapacity();

		std::unique_ptr<BfsNode[]> bfsArray(new BfsNode[arrayLength + 1]);		// 1 ~ capacity

		std::cout << "proc bfs array begin.." << std::endl;
		if (procBfsArray(bfsArray) == false) {
			std::cerr << "init bfs array error." << std::endl;
			return false;
		}

		std::cout << "get connect component  begin.." << std::endl;

		std::vector<std::pair<unsigned, unsigned>> resVec;
		if (getConnectedComponent(bfsArray, resVec) == false) {
			std::cerr << "get connected component error." << std::endl;
			return false;
		}
		std::cout << "write file begin.." << std::endl;
		std::stringstream ss;
		ss << i;		std::string fileId = ss.str();
		std::fstream outpufFile(BasicData::TargetAnaResPrefix + "ConnectedComponent_" + fileId, std::ios_base::out);
		if (!outpufFile.is_open()) {
			std::cerr << "open output file error." << std::endl;
			return false;
		}

		//write to file
		outpufFile << "% file name : " << srcFilePrefix + BasicData::VecSrcEventFiles.at(i) << std::endl;
		outpufFile << "% connected component number : " << resVec.size() << std::endl;

		for (unsigned k = 0; k < resVec.size(); k++) {
			outpufFile << resVec.at(k).first << "\t" << resVec.at(k).second << "\n";
		}
		outpufFile.close();
		std::cout << "write file finish.." << std::endl;

		std::cout << "\n clearNetworkOne() begin.." << std::endl;
		clearNetworkOne();
		std::cout << "\n clearNetworkOne() finish.." << std::endl;
	}

	std::cout << "connected component finish.." << std::endl;
	return true;
}
bool EventAnalysis::procBfsArray(std::unique_ptr<BfsNode[]> &bfsArray)
{
	// initialize bfs node
	//std::cout << "network one capa = " << networkOne.getCapacity();

	unsigned capa = networkOne.getCapacity();
	for (unsigned i = 1; i <= capa; ++i) {
		unsigned horiSize = 0, vertSize = 0;
		if (networkOne.getAjaSizeFromHori(i, horiSize) == false) {
			std::cerr << "get aja size from hori :" << i << " error." << std::endl;
			return false;
		}
		if (horiSize != 0) {
			bfsArray[i].useNode(); continue;
		}
		if (networkOne.getAjaSizeFromVert(i, vertSize) == false) {
			std::cerr << "get aja size from vert :" << i << " error." << std::endl;
			return false;
		}
		if (vertSize != 0) {
			bfsArray[i].useNode(); continue;
		}
	}
	unsigned branch = 1;
	for (unsigned i = 1; i <= capa; ++i)
		if (bfsArray[i].getColor() == BfsNode::BfsColor::WHITE && bfsArray[i].useJudge())
			setBfsBranch(bfsArray, i, branch++);

	return true;
}
bool EventAnalysis::setBfsBranch(std::unique_ptr<BfsNode[]> &bfsArray, const unsigned start, const unsigned branch)
{
	bfsArray[start].setColor(BfsNode::BfsColor::GRAY);
	bfsArray[start].setBranch(branch);

	std::queue<unsigned> myQue;
	myQue.push(start);

	while (!myQue.empty()) {
		unsigned u = myQue.front();
		myQue.pop();

		std::vector<unsigned> ajaOfU;
		if (networkOne.getAjaOfNodeFromHoriCroLink(u, ajaOfU) == false) {
			std::cerr << "get aja of node from hori error." << std::endl;
			return false;
		}
		for (const auto &v : ajaOfU)
			if (bfsArray[v].getColor() == BfsNode::BfsColor::WHITE && bfsArray[v].useJudge()) {
				bfsArray[v].setBranch(branch); bfsArray[v].setColor(BfsNode::BfsColor::GRAY);
				myQue.push(v);
			}

		std::vector<unsigned> reajaOfU;
		if (networkOne.getAjaOfNodeFromVertCroLink(u, reajaOfU) == false) {
			std::cerr << "get aja of node from vert error." << std::endl;
			return false;
		}
		for (const auto &v : reajaOfU)
			if (bfsArray[v].getColor() == BfsNode::BfsColor::WHITE && bfsArray[v].useJudge()) {
				bfsArray[v].setBranch(branch); bfsArray[v].setColor(BfsNode::BfsColor::GRAY);
				myQue.push(v);
			}

		bfsArray[u].setColor(BfsNode::BfsColor::BLACK);
	}
	return true;
}
bool EventAnalysis::getConnectedComponent(std::unique_ptr<BfsNode[]> &bfsArray, std::vector<std::pair<unsigned, unsigned>> &res)
{
	std::map<unsigned, unsigned> record;

	unsigned capa = networkOne.getCapacity();

	for (unsigned i = 1; i <= capa; ++i) {
		if (bfsArray[i].useJudge() == false)
			continue;
		record[bfsArray[i].getBranch()]++;
	}
	std::map<unsigned, unsigned> result;
	for (const auto &elem : record)
		result[elem.second]++;					//连通分支的大小， 个数
	for (const auto &elem : result)
		res.push_back(std::make_pair(elem.first, elem.second));

	return true;
}
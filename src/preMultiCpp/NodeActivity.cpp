#include "../../include/preMulti/PreMultiAna.h"
#include <iostream>
#include <cmath>
#include <ctgmath>
#include <utility>
#include <fstream>
#include <map>

/*
*	计算 node activity Bi（节点活跃度） 的分布， exp指数
*	output: <i Bi>
*/
bool PreMultiLayerManage::calNodeActiDisOfBi(const std::string &outputFilePref)
{
	std::cout << "init node acti before bi begin.." << std::endl;

	if (initNodeActiBeforeBi() == false) {
		std::cerr << "init node activity before Bi error." << std::endl;
		return false;
	}

	std::cout << "init node acti finish." << std::endl;//	
	//getchar();

	unsigned maxNodeId = 0;
	if (findMaxNodeIdFromMultiNet(maxNodeId) == false)
		return false;

	std::cout << "maxnodeid = " << maxNodeId << std::endl;		//
	//getchar();

	std::vector<std::pair<unsigned, unsigned>> resultVec;				// format <nodeId, Bi>

	//对于每个节点，计算它的 Bi
	for (unsigned i = 1; i <= maxNodeId; ++i) {

		if (i % 10000 == 0)
			std::cout << "i = " << i << std::endl;//////////

		unsigned Bi = 0;
		for (unsigned j = 0; j < multiLayerNum; ++j) {
			// i 如果超过当前层网络的capacity，跳过这层
			if (i > multiLayerObj.at(j).getLayerCapacity())
				continue;
			unsigned tempI = 0;
			if (multiLayerObj.at(j).getNodeActiVecAt(i, tempI) == false) {
				std::cerr << "get node acti vec at :" << i << " error." << std::endl;
				return false;
			}
			Bi += tempI;
		}
		resultVec.push_back(std::make_pair(i, Bi));
	}

	//std::cout << "write file begin.." << std::endl; //////

	if (writeFileOfNodeActi(outputFilePref, resultVec) == false) {
		std::cerr << "write file of node activity error." << std::endl;
		return false;
	}

	//std::cout << "write file finish" << std::endl;	//
	//getchar();

	return true;
}
bool PreMultiLayerManage::initNodeActiBeforeBi()
{
	if (multiLayerNum == 0)
		return false;
	for (unsigned i = 0; i < multiLayerNum; ++i) {
		std::cout << "init node acti vec at i = " << i << std::endl;
		multiLayerObj.at(i).initNodeActiVec();
	}

	return true;
}
bool PreMultiLayerManage::findMaxNodeIdFromMultiNet(unsigned &max)
{
	if (multiLayerNum == 0)
		return false;

	max = 0;
	for (unsigned i = 0; i < multiLayerNum; ++i) {
		unsigned maxNodeTemp = multiLayerObj.at(i).getLayerCapacity();
		max = maxNodeTemp > max ? maxNodeTemp : max;
	}
	return true;
}
bool PreMultiLayerManage::writeFileOfNodeActi(const std::string &outputPref,
	const std::vector<std::pair<unsigned, unsigned>> &resultVec)
{
	// to do ..
	std::fstream outpufFile(std::string(outputPref + "nodeActi节点活跃度总"), std::ios_base::out);
	if (!outpufFile) {
		std::cerr << "open file error." << std::endl;
		return false;
	}
	for (const auto &elem : resultVec)
		outpufFile << elem.first << "\t" << elem.second << "\n";

	outpufFile.close();

	return true;
}

/*
*	分析 node activity Bi 结果文件，取对数画图
*/
bool PreMultiLayerManage::handleTheResultFromNodeActi(const std::string &srcFile, const std::string &tarPref, const double exp)
{
	//std::cout << "handle the result from node acti begin.." << std::endl;

	//map<Bi, 出现次数>
	std::map<unsigned, unsigned> anaMap;

	std::fstream inputFile(srcFile, std::ios_base::in);
	if (!inputFile) {
		std::cerr << "open input file error." << std::endl; return false;
	}
	unsigned nodeId, Bi;
	while (inputFile >> nodeId >> Bi)
		anaMap[Bi]++;

	inputFile.close();

	std::stringstream ss;
	ss << static_cast<int>(exp * 10);
	std::string targetFileName(ss.str());

	std::fstream outputFile(tarPref + "file" + targetFileName, std::ios_base::out);
	if (!outputFile) {
		std::cerr << "open output file error." << std::endl;	return false;
	}
	for (const auto &elem : anaMap)
		outputFile << elem.first << "\t" << pow(static_cast<double>(elem.second), -exp) << "\n";

	outputFile.close();

	//把原始的结果写文件 
	//输出格式: <nodeid, Bi>
	std::fstream outputOrigin(tarPref + "originRes", std::ios_base::out);
	if (!outputOrigin)
		return false;
	for (const auto &elem : anaMap)
		outputOrigin << elem.first << "\t" << elem.second << std::endl;
	outputOrigin.close();

	//std::cout << "handle the result from node acti finish.." << std::endl;
	return true;
}

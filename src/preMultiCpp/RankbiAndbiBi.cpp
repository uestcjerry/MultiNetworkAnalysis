#include "../../include/preMulti/PreMultiAna.h"
#include "../../include/preMulti/Rankbi.h"
#include <iostream>
#include <fstream>
#include <sstream>

/*
 *	分析节点活跃度向量 bi 的分布 
 *	Rank(bi) && Rank(bi | Bi)
 */
bool PreMultiLayerManage::calNodeVecDisOfbiAndbiBi(const std::string &tarFilePref)
{
	// 在使用多层网络之前，需要计算每一层的节点活跃度，调用init()
	if (initNodeActiBeforeBi() == false) {
		std::cerr << "init node activity before Bi error." << std::endl;
		return false;
	}

	unsigned maxNodeId = 0;
	if (findMaxNodeIdFromMultiNet(maxNodeId) == false) {
		std::cerr << "find max node id from multi error." << std::endl;
		return false;
	}
	
	//	保存每个点计算时需要的信息
	std::vector<Rankbi> rankVec = std::vector<Rankbi>(maxNodeId + 1);  //有效角标 [1, maxNodeId]
	//	最大Bi
	unsigned tempMaxBi = 0, tempBi = 0;

	//	根据多层网络数据，计算每个点的信息
	for (unsigned i = 1; i <= maxNodeId; ++i) {
		rankVec.at(i).setNodeId(i);

		//计算每个点的活跃度向量 bi，从每一层的相应活跃度向量中寻找
		std::vector<unsigned> tempbi = std::vector<unsigned>(multiLayerNum);	//维度和多层的层数一样 一一对应
		
		//查询每一层
		for (unsigned j = 0; j < multiLayerNum; ++j) {
			// i 如果超过当前层网络的capacity，跳过这层
			if (i > multiLayerObj.at(j).getLayerCapacity())
				continue;

			unsigned tempi = 0;
			if (multiLayerObj.at(j).getNodeActiVecAt(i, tempi) == false) {
				std::cerr << "get node activity vec at : " << i << " error." << std::endl;
				return false;
			}
			tempbi.at(j) = tempi;
		}
		//为每一个元素正确赋值
		rankVec.at(i).setbi(tempbi);
		rankVec.at(i).setMultilayerNum(multiLayerNum);
		if (rankVec.at(i).calRankbiAndBiOfThisNode() == false) {
			std::cerr << "cal rank bi and Bi of this node error, nodeid = " << i << std::endl;
			return false;
		}

		//更新最大 Bi 后续计算使用
		tempBi = rankVec.at(i).getBi();
		tempMaxBi = tempBi > tempMaxBi ? tempBi : tempMaxBi;
	}

	// 统计结果
	/*
	 *	第一组： map<rank(bi), exist number>
	 */
	std::map<unsigned, unsigned> firstResMap;

	//遍历每个点
	for (unsigned i = 1; i <= maxNodeId; ++i)
		firstResMap[rankVec.at(i).getRankbi()]++;
	//写文件
	if (writeFileOfRankbi(tarFilePref, firstResMap) == false) {
		std::cerr << "write file of rank bi error." << std::endl;
		return false;
	}

	/*
	 *	第二组：条件概率 vector<std::pair<Bi, map<rank(bi|Bi), exist number>>> Bi当成已知条件，即从[1, maxBi]，每一组Bi一条曲线
	 */
	std::vector<std::pair<unsigned, std::map<unsigned, unsigned> > > secondResVec;
	
	//	使用前面找到的 maxBi， 对于每一个 Bi 值，作为条件概率的已知条件
	for (unsigned m = 1; m <= tempMaxBi; ++m) {
		std::map<unsigned, unsigned> tempBiMap;
		
		//	遍历每个点，过滤条件
		for (unsigned k = 1; k <= maxNodeId; ++k) {
			if (rankVec.at(k).getBi() != m)
				continue;
			tempBiMap[rankVec.at(k).getRankbi()]++;
		}
		secondResVec.push_back(std::make_pair(m, tempBiMap));
	}

	//写文件
	if (writeFileOfRankbiBi(tarFilePref, secondResVec) == false) {
		std::cerr << "write file of rank bi Bi error." << std::endl;
		return false;
	}

	return true;
}

bool PreMultiLayerManage::writeFileOfRankbi(const std::string &tarFilePref, const std::map<unsigned, unsigned> &resVec)
{
	std::fstream outputFile(tarFilePref + "rankbi", std::ios_base::out);
	if (!outputFile.is_open()) {
		std::cerr << "open file error." << std::endl;
		return false;
	}

	outputFile << "% rank(bi) existNumber" << "\n";

	for (const auto &elem : resVec)
		outputFile << elem.first << "\t" << elem.second << "\n";

	outputFile.close();

	return true;
}

bool PreMultiLayerManage::writeFileOfRankbiBi(const std::string &tarFilePref, 
						const std::vector<std::pair<unsigned, std::map<unsigned, unsigned > > > &resVec)
{
	for (unsigned i = 0; i < resVec.size(); ++i) {
		std::stringstream ss;
		ss << resVec.at(i).first;
		std::string fileName = tarFilePref + "Bi" + ss.str();
		
		std::fstream outputFile(fileName, std::ios_base::out);
		if (!outputFile.is_open()) {
			std::cerr << "open file error: " << fileName << std::endl;
			return false;
		}

		unsigned totalNumber = 0;
		for (const auto &elem : resVec.at(i).second)
			totalNumber += elem.second;

		outputFile << "% rank(bi|Bi) existNumber" << "\n";
		outputFile << "% total number = " << totalNumber << "\n";

		for (const auto &elem : resVec.at(i).second)
			outputFile << elem.first << "\t" << elem.second << "\n";

		outputFile.close();
	}

	return true;
}

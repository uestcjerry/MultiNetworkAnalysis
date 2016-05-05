#include "../../include/preMulti/PreMultiAna.h"
#include "../../include/preMulti/Rankbi.h"
#include <iostream>
#include <fstream>
#include <sstream>

/*
 *	�����ڵ��Ծ������ bi �ķֲ� 
 *	Rank(bi) && Rank(bi | Bi)
 */
bool PreMultiLayerManage::calNodeVecDisOfbiAndbiBi(const std::string &tarFilePref)
{
	// ��ʹ�ö������֮ǰ����Ҫ����ÿһ��Ľڵ��Ծ�ȣ�����init()
	if (initNodeActiBeforeBi() == false) {
		std::cerr << "init node activity before Bi error." << std::endl;
		return false;
	}

	unsigned maxNodeId = 0;
	if (findMaxNodeIdFromMultiNet(maxNodeId) == false) {
		std::cerr << "find max node id from multi error." << std::endl;
		return false;
	}
	
	//	����ÿ�������ʱ��Ҫ����Ϣ
	std::vector<Rankbi> rankVec = std::vector<Rankbi>(maxNodeId + 1);  //��Ч�Ǳ� [1, maxNodeId]
	//	���Bi
	unsigned tempMaxBi = 0, tempBi = 0;

	//	���ݶ���������ݣ�����ÿ�������Ϣ
	for (unsigned i = 1; i <= maxNodeId; ++i) {
		rankVec.at(i).setNodeId(i);

		//����ÿ����Ļ�Ծ������ bi����ÿһ�����Ӧ��Ծ��������Ѱ��
		std::vector<unsigned> tempbi = std::vector<unsigned>(multiLayerNum);	//ά�ȺͶ��Ĳ���һ�� һһ��Ӧ
		
		//��ѯÿһ��
		for (unsigned j = 0; j < multiLayerNum; ++j) {
			// i ���������ǰ�������capacity���������
			if (i > multiLayerObj.at(j).getLayerCapacity())
				continue;

			unsigned tempi = 0;
			if (multiLayerObj.at(j).getNodeActiVecAt(i, tempi) == false) {
				std::cerr << "get node activity vec at : " << i << " error." << std::endl;
				return false;
			}
			tempbi.at(j) = tempi;
		}
		//Ϊÿһ��Ԫ����ȷ��ֵ
		rankVec.at(i).setbi(tempbi);
		rankVec.at(i).setMultilayerNum(multiLayerNum);
		if (rankVec.at(i).calRankbiAndBiOfThisNode() == false) {
			std::cerr << "cal rank bi and Bi of this node error, nodeid = " << i << std::endl;
			return false;
		}

		//������� Bi ��������ʹ��
		tempBi = rankVec.at(i).getBi();
		tempMaxBi = tempBi > tempMaxBi ? tempBi : tempMaxBi;
	}

	// ͳ�ƽ��
	/*
	 *	��һ�飺 map<rank(bi), exist number>
	 */
	std::map<unsigned, unsigned> firstResMap;

	//����ÿ����
	for (unsigned i = 1; i <= maxNodeId; ++i)
		firstResMap[rankVec.at(i).getRankbi()]++;
	//д�ļ�
	if (writeFileOfRankbi(tarFilePref, firstResMap) == false) {
		std::cerr << "write file of rank bi error." << std::endl;
		return false;
	}

	/*
	 *	�ڶ��飺�������� vector<std::pair<Bi, map<rank(bi|Bi), exist number>>> Bi������֪����������[1, maxBi]��ÿһ��Biһ������
	 */
	std::vector<std::pair<unsigned, std::map<unsigned, unsigned> > > secondResVec;
	
	//	ʹ��ǰ���ҵ��� maxBi�� ����ÿһ�� Bi ֵ����Ϊ�������ʵ���֪����
	for (unsigned m = 1; m <= tempMaxBi; ++m) {
		std::map<unsigned, unsigned> tempBiMap;
		
		//	����ÿ���㣬��������
		for (unsigned k = 1; k <= maxNodeId; ++k) {
			if (rankVec.at(k).getBi() != m)
				continue;
			tempBiMap[rankVec.at(k).getRankbi()]++;
		}
		secondResVec.push_back(std::make_pair(m, tempBiMap));
	}

	//д�ļ�
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

#include "../../include/basicDef/BasicData.h"
#include "../../include/eventAnalysis/EventAnalysis.h"
#include "../../include/dirent/MyDirent.h"

#include <iostream>
#include <fstream>
#include <sstream>

/* 
 *	k- core decomposition revoke function
 *	ת���¼���kcore�ֽ�
 */
bool EventAnalysis::kCoreDecomposition() 
{
	std::cout << "k core decomposition begin.." << std::endl;

	//	kcore �ֽ⣬�ֱ���� ת������ && �û� ���зֽ����򣬷ֱ���ò�ͬ��src����

	//	step one: �����Ƕ�ת���¼���kcore�ֽ�
	std::string srcFilePrefix = BasicData::SrcEventWithTimePrefix;	

	//	step one: BasicData::VecSrcEventFiles.size() �����ת��������¼��б�

	//	to do here..
	//	���ղ�ͬ��stepĿ�ģ������vector�ò�ͬ��
	//	step one:	BasicData::VecSrcEventFiles
	for (unsigned i = 0; i < BasicData::VecSrcEventFiles.size(); ++i) {
		std::cout << "\n handling : " << BasicData::VecSrcEventFiles.at(i) << std::endl;	/////////////

		if (buildNetworkOne(srcFilePrefix + BasicData::VecSrcEventFiles.at(i)) == false) {
			std::cerr << "build network one error." << std::endl, getchar();
			clearNetworkOne();
			return false;
		}

		//todo decomposition
		std::vector<std::pair<unsigned, unsigned>> res;
		if (decompEventFile(networkOne, res, i) == false) {
			std::cerr << "de-composite event file error : " << BasicData::VecSrcEventFiles.at(i) << std::endl;
			return false;
		}

		//д���ļ�֮��
		if (kCoreDecomWriteFile(BasicData::TargetAnaResPrefix, i, res) == false) {
			std::cerr << "k-core decomposition write file error." << std::endl;
			return false;
		}
		clearNetworkOne();
	}

	std::cout << "k core decomposition finish.." << std::endl;
	std::cout << "Press any key to continue.." << std::endl;
	getchar();
	return true;
}
bool EventAnalysis::decompEventFile(CrossLink &network, std::vector<std::pair<unsigned, unsigned>> &resultVec, const unsigned eventId)
{
	std::cout << "decomposite event file begin.." << std::endl;

	//����� pair<ks, nodeid> ���棬first��ksֵ��secondΪ�ڵ��

	unsigned len = network.getCapacity();

	//ʣ��ڵ��ʶ���飬vector�±��ʾ�ڵ�id��first��ʾ�ڵ��Ƿ���Ч����Ϊ0�ĵ���Ч����second��ʾ��ǰ����ʣ�Ķ�
	std::vector<std::pair<bool, unsigned>> remainEdgeVec = std::vector<std::pair<bool, unsigned>>(len + 1);

	//��¼ʣ��Ľڵ���Ŀ
	unsigned remainNodeNum = 0;

	for (unsigned i = 1; i <= len; ++i) {
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

		unsigned horiAndVert = 0;
		for (const auto &elem : ajaVec)
			if (elem != i)
				horiAndVert++;
		for (const auto &elem : ajaReVec)
			if (elem != i)
				horiAndVert++;

		/*
		unsigned horiSize = 0, vertSize = 0;
		if (network.getAjaSizeFromHori(i, horiSize) == false) {
			std::cerr << "get aja size from hori error." << std::endl;
			return false;
		}
		if (network.getAjaSizeFromVert(i, vertSize) == false) {
			std::cerr << "get aja size from vert error." << std::endl;
			return false;
		}
		if (horiSize + vertSize != 0) {
			remainEdgeVec.at(i).first = true;	remainNodeNum++;
		}
		else
			remainEdgeVec.at(i).first = false;
		*/

		if (horiAndVert != 0) {
			remainEdgeVec.at(i).first = true; remainNodeNum++;
		}
		else
			remainEdgeVec.at(i).first = false;

		remainEdgeVec.at(i).second = horiAndVert;
	}

	/*
	*	k-core �ֽ�
	*	�Ӷ���С��Ϊi = 1���ĵ㿪ʼ��ɾ�����Լ����ıߣ�֪��ɾ�������ж�Ϊi�ĵ�
	*	Ȼ�����ɾ�����Ӷ���С�Ŀ�ʼɾ��һֱ��ͼ���ڲ�ɾ��������ͬ�ĵ㹹�ɼ��Ͼ�����ͬ��ksֵ
	*	���õ����е��ksֵ����ɷֽ�
	*/
	unsigned ks = 1;
	unsigned preMinDegree = findMinDegreeNow(network, remainEdgeVec);

	while (1) {
		//ÿһ�֣������� curMinDeg ��ͬ�ĵ��ռ�������һ����ɾ��
		//��һ�ּ����� curMinDeg ����� pre ��ͬ�� ks++ ��һ��

		unsigned curMinDegree = findMinDegreeNow(network, remainEdgeVec);
		if (curMinDegree != preMinDegree) {
			ks++;
			preMinDegree = curMinDegree;
		}

		//�õ����е�֮��һ����ɾ��
		std::vector<unsigned> thisTurnNodesVec;
		for (unsigned i = 1; i <= len; ++i) {
			if (remainEdgeVec.at(i).first && remainEdgeVec.at(i).second == curMinDegree)
				thisTurnNodesVec.push_back(i);
		}

		for (const auto &k : thisTurnNodesVec) {
			//std::cout << "this turn : " << k << std::endl;//////////////

			std::vector<unsigned> ajaVec;
			if (network.getAjaOfNodeFromHoriCroLink(k, ajaVec) == false)
				return false;

			if (ajaVec.size() != 0) {
				if (delEdgesFromVec(network, true, k, ajaVec) == false) {
					std::cerr << "del edges from vec error : " << k << std::endl;
					return false;
				}
				for (const auto &j : ajaVec)
					remainEdgeVec.at(j).second--;

				//std::cout << "ajavec size=" << ajaVec.size();	/////////////////////
				remainEdgeVec.at(k).second -= ajaVec.size();
				//std::cout << ", remain at: " << k << " degree=" << remainEdgeVec.at(k).second << std::endl;////////////////	
			}

			std::vector<unsigned> reAjaVec;
			if (network.getAjaOfNodeFromVertCroLink(k, reAjaVec) == false)
				return false;

			if (reAjaVec.size() != 0) {
				if (delEdgesFromVec(network, false, k, reAjaVec) == false) {
					std::cerr << "del edges from reVec error: " << k << std::endl;
					return false;
				}
				for (const auto &j : reAjaVec)
					remainEdgeVec.at(j).second--;

				//std::cout << "reajavec size=" << reAjaVec.size();	/////////////////////
				remainEdgeVec.at(k).second -= reAjaVec.size();
				//std::cout << ", remain at: " << k << " degree=" << remainEdgeVec.at(k).second << std::endl;////////////////	

			}
			//��ǰ��� k-core ֵ ks ��¼
			resultVec.push_back(std::make_pair(ks, k));

			//ɾ����ǰ��i���������������
			remainNodeNum--;
			remainEdgeVec.at(k).first = false;
			//remainEdgeVec.at(k).second = 0;
		}

		//ֱ�����е�ɾ����ϣ�����ѭ��
		if (remainNodeNum == 0)
			break;
	}
	std::cout << "decomposite event file finish.." << std::endl;
	return true;
}
/*
*	�ҳ���ǰͼ����С��degree����Ϊk-core�ֽ������
*/
unsigned EventAnalysis::findMinDegreeNow(const CrossLink &network, const std::vector<std::pair<bool, unsigned>> &src)
{
	unsigned len = network.getCapacity();
	unsigned currentMin = 2 << 16;

	for (unsigned i = 1; i <= len; ++i) {
		if (src.at(i).first) {					//first�жϽڵ��Ƿ���Ч�����Ȳ�Ϊ0��
			unsigned remainDegree = src.at(i).second;
			if (remainDegree < currentMin)		//if (remainDegree < currentMin && remainDegree != 0)
				currentMin = remainDegree;
		}
	}
	return currentMin;
}
/*
*	bool order ��ʶ˳��trueΪ����˳��falseΪ����˳��
*	����˳��ıߣ�	i -> vector
*	����˳��ıߣ�	vector -> i
*/
bool EventAnalysis::delEdgesFromVec(CrossLink &network, bool order, const unsigned i, std::vector<unsigned> &srcVec)
{
	for (const auto &elem : srcVec) {
		if (order) {
			//std::cout << "delete edge : " << i << " " << elem << std::endl;
			if (network.deleteUndiEdge(i, elem) == false) {
				std::cerr << "delete undirected edge error." << std::endl;
				return false;
			}
		}
		else {
			//std::cout << "delete edge : " <<elem << " " << i << std::endl;
			if (network.deleteUndiEdge(elem, i) == false) {
				std::cerr << "delete undirected edge error." << std::endl;
				return false;
			}
		}
	}
	return true;
}
/*
*	pair<ks, nodeId> �����ʽ
*/
bool EventAnalysis::kCoreDecomWriteFile(const std::string &tarFilePre, const unsigned eventId,
	std::vector<std::pair<unsigned, unsigned>> &resultVec)
{
	std::stringstream ss;
	ss << eventId;

	std::fstream outpufFile(tarFilePre + "event" + ss.str(), std::ios_base::out);
	if (!outpufFile.is_open()) {
		std::cerr << "open output file error." << std::endl;
		return false;
	}
	// todo.
	outpufFile << "% k-core decomposition <ks, nodeId>" << std::endl;

	for (const auto &elem : resultVec)
		outpufFile << elem.first << "\t" << elem.second << "\n";

	outpufFile.close();
	return true;
}

/*
 *	�û���ϵ����kcore�ֽ�
 */
bool EventAnalysis::kCoreUserDecomposition()
{
	std::cout << "k core decomposition begin.." << std::endl;

	//	kcore �ֽ⣬�ֱ���� ת������ && �û� ���зֽ����򣬷ֱ���ò�ͬ��src����

	//	step two: �����ǰ��û���ϵ�����¼�ת����ʽ ���û���kcore�ֽ⣬��ҪԤ�����û���ϵ�ļ����ɴ�ʱ����ġ�ת�������ʽ���ļ������е��û���ȥ�ص�һ������
	const std::string srcFilePrefix = BasicData::SrcEventWithTimePrefix;

	const std::string fileName = "user_k_core_src_network";
	//const std::string fileName = "aggregated_event_network";


	//	���ղ�ͬ��stepĿ��
	//	step two:	userFileVec
	if (buildNetworkOne(srcFilePrefix + fileName) == false) {
		std::cerr << "build network one error." << std::endl, getchar();
		clearNetworkOne();
		return false;
	}

	//todo decomposition
	std::vector<std::pair<unsigned, unsigned>> res;
	if (decompEventFile(networkOne, res, 0) == false) {
		std::cerr << "de-composite event file error : " << srcFilePrefix + fileName << std::endl;
		return false;
	}

	//д���ļ�֮��
	if (kCoreDecomWriteFile(BasicData::TargetAnaResPrefix, 0, res) == false) {
		std::cerr << "k-core decomposition write file error." << std::endl;
		return false;
	}
	clearNetworkOne();

	std::cout << "k core decomposition finish.." << std::endl;
	std::cout << "Press any key to continue.." << std::endl;
	getchar();
	return true;
}
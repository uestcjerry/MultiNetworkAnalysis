#include "../../include/basicDef/BasicData.h"
#include "../../include/eventAnalysis/EventAnalysis.h"

#include <iostream>
#include <fstream>
#include <sstream>

/* 
 *	k- core decomposition 
 */
bool EventAnalysis::kCoreDecomposition() 
{
	std::cout << "k core decomposition begin.." << std::endl;

	std::string srcFilePrefix = BasicData::SrcEventWithTimePrefix;

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

		//写入文件之中
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

	//结果以 pair<ks, nodeid> 保存，first是ks值，second为节点号

	unsigned len = network.getCapacity();

	//剩余节点标识数组，vector下标表示节点id，first表示节点是否有效（度为0的点无效），second表示当前点所剩的度
	std::vector<std::pair<bool, unsigned>> remainEdgeVec = std::vector<std::pair<bool, unsigned>>(len + 1);

	//记录剩余的节点数目
	unsigned remainNodeNum = 0;

	for (unsigned i = 1; i <= len; ++i) {
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
		remainEdgeVec.at(i).second = horiSize + vertSize;
	}

	/*
	*	k-core 分解
	*	从度最小（为i = 1）的点开始，删除它以及它的边，知道删除完所有度为i的点
	*	然后继续删除，从度最小的开始删，一直往图的内部删除，度相同的点构成集合具有相同的ks值
	*	最后得到所有点的ks值，完成分解
	*/
	unsigned ks = 1;
	unsigned preMinDegree = findMinDegreeNow(network, remainEdgeVec);

	while (1) {
		//每一轮，将所有 curMinDeg 相同的点收集起来，一起做删除
		//下一轮继续找 curMinDeg 如果和 pre 不同， ks++ 下一层

		unsigned curMinDegree = findMinDegreeNow(network, remainEdgeVec);
		if (curMinDegree != preMinDegree) {
			ks++;
			preMinDegree = curMinDegree;
		}

		//拿到所有点之后一起做删除
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
			//当前点的 k-core 值 ks 记录
			resultVec.push_back(std::make_pair(ks, k));

			//删除当前点i，清除工作做完整
			remainNodeNum--;
			remainEdgeVec.at(k).first = false;
			//remainEdgeVec.at(k).second = 0;
		}

		//直到所有点删除完毕，跳出循环
		if (remainNodeNum == 0)
			break;
	}
	std::cout << "decomposite event file finish.." << std::endl;
	return true;
}
/*
*	找出当前图中最小的degree，作为k-core分解的依据
*/
unsigned EventAnalysis::findMinDegreeNow(const CrossLink &network, const std::vector<std::pair<bool, unsigned>> &src)
{
	unsigned len = network.getCapacity();
	unsigned currentMin = 2 << 16;

	for (unsigned i = 1; i <= len; ++i) {
		if (src.at(i).first) {					//first判断节点是否有效（即度不为0）
			unsigned remainDegree = src.at(i).second;
			if (remainDegree < currentMin)		//if (remainDegree < currentMin && remainDegree != 0)
				currentMin = remainDegree;
		}
	}
	return currentMin;
}
/*
*	bool order 标识顺序，true为正向顺序，false为反向顺序
*	正向顺序的边：	i -> vector
*	反向顺序的边：	vector -> i
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
*	pair<ks, nodeId> 输出格式
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

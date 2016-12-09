#include "../../include/apCluster/ApClusteringProcedure.h"
#include "../../include/apCluster/ApClusterStorageDefinition.h"

#include <iostream>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <time.h>
#include <windows.h>

//参数是文件完整路径名
bool ApClusteringProcedure::buildNetwork(const std::string &file)
{
	if (network.readFileConsCrosLink(file) == false)
		return false;
	return true;
}
void ApClusteringProcedure::clearNetwork()
{
	network.deconsCrossLinkObj();
}

bool ApClusteringProcedure::apClusteringProcedure()
{
	std::cout.unsetf(std::ios::floatfield);
	std::cout.precision(10);

	const double preference = -15.561256;			// 测试用
	const double lambda = 0.5;

	// 先将所有转发事件网络分析一遍
	// preference 用正数传进去，保存文件的时候 * 10
	for (const auto &elem : BasicData::VecSrcEventFiles) {
		if (!apClusteringFromFile(elem, lambda, preference)) {
			std::cerr << "ap-clustering from file error: " << elem << std::endl;
			return false;
		}

		std::cout << "finish file: " << elem << std::endl;
		getchar();
	}

	// 再分析一下朋友关系网络
	/*
	if (!apClusteringFromFile("user_k_core_src_network", lambda)) {
		std::cerr << "ap-clustering from user-relationship error." << std::endl;
		return false;
	}
	*/
	return true;
}

/*
	几个步骤：
	1 :	建立network网络
	2 :	每次选定一个连通分量，标记访问
		对较大的连通分量进行 ap 聚类
		辅助 ApEdgeStorage && ApNodeStorage 进行存储标记
	3 :	保存ap聚类的结果
		output: filename.txt
		output format: nodeid

	心得体会：
	AP聚类的收敛终止条件判断，结合这里的情况，我们折中处理
	在可以接受的计算时间范围内，取最大推荐收敛状态的集合点，即 iterationTime 最大的推荐集合

*/
bool ApClusteringProcedure::apClusteringFromFile(const std::string &file, const double lambda, const double preference)
{
	// test from here 测试样例是否正确 ..
	std::cout.unsetf(std::ios::floatfield);
	std::cout.precision(10);

	if (!buildNetwork(BasicData::SrcEventWithTimePrefix + file)) {
		std::cerr << "build network error: " << std::endl;
		return false;
	}

	unsigned networkCapacity = network.getCapacity();

	// 获取当前网络所有点，存在的点为true，访问之后置false  下标有效范围 [1, capacity]
	NODETAG_T totalNodeVec(networkCapacity + 1);	
	unsigned nodesNumber = 0, fileNodesName = 0;

	if (!tagAllNetworkNodes(totalNodeVec, nodesNumber)) {
		std::cerr << "tagAllNetworkNodes(): error." << std::endl;
		return false;
	}

	// 选取一组连通分量，对较大的连通分量进行处理
	while (nodesNumber > 0) {
		unsigned componentStartNode = 0;
		for (unsigned i = 1; i < totalNodeVec.size(); ++i)
			if (totalNodeVec.at(i)) {
				componentStartNode = i;
				totalNodeVec.at(i) = false;
				break;
			}
		
		//获取连通分量 函数内标记连通分量所相关的点 更新 totalNodeVec
		
		// 连通分量这里还要加一组测试，多个连通分量的网络是否正确!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		
		std::vector<unsigned> componentNodeVec;
		componentNodeVec.push_back(componentStartNode);
		if (!getComponentNodesFromNetwork(totalNodeVec, componentNodeVec, componentStartNode)) {
			std::cerr << "get component nodes error." << std::endl;
			clearNetwork();
			return false;
		}
		unsigned componentNodesNum = componentNodeVec.size();
		std::cout << "componentNodesNum = " << componentNodesNum << std::endl;
		
		//getchar();

		// 测试的时候抹去
		/*
		if (componentNodesNum < 10000) {
			nodesNumber -= componentNodesNum;
			continue;
		}
		*/

		fileNodesName = nodesNumber;
		nodesNumber -= componentNodesNum;

		// 根据连通分量获取相似度矩阵 s 并初始化 a 和 r
		// 设置经验参数 lam = 0.5
		// 然后开始 ap 聚类的迭代计算
		ApEdgeStorage edgeStorageObj;
		edgeStorageObj.initApEdgeStorage(networkCapacity);
		
		// test 测试论文数据集，从样例文件读取
		//const double preference = -15.561256;
		/*
		edgeStorageObj.initApEdgeStorage(25);
		const std::string simiFile = "E:\\apclustermex_win32_2009\\apcluster_win32\\Similarities.txt";		// similarity文件
		if (!readSimilarityFromFile(simiFile, edgeStorageObj, preference)) {
			std::cerr << "readSimilarityFromFile() error." << std::endl;
			return false;
		}
		*/

		// 更新 s    
		// test论文数据集时候注释掉
		if (!getCnIndex(componentNodeVec, edgeStorageObj, preference)) {
			std::cerr << "getCnIndex() error." << std::endl;
			return false;
		}
		std::cout << "getCnIndex() finish.." << std::endl;
		

		// ap聚类三步骤 1： 更新r	2：更新a	3：观察本轮所推荐的点集合，当十次迭代的点集合不变时，停止迭代
		
		unsigned iterationTime = 1;
		std::vector<unsigned> lastTimeRecommendVec;

		unsigned max_iterationtime = 0;
		std::vector<unsigned> max_iteration_vec;
		const double calculation_time = 1000 * 10;			// 控制计算时间
			
		time_t start_time, end_time, cost_time;
		start_time = GetTickCount();
		end_time = GetTickCount();							// 1000 毫秒级
		cost_time = end_time - start_time;

		while ((iterationTime < 10) && (cost_time < calculation_time)) {
			if (!updateResponsibility(componentNodeVec, edgeStorageObj, lambda)) {		// re-edit
				std::cerr << "updateResponsibility() error." << std::endl;
				return false;
			}
			//std::cout << "updateResponsibility() finish.." << std::endl;

			if (!updateAvailability(componentNodeVec, edgeStorageObj, lambda)) {
				std::cerr << "updateAvailability() error." << std::endl;
				return false;
			}
			//std::cout << "updateAvailability() finish.." << std::endl;

			std::vector<unsigned> thisTimeRecommendVec;
			if (!getRecommendNode(edgeStorageObj, componentNodeVec, thisTimeRecommendVec)) {
				std::cerr << "getRecommendNode() error." << std::endl;
				return false;
			}

			std::sort(thisTimeRecommendVec.begin(), thisTimeRecommendVec.end());
			if (thisTimeRecommendVec == lastTimeRecommendVec && thisTimeRecommendVec.size() != 0)
				++iterationTime;
			else {
				iterationTime = 1;
				std::sort(thisTimeRecommendVec.begin(), thisTimeRecommendVec.end());
				lastTimeRecommendVec.assign(thisTimeRecommendVec.begin(), thisTimeRecommendVec.end());
			}

			if (iterationTime > max_iterationtime) {
				max_iterationtime = iterationTime;
				max_iteration_vec.assign(thisTimeRecommendVec.begin(), thisTimeRecommendVec.end());
			}

			end_time = GetTickCount();
			cost_time = end_time - start_time;
		}

		// test
		std::cout << "max iterartion time = " << max_iterationtime << std::endl;
		for (const auto &elem : max_iteration_vec)
			std::cout << elem << ", ";
		std::cout << std::endl;
		getchar();


		// 本轮所得到的推荐点保存起来
		std::stringstream ss;
		ss << file; ss << "_"; ss << fileNodesName;
		std::string outputName;
		ss >> outputName;

		std::fstream outputFile;
		outputFile.open(BasicData::TargetAnaResPrefix + outputName, std::fstream::out);
		if (!outputFile.is_open()) {
			std::cerr << "open() file error. file: " << file << std::endl;
			return false;
		}

		for (const auto &u : max_iteration_vec)
			outputFile << u << std::endl;
		outputFile.close();

		std::cout << "save file finish: " << BasicData::TargetAnaResPrefix + outputName << std::endl;
		getchar();
	} // while

	clearNetwork();
	return true;
}


bool ApClusteringProcedure::tagAllNetworkNodes(NODETAG_T &totalNodeVec, unsigned &nodesNumber)
{	
	unsigned networkCapacity = network.getCapacity();
	for (unsigned i = 1; i <= networkCapacity; ++i) {
		unsigned horiSize = 0, vertSize = 0;
		if (!network.getAjaSizeFromHori(i, horiSize) || !network.getAjaSizeFromVert(i, vertSize)) {
			std::cerr << "get hori / vert size from network error." << std::endl;
			return false;
		}
		if (horiSize + vertSize > 0) {
			totalNodeVec[i] = true;
			++nodesNumber;
		} else
			totalNodeVec[i] = false;
	}
	return true;
}

/*
 *	这里会更新 totalNodeVec 访问过的置false
 */
bool ApClusteringProcedure::getComponentNodesFromNetwork(NODETAG_T &totalNodeVec, 
			std::vector<unsigned> &componentNodeVec, const unsigned startNode)
{
	std::queue<unsigned> nodeQue;
	nodeQue.push(startNode);

	while (!nodeQue.empty()) {
		unsigned u = nodeQue.front();
		nodeQue.pop();

		std::vector<unsigned> horiNodeVec, vertNodeVec;
		if (!network.getAjaOfNodeFromHoriCroLink(u, horiNodeVec) || !network.getAjaOfNodeFromVertCroLink(u, vertNodeVec)) {
			std::cerr << "get node from hori/vert cross-link error, u = " << u << std::endl;
			return false;
		}

		for (const auto &v : horiNodeVec)
			if (totalNodeVec[v]) {
				nodeQue.push(v);
				componentNodeVec.push_back(v);
				totalNodeVec[v] = false;
			}
		for (const auto &v : vertNodeVec) 
			if (totalNodeVec[v]) {
				nodeQue.push(v);
				componentNodeVec.push_back(v);
				totalNodeVec[v] = false;
			}
	}
	return true;
}

/*
 *	基于局部信息的相似性指标构造，这里更新的是固定相似度s矩阵
 *	CN AA RA
 *	注意：对于固定相似度而言，s_x_y 和 s_y_x 是一样的。而r_x_y 以及 a_x_y 不能逆推，因为逻辑不同。
 */
bool ApClusteringProcedure::getCnIndex(std::vector<unsigned> &compNodeVec, ApEdgeStorage &edgeStoreObj, const double preference)
{
	unsigned componentNodeSize = compNodeVec.size();

	for (unsigned i = 0; i < componentNodeSize; ++i) {

		if (i % 100 == 0) std::cout << "i = " << i << std::endl;		// test

		for (unsigned j = i; j < componentNodeSize; ++j) {

			unsigned x = compNodeVec.at(i), y = compNodeVec.at(j);
			if (x == y) {
				// s(k, k) 只插入一次即可
				if (!edgeStoreObj.insertEdgeToEdgesVec(x, x, ApElement(-preference, 0.0, 0.0))) {
					std::cerr << "getCnIndex: insertEdgeToEdgesVec() error, x = " << x << ", y = " << y << std::endl;
					return false;
				}
				continue;
			}

			std::vector<unsigned> horiVec, vertVec;
			if (!network.getAjaOfNodeFromHoriCroLink(x, horiVec) || !network.getAjaOfNodeFromVertCroLink(y, vertVec)) {
				std::cerr << "getCnIndex: getAjaOfNodeFrom Hori/Vert error, x = " << x << ", y = " << y << std::endl;
				return false;
			}
			double commonNode = 0.0;
			std::set<unsigned> vertSet(vertVec.begin(), vertVec.end());
			for (const auto &elem : horiVec)
				if (vertSet.find(elem) != vertSet.end())
					commonNode += 1.0;
			if (!edgeStoreObj.insertEdgeToEdgesVec(x, y, ApElement(-commonNode, 0.0, 0.0)) ||
				!edgeStoreObj.insertEdgeToEdgesVec(y, x, ApElement(-commonNode, 0.0, 0.0))) {
				std::cerr << "getCnIndex: insertEdgeToEdgesVec() error, x = " << x << ", y = " << y << std::endl;
				return false;
			}
		}
	}
	return true;
}
bool ApClusteringProcedure::getAaIndex(std::vector<unsigned> &comNodeVec, ApEdgeStorage &edgeStoreObj, const double preference)
{


	return true;
}
bool ApClusteringProcedure::getRaIndex(std::vector<unsigned> &comNodeVec, ApEdgeStorage &edgeStoreObj, const double preference)
{


	return true;
}

/*
 *	更新 responsibility，注意 r(i, k) != r(k, i) 并且 r(i, k)包含r(k, k)的情况
 */
bool ApClusteringProcedure::updateResponsibility(std::vector<unsigned> &comNodeVec, ApEdgeStorage &edgeStoreObj, const double lambda)
{
	// 先算 r(i, k)
	for (unsigned i = 0; i < comNodeVec.size(); ++i)
		for (unsigned k = 0; k < comNodeVec.size(); ++k) {
			if (i == k)
				continue;

			unsigned x = comNodeVec.at(i), y = comNodeVec.at(k);

			std::vector<unsigned> horiVec, vertVec;
			if (!network.getAjaOfNodeFromHoriCroLink(x, horiVec) || !network.getAjaOfNodeFromVertCroLink(x, vertVec)) {
				std::cerr << "getAjaOfNodeFromHoriCroLink() or VertCroLink() error." << std::endl;
				return false;
			}
			std::set<unsigned> newSet;
			for (const auto &u : horiVec)
				if (u != y)
					newSet.insert(u);
			for (const auto &u : vertVec)
				if (u != y)
					newSet.insert(u);

			// 找最大的 a(i, k`) + s(i, k`)
			double max_a_i_k_and_s_i_k = -1.7e10;

			for (const auto & v : newSet) {
				ApElement newObj;
				if (!edgeStoreObj.searchEdgeInEdgesVec(x, v, newObj)) {
					std::cerr << "searchEdgeInEdgesVec() in k` search error, " << x << ", " << v << std::endl;
					return false;
				}
				double this_a_i_k_and_s_i_k = newObj.getA() + newObj.getS();
				max_a_i_k_and_s_i_k = this_a_i_k_and_s_i_k > max_a_i_k_and_s_i_k ? this_a_i_k_and_s_i_k : max_a_i_k_and_s_i_k;
			}

			double old_r_i_k = 0.0, old_s_i_k = 0.0;
			if (!edgeStoreObj.getEdgeElemR(x, y, old_r_i_k)) {
				std::cerr << "getEdgeElemR(), x = " << x << ", y = " << y << " error." << std::endl;
				return false;
			}
			if (!edgeStoreObj.getEdgeElemS(x, y, old_s_i_k)) {
				std::cerr << "getEdgeElemS(), x = " << x << ", y = " << y << " error." << std::endl;
				return false;
			}
			
			double new_r_i_k = old_s_i_k - max_a_i_k_and_s_i_k;
			new_r_i_k = lambda * old_r_i_k + (1.0 - lambda) * new_r_i_k;

			if (!edgeStoreObj.modifyEdgeElemR(x, y, new_r_i_k)) {
				std::cerr << "modifyEdgeElemR(): x = " << x << ", y = " << y << " error." << std::endl;
				return false;
			}
		}

	//再算 r(k, k)
	for (unsigned k = 0; k < comNodeVec.size(); ++k) {
		unsigned x = comNodeVec.at(k);

		std::vector<unsigned> horiVec, vertVec;
		if (!network.getAjaOfNodeFromHoriCroLink(x, horiVec) || !network.getAjaOfNodeFromVertCroLink(x, vertVec)) {
			std::cerr << "getAjaOfNodeFromHoriCroLink() or VertCroLink() error." << std::endl;
			return false;
		}
		std::set<unsigned> newSet;
		for (const auto &u : horiVec)
			if (u != x)
				newSet.insert(u);
		for (const auto &u : vertVec)
			if (u != x)
				newSet.insert(u);
		
		double max_s_k_k = -1.7e10;
		for (const auto &v : newSet) {
			double this_s_k_k = 0.0;
			if (!edgeStoreObj.getEdgeElemS(x, v, this_s_k_k)) {
				std::cerr << "getEdgeElemS(): x = " << x << ", v = " << v << " error." << std::endl;
				return false;
			}
			max_s_k_k = this_s_k_k > max_s_k_k ? this_s_k_k : max_s_k_k;
		}

		double old_r_k_k = 0.0, old_s_k_k = 0.0;
		if (!edgeStoreObj.getEdgeElemR(x, x, old_r_k_k)) {
			std::cerr << "getEdgeElemR(): x = " << x << ", x = " << x << " error." << std::endl;
			return false;
		}
		if (!edgeStoreObj.getEdgeElemS(x, x, old_s_k_k)) {
			std::cerr << "getEdgeElemS(): x = " << x << ", x = " << x << " error." << std::endl;
			return false;
		}

		double new_r_k_k = old_s_k_k - max_s_k_k;
		new_r_k_k = lambda * old_r_k_k + (1.0 - lambda) * new_r_k_k;
		if (!edgeStoreObj.modifyEdgeElemR(x, x, new_r_k_k)) {
			std::cerr << "modifyEdgeElemR(), x = " << x << ", x = " << x << " error." << std::endl;
			return false;
		}
	}

	return true;
}

/* 
 *	更新 availability， a(i, k) != a(k, i), 且 a(k, k)区别计算
 */
bool ApClusteringProcedure::updateAvailability(std::vector<unsigned> &comNodeVec, ApEdgeStorage &edgStoreObj, const double lambda)
{
	// 先算 a(i, k)
	for (unsigned i = 0; i < comNodeVec.size(); ++i)
		for (unsigned k = 0; k < comNodeVec.size(); ++k) {
			if (i == k)
				continue;

			unsigned x = comNodeVec.at(i), y = comNodeVec.at(k);
			
			std::vector<unsigned> horiVec, vertVec;
			if (!network.getAjaOfNodeFromHoriCroLink(y, horiVec) ||
				!network.getAjaOfNodeFromVertCroLink(y, vertVec)) {
				std::cerr << "getAjaOfNodeFromHoriCroLink() or VertCroLink() error." << std::endl;
				return false;
			}

			std::set<unsigned> newSet;
			for (const auto &u : horiVec)
				if (u != x)	newSet.insert(u);
			for (const auto &u : vertVec)
				if (u != x) newSet.insert(u);

			double max_sum = 0.0;
			for (const auto &v : newSet) {
				double temp_r_i_k = 0.0;
				if (!edgStoreObj.getEdgeElemR(v, y, temp_r_i_k)) {
					std::cerr << "getEdgeElemR(), v = " << v << ", y = " << y << " error." << std::endl;
					return false;
				}

				if (temp_r_i_k > 0.0)
					max_sum += temp_r_i_k;
			}

			double temp_r_k_k = 0.0;
			if (!edgStoreObj.getEdgeElemR(y, y, temp_r_k_k)) {
				std::cerr << "getEdgeElemR(), y = " << y << ", y = " << y << " error." << std::endl;
				return false;
			}
			
			max_sum += temp_r_k_k;
			double new_a_i_k = max_sum < 0.0 ? max_sum : 0.0;

			double old_a_i_k = 0.0;
			if (!edgStoreObj.getEdgeElemA(x, y, old_a_i_k)) {
				std::cerr << "getEdgeElemA(), x = " << x << ", y = " << y << " error." << std::endl;
				return false;
			}

			new_a_i_k = lambda * old_a_i_k + (1.0 - lambda) * new_a_i_k;
			if (!edgStoreObj.modifyEdgeElemA(x, y, new_a_i_k)) {
				std::cerr << "modifyEdgeElemA(), x = " << x << ", y = " << y << " error." << std::endl;
				return false;
			}
		}	// second - for

	// 再算 a(k, k)
	for (unsigned i = 0; i < comNodeVec.size(); ++i) {
		unsigned x = comNodeVec.at(i);

		std::vector<unsigned> horiVec, vertVec;
		if (!network.getAjaOfNodeFromHoriCroLink(x, horiVec) ||
			!network.getAjaOfNodeFromVertCroLink(x, vertVec)) {
			std::cerr << "getAjaOfNodeFromHoriCroLink() or VertCroLink() error." << std::endl;
			return false;
		}

		std::set<unsigned> newSet(horiVec.begin(), horiVec.end());
		for (const auto &u : vertVec)
			newSet.insert(u);

		double sum_r_i_k = 0.0;					// new_a_k_k
		for (const auto &v : newSet) {
			double temp_r_i_k = 0.0;
			if (!edgStoreObj.getEdgeElemR(v, x, temp_r_i_k)) {
				std::cerr << "getEdgeElemR(), v = " << v << ", x = " << x << " error." << std::endl;
				return false;
			}
			if (temp_r_i_k > 0.0)
				sum_r_i_k += temp_r_i_k;			
			
			// 要检查下能否正确判断  从这里测试！！！！！！！！！！！！！！！！！！！！！！！！！
			// r 和 a 都要测试是否正确 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}

		double old_a_k_k = 0.0;
		if (!edgStoreObj.getEdgeElemA(x, x, old_a_k_k)) {
			std::cerr << "getEdgeElemA(): x = " << x << ", x = " << x << " error." << std::endl;
			return false;
		}
		double new_a_k_k = lambda * sum_r_i_k + (1.0 - lambda) * old_a_k_k;

		if (!edgStoreObj.modifyEdgeElemA(x, x, new_a_k_k)) {
			std::cerr << "modifyEdgeElemA(), x = " << x << ", x = " << x << " error." << std::endl;
			return false;
		}
	}
	return true;
}

/*
 *	获取本轮推荐点    这个函数有问题！
 */
bool ApClusteringProcedure::getRecommendNode(ApEdgeStorage &edgStoreObj, std::vector<unsigned> &comNodeVec, std::vector<unsigned> &recommendVec)
{
	unsigned networkSize = network.getCapacity();
	std::vector<bool> nodeBoolVec(networkSize + 1, false);

	for (const auto &u : comNodeVec) {
		std::vector<unsigned> horiVec, vertVec;

		if (!network.getAjaOfNodeFromHoriCroLink(u, horiVec) || !network.getAjaOfNodeFromVertCroLink(u, vertVec)) {
			std::cerr << "getAjaOfNodeFrom Hori/Vert() error." << std::endl;
			return false;
		}
		std::set<unsigned> newSet(horiVec.begin(), horiVec.end());
		for (const auto &v : vertVec)
			newSet.insert(v);
		newSet.insert(u);

		unsigned maxNode = 0;	double maxValue = -1.7e10;
		for (const auto &v : newSet) {
			double a_i_k = 0.0, r_i_k = 0.0;
			if (!edgStoreObj.getEdgeElemA(u, v, a_i_k) || !edgStoreObj.getEdgeElemR(u, v, r_i_k)) {
				std::cerr << "getEdgeElemA() or getEdgeElemR() error." << std::endl;
				return false;
			}
			//std::cout << "a_i_k = " << a_i_k << std::endl;  // test
			//std::cout << "r_i_k = " << r_i_k << std::endl;	// test
			//std::cout << "maxValue = " << maxValue << std::endl; // test
			//if (a_i_k + r_i_k > maxValue)
			//	std::cout << "greater than.." << std::endl;
		
			if (a_i_k + r_i_k > maxValue) {
				maxValue = a_i_k + r_i_k;
				maxNode = v;
			}
		}
		nodeBoolVec.at(maxNode) = true;
	}

	for (unsigned i = 1; i <= networkSize; ++i)
		if (nodeBoolVec.at(i))
			recommendVec.push_back(i);
	return true;
}


/*
 *	test test test test test test test test test test test test test test test test test test test test 
 */
void ApClusteringProcedure::showNetwork()
{
	network.showCroLink();
}

bool ApClusteringProcedure::readSimilarityFromFile(const std::string &file, ApEdgeStorage &edgeObj, const double preference)
{
	std::fstream inputFile;
	inputFile.open(file, std::fstream::in);
	if (!inputFile.is_open()) {
		std::cerr << "open() error: " << file << std::endl;
		return false;
	}
	
	unsigned i = 0, j = 0;		double simi = 0.0;
	unsigned lineCounter = 0;

	while (inputFile >> i >> j >> simi) {	
		ApElement obj(simi, 0.0, 0.0);
		if (!edgeObj.insertEdgeToEdgesVec(i, j, obj)) {
			std::cerr << "insertEdgeToEdgesVec() error, i = " << i << ", j = " << j << std::endl;
			return false;
		}
		++lineCounter;
	}

	inputFile.close();
	
	std::cout << "readSimilarityFromFile() finish, line = " << lineCounter << std::endl;

	for (unsigned i = 1; i <= 25; ++i) {
		ApElement obj(preference, 0.0, 0.0);
		if (!edgeObj.insertEdgeToEdgesVec(i, i, obj)) {
			std::cerr << "insertEdgeToEdgesVec() error, i = " << i << ", i = " << i << std::endl;
			return false;
		}
	}

	std::cout << "readSimilarityFromFile() finish.." << std::endl;
	//getchar();

	return true;
}

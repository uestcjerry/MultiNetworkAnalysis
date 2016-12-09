#ifndef APCLUSTERINGPROCEDURE_H
#define APCLUSTERINGPROCEDURE_H

#include "../crossLink/CrossLink.h"
#include "../apCluster/ApClusterStorageDefinition.h"

/*
*	Ap-Clustering
*/
class ApClusteringProcedure {
public:
	typedef std::vector<bool>					NODETAG_T;

	ApClusteringProcedure() = default;
	~ApClusteringProcedure() = default;

public:
	bool buildNetwork(const std::string &);		//参数是文件完整路径名
	void clearNetwork();

	
public:
	// ap 聚类调用函数
	bool apClusteringProcedure();				// total function

public:
	// 指定完整数据文件名，开始ap聚类，这里是带时间戳的数据，并且只处理较大连通分量
	bool apClusteringFromFile(const std::string &, const double, const double);
	// 标记当前network中的所有节点
	bool tagAllNetworkNodes(NODETAG_T &, unsigned &);
	// 选取以start开始的连通分量
	bool getComponentNodesFromNetwork(NODETAG_T &, std::vector<unsigned> &, const unsigned);

	// 基于局部信息的相似性指标构造，这里更新的是固定相似度s矩阵
	// 注意：对于固定相似度而言，s_x_y 和 s_y_x 是一样的。而r_x_y 以及 a_x_y 不能逆推，因为逻辑不同。
	bool getCnIndex(std::vector<unsigned> &, ApEdgeStorage &, const double);
	bool getAaIndex(std::vector<unsigned> &, ApEdgeStorage &, const double);
	bool getRaIndex(std::vector<unsigned> &, ApEdgeStorage &, const double);

	// 更新 r
	bool updateResponsibility(std::vector<unsigned> &, ApEdgeStorage &, const double);

	// 更新 a
	bool updateAvailability(std::vector<unsigned> &, ApEdgeStorage &, const double);
	
	// 找出本轮中所推荐出的点
	bool getRecommendNode(ApEdgeStorage &, std::vector<unsigned> &, std::vector<unsigned> &);


public:
	// 测试函数
	void showNetwork();
	bool readSimilarityFromFile(const std::string &, ApEdgeStorage &, const double);


private:
	CrossLink network;							// 仅仅取较大的连通分量进行分析
};

#endif
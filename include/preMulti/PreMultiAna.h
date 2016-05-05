#ifndef PREMULTIANA_H
#define PREMULTIANA_H

#include "../../include/crossLink/CrossLink.h"

#include <string>
#include <vector>
#include <map>

/* 
 *	接下来做Pre的多层网络分析
 *	需要存所有多层网络
 */

/*
 *	单层网络对象
 */
class PreMultiLayer {
public:
	typedef std::vector<unsigned> nodeActiVec_t;

	PreMultiLayer();
	~PreMultiLayer();
public:
	bool initLayerObj(const std::string &);
	void clearLayerObj();
	void showLayerObj();

	void setLayerId(const unsigned);
	unsigned getLayerId() const;

	unsigned getLayerCapacity() const;

	// 初始化node activity vector，即bi向量 
	bool initNodeActiVec();

	// 寻找当前层中对应下标的节点，是否活跃
	bool getNodeActiVecAt(const unsigned, unsigned &);

private:
	CrossLink layerObj;						//自带capacity 属性表示最大角标（节点编号）范围 1~capacity
	unsigned layerId;						//根据文件大小排序作为编号 0 ~ 21

	// node activity vector b_i[userNum + 1]; 注意在使用这个对象之前，要调用 initNodeActiVec()，有效位1 ~ layerObj.capacity
	// 这个是这层的点的活跃度向量，从 1 ~ maxNodeId，注意区分某个点的活跃度 bi 和这个是不同的
	nodeActiVec_t nodeActiVec;
};
/*
 *	多层网络
 */
class PreMultiLayerManage {
public:
	// NodeActivity.cpp
	typedef std::vector<PreMultiLayer> PreMultiLayerManage_t;

	PreMultiLayerManage();
	~PreMultiLayerManage();

	void setMultiNum(const unsigned);
	const unsigned getMultiNum() const;

	// 使用Manage对象之前，需调用这个init() !
	bool initMultiLayerObj(const std::string &, const std::vector<std::string> &);
	void clearMultiLayerObj();
	void showMultiLayerObj();

	//readdir() 获取所有的源数据文件 不过这里还是按多层网络的大小来统计吧，统一化，这个暂时不用
	bool getSrcFile(const std::string &, std::vector<std::string> &);

public:
	//分析过程写这里 一次指标分析调用相应地总方法即可

	//	节点活跃度 node activity Bi
	//	NodeActivity.cpp
	bool calNodeActiDisOfBi(const std::string &);			//revoke this
	
	bool initNodeActiBeforeBi();							//先计算所有层的 bi ，再计算所有点的 Bi，这里调用了initNodeActiVec()
	bool findMaxNodeIdFromMultiNet(unsigned &);
	bool writeFileOfNodeActi(const std::string &, const std::vector<std::pair<unsigned, unsigned>> &);

	bool handleTheResultFromNodeActi(const std::string &, const std::string &, const double);	//分析 node activity Bi 结果文件，取对数画图

	// 分析节点活跃度向量 bi 的分布 
	// Rank(bi) && Rank(bi | Bi) (已知Bi的情况下)
	// RankbiAndbiBi.cpp
	bool calNodeVecDisOfbiAndbiBi(const std::string &);
	
	bool writeFileOfRankbi(const std::string &, const std::map<unsigned, unsigned> &);
	bool writeFileOfRankbiBi(const std::string &, const std::vector<std::pair<unsigned, std::map<unsigned, unsigned > > > &);


	// 层活跃度分析 Layer activity
	// Layer acitivity
	// continue..




private:
	PreMultiLayerManage(const PreMultiLayerManage &) = delete;
	PreMultiLayerManage &operator=(const PreMultiLayerManage &) = delete;
private:
	PreMultiLayerManage_t multiLayerObj;				//这个长度等于多层网络个数，有效角标范围 [0, multiLayerNum)
	unsigned multiLayerNum;								//多层网络层数			
};

bool preMultiAnalysisRevokeThis();			//after all, revoke this function

#endif
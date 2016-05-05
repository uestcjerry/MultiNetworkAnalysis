#ifndef PREMULTIANA_H
#define PREMULTIANA_H

#include "../../include/crossLink/CrossLink.h"

#include <string>
#include <vector>

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

	//获取bi的某个位
	bool getNodeActiVecAt(const unsigned, unsigned &);

private:
	CrossLink layerObj;						//自带capacity 属性表示最大角标（节点编号）范围 1~capacity
	unsigned layerId;						//根据文件大小排序作为编号 0 ~ 21

	nodeActiVec_t nodeActiVec;				// node activity vector b_i[userNum + 1]; 注意要调用initNodeActiVec()，有效位1~capacity
};

/*
 *	多层网络
 */
class PreMultiLayerManage {
public:
	typedef std::vector<PreMultiLayer> PreMultiLayerManage_t;

	PreMultiLayerManage();
	~PreMultiLayerManage();

	void setMultiNum(const unsigned);
	const unsigned getMultiNum() const;

	bool initMultiLayerObj(const std::string &, const std::vector<std::string> &);
	void clearMultiLayerObj();
	void showMultiLayerObj();

public:
	//分析过程写这里 一次指标分析调用相应地总方法即可

	//	节点活跃度 node activity Bi
	//	NodeActivity.cpp
	bool calNodeActiDisOfBi(const std::string &);			//revoke this
	bool initNodeActiBeforeBi();							//先计算所有层的 bi ，再计算所有点的 Bi
	bool findMaxNodeIdFromMultiNet(unsigned &);
	bool writeFileOfNodeActi(const std::string &, const std::vector<std::pair<unsigned, unsigned>> &);

	bool handleTheResultFromNodeActi(const std::string &, const std::string &, const double);		//分析 node activity Bi 结果文件，取对数画图


	// 分析节点活跃度向量 bi 的分布 
	// Rank(bi) && Rank(bi | Bi) (已知Bi的情况下) 


private:
	PreMultiLayerManage(const PreMultiLayerManage &) = delete;
	PreMultiLayerManage &operator=(const PreMultiLayerManage &) = delete;
private:
	PreMultiLayerManage_t multiLayerObj;
	unsigned multiLayerNum;
};

bool preMultiAnalysisRevokeThis();			//after all, revoke this function

#endif
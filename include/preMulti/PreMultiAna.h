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
	PreMultiLayer();
	~PreMultiLayer();
public:
	bool initLayerObj(const std::string &);
	void clearLayerObj();
	void showLayerObj();

	void setLayerId(const unsigned);
	unsigned getLayerId() const;

private:
	//PreMultiLayer(const PreMultiLayer &) = delete;
	//PreMultiLayer & operator=(const PreMultiLayer &) = delete;	

private:
	CrossLink layerObj;						//自带capacity 属性表示最大角标（节点编号）范围 1~capacity
	unsigned layerId;						//根据文件大小排序作为编号 0 ~ 21
};

/*
 *	多层网络
 */
class PreMultiLayerManage {
public:
	typedef std::vector<PreMultiLayer> PreMultiLayerManage_t;

	PreMultiLayerManage();
	~PreMultiLayerManage();
public:
	void setMultiNum(const unsigned);
	const unsigned getMultiNum() const;

	bool initMultiLayerObj(const std::string &, const std::vector<std::string> &);
	void clearMultiLayerObj();
	void showMultiLayerObj();


private:
	PreMultiLayerManage(const PreMultiLayerManage &) = delete;
	PreMultiLayerManage &operator=(const PreMultiLayerManage &) = delete;
private:
	PreMultiLayerManage_t multiLayerObj;
	unsigned multiLayerNum;
};

bool preMultiAnalysisRevokeThis();			//after all, revoke this function

#endif
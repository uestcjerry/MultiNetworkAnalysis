#ifndef PREMULTIANA_H
#define PREMULTIANA_H

#include "../../include/crossLink/CrossLink.h"

#include <string>
#include <vector>


/* 
 *	��������Pre�Ķ���������
 *	��Ҫ�����ж������
 */

/*
 *	�����������
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
	CrossLink layerObj;						//�Դ�capacity ���Ա�ʾ���Ǳ꣨�ڵ��ţ���Χ 1~capacity
	unsigned layerId;						//�����ļ���С������Ϊ��� 0 ~ 21
};

/*
 *	�������
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
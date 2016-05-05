#ifndef PREMULTIANA_H
#define PREMULTIANA_H

#include "../../include/crossLink/CrossLink.h"

#include <string>
#include <vector>
#include <map>

/* 
 *	��������Pre�Ķ���������
 *	��Ҫ�����ж������
 */

/*
 *	�����������
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

	// ��ʼ��node activity vector����bi���� 
	bool initNodeActiVec();

	// Ѱ�ҵ�ǰ���ж�Ӧ�±�Ľڵ㣬�Ƿ��Ծ
	bool getNodeActiVecAt(const unsigned, unsigned &);

private:
	CrossLink layerObj;						//�Դ�capacity ���Ա�ʾ���Ǳ꣨�ڵ��ţ���Χ 1~capacity
	unsigned layerId;						//�����ļ���С������Ϊ��� 0 ~ 21

	// node activity vector b_i[userNum + 1]; ע����ʹ���������֮ǰ��Ҫ���� initNodeActiVec()����Чλ1 ~ layerObj.capacity
	// ��������ĵ�Ļ�Ծ���������� 1 ~ maxNodeId��ע������ĳ����Ļ�Ծ�� bi ������ǲ�ͬ��
	nodeActiVec_t nodeActiVec;
};
/*
 *	�������
 */
class PreMultiLayerManage {
public:
	// NodeActivity.cpp
	typedef std::vector<PreMultiLayer> PreMultiLayerManage_t;

	PreMultiLayerManage();
	~PreMultiLayerManage();

	void setMultiNum(const unsigned);
	const unsigned getMultiNum() const;

	// ʹ��Manage����֮ǰ����������init() !
	bool initMultiLayerObj(const std::string &, const std::vector<std::string> &);
	void clearMultiLayerObj();
	void showMultiLayerObj();

	//readdir() ��ȡ���е�Դ�����ļ� �������ﻹ�ǰ��������Ĵ�С��ͳ�ưɣ�ͳһ���������ʱ����
	bool getSrcFile(const std::string &, std::vector<std::string> &);

public:
	//��������д���� һ��ָ�����������Ӧ���ܷ�������

	//	�ڵ��Ծ�� node activity Bi
	//	NodeActivity.cpp
	bool calNodeActiDisOfBi(const std::string &);			//revoke this
	
	bool initNodeActiBeforeBi();							//�ȼ������в�� bi ���ټ������е�� Bi�����������initNodeActiVec()
	bool findMaxNodeIdFromMultiNet(unsigned &);
	bool writeFileOfNodeActi(const std::string &, const std::vector<std::pair<unsigned, unsigned>> &);

	bool handleTheResultFromNodeActi(const std::string &, const std::string &, const double);	//���� node activity Bi ����ļ���ȡ������ͼ

	// �����ڵ��Ծ������ bi �ķֲ� 
	// Rank(bi) && Rank(bi | Bi) (��֪Bi�������)
	// RankbiAndbiBi.cpp
	bool calNodeVecDisOfbiAndbiBi(const std::string &);
	
	bool writeFileOfRankbi(const std::string &, const std::map<unsigned, unsigned> &);
	bool writeFileOfRankbiBi(const std::string &, const std::vector<std::pair<unsigned, std::map<unsigned, unsigned > > > &);


	// ���Ծ�ȷ��� Layer activity
	// Layer acitivity
	// continue..




private:
	PreMultiLayerManage(const PreMultiLayerManage &) = delete;
	PreMultiLayerManage &operator=(const PreMultiLayerManage &) = delete;
private:
	PreMultiLayerManage_t multiLayerObj;				//������ȵ��ڶ�������������Ч�Ǳ귶Χ [0, multiLayerNum)
	unsigned multiLayerNum;								//����������			
};

bool preMultiAnalysisRevokeThis();			//after all, revoke this function

#endif
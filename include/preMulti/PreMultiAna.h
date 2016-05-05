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

	//��ȡbi��ĳ��λ
	bool getNodeActiVecAt(const unsigned, unsigned &);

private:
	CrossLink layerObj;						//�Դ�capacity ���Ա�ʾ���Ǳ꣨�ڵ��ţ���Χ 1~capacity
	unsigned layerId;						//�����ļ���С������Ϊ��� 0 ~ 21

	nodeActiVec_t nodeActiVec;				// node activity vector b_i[userNum + 1]; ע��Ҫ����initNodeActiVec()����Чλ1~capacity
};

/*
 *	�������
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
	//��������д���� һ��ָ�����������Ӧ���ܷ�������

	//	�ڵ��Ծ�� node activity Bi
	//	NodeActivity.cpp
	bool calNodeActiDisOfBi(const std::string &);			//revoke this
	bool initNodeActiBeforeBi();							//�ȼ������в�� bi ���ټ������е�� Bi
	bool findMaxNodeIdFromMultiNet(unsigned &);
	bool writeFileOfNodeActi(const std::string &, const std::vector<std::pair<unsigned, unsigned>> &);

	bool handleTheResultFromNodeActi(const std::string &, const std::string &, const double);		//���� node activity Bi ����ļ���ȡ������ͼ


	// �����ڵ��Ծ������ bi �ķֲ� 
	// Rank(bi) && Rank(bi | Bi) (��֪Bi�������) 


private:
	PreMultiLayerManage(const PreMultiLayerManage &) = delete;
	PreMultiLayerManage &operator=(const PreMultiLayerManage &) = delete;
private:
	PreMultiLayerManage_t multiLayerObj;
	unsigned multiLayerNum;
};

bool preMultiAnalysisRevokeThis();			//after all, revoke this function

#endif
#ifndef CROSSLINK_H
#define CROSSLINK_H

#include "../basicDef/BasicStruct.h"

#include <utility>
#include <set>


class CrossLink {
public:
	typedef std::multiset<BasicEdge> CrossLink_t;

	CrossLink();
	~CrossLink();

	bool testCrossLink();										//test finish.

	unsigned getCapacity();
	const unsigned getCapacity() const;

	void deconsCrossLinkObj();
																//first consCross(), second readFile()
	bool consCrossLinkObj(const std::string &);					//renew capacity , after decons , call this

	bool readFileConsCrosLink(const std::string &);				//revoke this to initialize CrossLink obj. argument is total path of input file.

	bool findMaxNodeTag(const std::string &, unsigned &);
	bool findMaxNodeTagTime(const std::string &, unsigned &);
	
	bool insertHoriCroLink(const BasicEdge &);
	bool insertVertCroLink(const BasicEdge &);

	bool searchHoriCroLink(const BasicEdge &);
	//bool searchVertCroLink(const BasicEdge &);				//�߼���

	bool getAjaOfNodeFromHoriCroLink(const _NodeTag_t, std::vector<_NodeTag_t> &);			//�ڽӱ�
	bool getAjaOfNodeFromVertCroLink(const _NodeTag_t, std::vector<_NodeTag_t> &);			//���ڽӱ�

	bool getAjaFromHoriCroLink(const BasicEdge &, std::vector<_NodeTag_t> &);
	bool getAjaFromVertCroLink(const BasicEdge &, std::vector<_NodeTag_t> &);
	
	bool getAjaSizeFromHori(const _NodeTag_t, unsigned &);							// get out-degree of node u ����
	bool getAjaSizeFromVert(const _NodeTag_t, unsigned &);							// get in-degree of node u	���
	unsigned getDegreeAsUndi(const _NodeTag_t, unsigned &);								//������ͼ�� ȡ�ȣ�����+��ȣ�

	bool getAjaOfTimeFromHori(const _NodeTag_t, std::vector<Time> &);
	bool getAjaOfTimeFromVert(const _NodeTag_t, std::vector<Time> &);

	bool deleteUndiEdge(const _NodeTag_t, const _NodeTag_t);						//k-core-decomposition ɾ����

	void showCroLink();
private:
	std::vector<std::pair<CrossLink_t, CrossLink_t>> crossLinkObj;  //horizontal ��, vertical ��, �Ǳ��ʾ�ڵ�ţ�Լ����һ��Ϊ��x �ڶ���Ϊ��y
	unsigned capacity;												//���Ǳ�������  ��ȡ���ȣ���ȡ����� ��Χ 1~capacity

private:
	//CrossLink(const CrossLink &) = delete;						//�������캯���Ϳ�����ֵ�����  delete��Ӱ��
	//CrossLink& operator=(const CrossLink &) = delete;
};

#endif
#ifndef CROSSLINK_H
#define CROSSLINK_H

#include "../basicDef/BasicStruct.h"

#include <utility>
#include <set>


class CrossLink {
public:
	
	// here !

	// �����￪ʼ������ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// ����������������ܹ������ң�  ����ɢ�б��! 
	// ���� crossklinkObj �ĽṹҲ�� !!!!!!!!!!!

	// �ĳ� unordered_set<BasicEdge>  ����BasicEdge�� == ���������


	typedef std::multiset<BasicEdge> CrossLink_T;		

	CrossLink() = default;
	~CrossLink() = default;

	// test
	bool testCrossLink();

	// ��ȡ�����ģ��С�������Ǳ��С
	unsigned getCapacity() const;

	// ����STL����
	void deconsCrossLinkObj();
						
	// ��ʼ�� first consCross(), second readFile()
	bool consCrossLinkObj(const std::string &);					//renew capacity , after decons , call this

	// ��ȡ��ʱ�����ת�����磬�������磬����������·����
	bool readFileConsCrosLink(const std::string &);

	// ��ȡ��ʱ�����ת�����磬�����޶�ʱ��
	bool readFileConsCrosLinkBeforTime(const std::string &, const Time &, const long long);		//min time, day

	// ��ȡ���ڵ�ţ�����ʱ������ݣ���ʱ�������
	bool findMaxNodeTag(const std::string &, unsigned &);
	bool findMaxNodeTagTime(const std::string &, unsigned &);
	
	// ���뵽ʮ�������ڽӺ����ڽ�
	bool insertHoriCroLink(const BasicEdge &);
	bool insertVertCroLink(const BasicEdge &);

	// �����ڽӱ�
	bool searchHoriCroLink(const BasicEdge &);
	//bool searchVertCroLink(const BasicEdge &);									//�߼���

	// ����nodeid��ȡ�ھ��Լ����ھ�
	bool getAjaOfNodeFromHoriCroLink(const _NodeTag_t, std::vector<_NodeTag_t> &);	//�ڽӱ�
	bool getAjaOfNodeFromVertCroLink(const _NodeTag_t, std::vector<_NodeTag_t> &);	//���ڽӱ�

	// ��Ѱ�����㹲ͬ���ھ�                                    δ���  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	bool getCommonAjacencyOfXandY(const _NodeTag_t, const _NodeTag_t, std::vector<_NodeTag_t> &);

	// ����BasicEdge�����ȡ�ھ��Լ����ھ�
	bool getAjaFromHoriCroLink(const BasicEdge &, std::vector<_NodeTag_t> &);
	bool getAjaFromVertCroLink(const BasicEdge &, std::vector<_NodeTag_t> &);
	
	// ����nodeid��ȡ�ھ��Լ����ھӵĴ�С
	bool getAjaSizeFromHori(const _NodeTag_t, _NodeTag_t &);							// get out-degree of node u ����
	bool getAjaSizeFromVert(const _NodeTag_t, _NodeTag_t &);							// get in-degree of node u	���
	
	// ��������ͼ��������û�д����Լ����Լ���ת�� eg: 300 300
	bool getDegreeAsUndi(const _NodeTag_t, _NodeTag_t &);							//������ͼ�� ȡ�ȣ�����+��ȣ� ����

	// ����nodeid��ȡ�ھӺ����ھӵ�ʱ��
	bool getAjaOfTimeFromHori(const _NodeTag_t, std::vector<Time> &);
	bool getAjaOfTimeFromVert(const _NodeTag_t, std::vector<Time> &);

	// ɾ��<a, b>�����ıߣ�k-core�ֽ�����
	bool deleteUndiEdge(const _NodeTag_t, const _NodeTag_t);						//k-core-decomposition ɾ����

	// test
	void showCroLink();
private:
	std::vector<std::pair<CrossLink_T, CrossLink_T>> crossLinkObj;  //horizontal ��, vertical ��, �Ǳ��ʾ�ڵ�ţ�Լ����һ��Ϊ��x �ڶ���Ϊ��y
	unsigned capacity;												//���Ǳ�������  ��ȡ���ȣ���ȡ����� ��Χ 1~capacity
};

#endif
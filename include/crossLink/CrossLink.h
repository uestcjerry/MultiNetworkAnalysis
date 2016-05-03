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
	//bool searchVertCroLink(const BasicEdge &);				//逻辑？

	bool getAjaOfNodeFromHoriCroLink(const _NodeTag_t, std::vector<_NodeTag_t> &);			//邻接表
	bool getAjaOfNodeFromVertCroLink(const _NodeTag_t, std::vector<_NodeTag_t> &);			//逆邻接表

	bool getAjaFromHoriCroLink(const BasicEdge &, std::vector<_NodeTag_t> &);
	bool getAjaFromVertCroLink(const BasicEdge &, std::vector<_NodeTag_t> &);
	
	bool getAjaSizeFromHori(const _NodeTag_t, unsigned &);							// get out-degree of node u 出度
	bool getAjaSizeFromVert(const _NodeTag_t, unsigned &);							// get in-degree of node u	入度
	unsigned getDegreeAsUndi(const _NodeTag_t, unsigned &);								//当无向图， 取度（出度+入度）

	bool getAjaOfTimeFromHori(const _NodeTag_t, std::vector<Time> &);
	bool getAjaOfTimeFromVert(const _NodeTag_t, std::vector<Time> &);

	bool deleteUndiEdge(const _NodeTag_t, const _NodeTag_t);						//k-core-decomposition 删除边

	void showCroLink();
private:
	std::vector<std::pair<CrossLink_t, CrossLink_t>> crossLinkObj;  //horizontal 横, vertical 竖, 角标表示节点号，约定第一个为横x 第二个为竖y
	unsigned capacity;												//最大角标做容量  横取出度，竖取的入度 范围 1~capacity

private:
	//CrossLink(const CrossLink &) = delete;						//拷贝构造函数和拷贝赋值运算符  delete有影响
	//CrossLink& operator=(const CrossLink &) = delete;
};

#endif
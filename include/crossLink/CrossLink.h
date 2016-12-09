#ifndef CROSSLINK_H
#define CROSSLINK_H

#include "../basicDef/BasicStruct.h"

#include <utility>
#include <set>


class CrossLink {
public:
	
	// here !

	// 从这里开始继续改 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 重新想象怎样存边能够最快查找？  利用散列表吧! 
	// 底下 crossklinkObj 的结构也改 !!!!!!!!!!!

	// 改成 unordered_set<BasicEdge>  重载BasicEdge的 == 运算符即可


	typedef std::multiset<BasicEdge> CrossLink_T;		

	CrossLink() = default;
	~CrossLink() = default;

	// test
	bool testCrossLink();

	// 获取网络规模大小，即最大角标大小
	unsigned getCapacity() const;

	// 清理STL容器
	void deconsCrossLinkObj();
						
	// 初始化 first consCross(), second readFile()
	bool consCrossLinkObj(const std::string &);					//renew capacity , after decons , call this

	// 读取带时间戳的转发网络，构造网络，参数是完整路径名
	bool readFileConsCrosLink(const std::string &);

	// 读取带时间戳的转发网络，并且限定时间
	bool readFileConsCrosLinkBeforTime(const std::string &, const Time &, const long long);		//min time, day

	// 获取最大节点号，不带时间戳数据，带时间戳数据
	bool findMaxNodeTag(const std::string &, unsigned &);
	bool findMaxNodeTagTime(const std::string &, unsigned &);
	
	// 插入到十字链，邻接和逆邻接
	bool insertHoriCroLink(const BasicEdge &);
	bool insertVertCroLink(const BasicEdge &);

	// 查找邻接表
	bool searchHoriCroLink(const BasicEdge &);
	//bool searchVertCroLink(const BasicEdge &);									//逻辑？

	// 根据nodeid获取邻居以及逆邻居
	bool getAjaOfNodeFromHoriCroLink(const _NodeTag_t, std::vector<_NodeTag_t> &);	//邻接表
	bool getAjaOfNodeFromVertCroLink(const _NodeTag_t, std::vector<_NodeTag_t> &);	//逆邻接表

	// 找寻两个点共同的邻居                                    未完成  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	bool getCommonAjacencyOfXandY(const _NodeTag_t, const _NodeTag_t, std::vector<_NodeTag_t> &);

	// 根据BasicEdge对象获取邻居以及逆邻居
	bool getAjaFromHoriCroLink(const BasicEdge &, std::vector<_NodeTag_t> &);
	bool getAjaFromVertCroLink(const BasicEdge &, std::vector<_NodeTag_t> &);
	
	// 根据nodeid获取邻居以及逆邻居的大小
	bool getAjaSizeFromHori(const _NodeTag_t, _NodeTag_t &);							// get out-degree of node u 出度
	bool getAjaSizeFromVert(const _NodeTag_t, _NodeTag_t &);							// get in-degree of node u	入度
	
	// 当做无向图处理，这里没有处理自己到自己的转发 eg: 300 300
	bool getDegreeAsUndi(const _NodeTag_t, _NodeTag_t &);							//当无向图， 取度（出度+入度） 修正

	// 根据nodeid获取邻居和逆邻居的时间
	bool getAjaOfTimeFromHori(const _NodeTag_t, std::vector<Time> &);
	bool getAjaOfTimeFromVert(const _NodeTag_t, std::vector<Time> &);

	// 删除<a, b>这样的边，k-core分解所需
	bool deleteUndiEdge(const _NodeTag_t, const _NodeTag_t);						//k-core-decomposition 删除边

	// test
	void showCroLink();
private:
	std::vector<std::pair<CrossLink_T, CrossLink_T>> crossLinkObj;  //horizontal 横, vertical 竖, 角标表示节点号，约定第一个为横x 第二个为竖y
	unsigned capacity;												//最大角标做容量  横取出度，竖取的入度 范围 1~capacity
};

#endif
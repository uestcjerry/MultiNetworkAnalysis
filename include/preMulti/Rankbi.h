#ifndef RANKBI_H
#define RANKBI_H

#include <vector>

/*
 *	对于每个节点，需要保存 bi 相关信息
 *	Rankbi 对象的个数等于节点个数
 */
class Rankbi {
public:
	Rankbi();
	Rankbi(const unsigned);
	Rankbi(const Rankbi &);
	Rankbi & operator=(const Rankbi &);
	~Rankbi();

	void setNodeId(const unsigned);
	const unsigned getNodeId() const;
	
	void setbi(const std::vector<unsigned> &);
	void setMultilayerNum(const unsigned);
	const unsigned getMultilayerNum() const;

	void setRankbi(const unsigned);
	const unsigned getRankbi() const;

	void setBi(const unsigned);
	const unsigned getBi() const;
public:
	bool calRankbiAndBiOfThisNode();	//拥有前三个数据之后，计算后两个数据

private:
	unsigned nodeId;

	std::vector<unsigned> bi;			//这个数组的长度应该和多层网络的层数一样  capacity
	unsigned multilayNum;				//长度等于总多层网络的层数，注意正确初始化

	unsigned rankbi;					//根据层数，一共有 2^M - 1 种排序组合，每个点的 bi 向量从低位到高位对应一种排序组合，类似进制数，作为rank值
	unsigned Bi;						//bi向量中非零的个数，表示节点活跃度
		
};


#endif
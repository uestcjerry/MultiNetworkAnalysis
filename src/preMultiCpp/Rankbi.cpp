#include "../../include/preMulti/Rankbi.h"

Rankbi::Rankbi() : nodeId(0), multilayNum(0), rankbi(0), Bi(0) {}

Rankbi::Rankbi(const unsigned node) : nodeId(node), multilayNum(0), rankbi(0), Bi(0) {}

Rankbi::Rankbi(const Rankbi &u) 
{
	nodeId = u.nodeId;
	bi = u.bi;
	multilayNum = u.multilayNum;
	rankbi = u.rankbi;
	Bi = u.Bi;
}
Rankbi & Rankbi::operator = (const Rankbi &u)
{
	if (this == &u)
		return *this;
	nodeId = u.nodeId;
	bi = u.bi;
	multilayNum = u.multilayNum;
	rankbi = u.rankbi;
	Bi = u.Bi;
	return *this;
}
Rankbi::~Rankbi() {}

void Rankbi::setNodeId(const unsigned u) { nodeId = u; }

const unsigned Rankbi::getNodeId() const { return nodeId; }

void Rankbi::setbi(const std::vector<unsigned> &u) { bi = u; }

void Rankbi::setMultilayerNum(const unsigned u)	{ multilayNum = u; }

const unsigned Rankbi::getMultilayerNum() const	{ return multilayNum; }

void Rankbi::setRankbi(const unsigned u) { rankbi = u; }

const unsigned Rankbi::getRankbi() const  { return rankbi; }

void Rankbi::setBi(const unsigned u) { Bi = u; }

const unsigned Rankbi::getBi() const { return Bi; }

bool Rankbi::calRankbiAndBiOfThisNode()
{
	if (multilayNum == 0 || bi.size() == 0)
		return false;

	unsigned tempBi = 0;
	for (unsigned i = 0; i < multilayNum; ++i)
		if (bi.at(i) != 0)
			++tempBi;
	Bi = tempBi;

	unsigned tempbi = 0, tempMulti = 1;
	for (unsigned i = 0; i < multilayNum; ++i) {
		if (bi.at(i) != 0)
			tempbi += bi.at(i) * tempMulti;
		tempMulti *= 2;
	}
		
	rankbi = tempbi;			//这个数据集的总层数为21 最大2^21-1 = 2097151 验证后正确

	return true;
}
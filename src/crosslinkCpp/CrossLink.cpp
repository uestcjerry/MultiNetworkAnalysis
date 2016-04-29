#include "../../include/crossLink/CrossLink.h"

#include <iostream>
#include <fstream>



CrossLink::CrossLink() { capacity = 0; }
CrossLink::~CrossLink() { capacity = 0; }


bool CrossLink::testCrossLink()
{
	std::string sourceFile = BasicData::SrcEventWithTimePrefix + "test.txt";
	unsigned max = 0;

	if (findMaxNodeTagTime(sourceFile, max) == false)
		return false;
	std::cout << "max = " << max << std::endl;
	std::cout << "press any key to continue.." << std::endl;
	getchar();

	//crossLinkObj = std::vector<std::pair<CrossLink_t, CrossLink_t>>(max + 1);
	capacity = max;

	std::string sourceFileTime = BasicData::SrcEventWithTimePrefix + "test.txt";
	std::cout << "source file = " << sourceFileTime << std::endl;

	if (readFileConsCrosLink(sourceFileTime) == false) {
		std::cerr << "read file cons CrosLink error." << std::endl;
		return false;
	}
		
	showCroLink();

	std::cout << "press any key to continue.." << std::endl;
	getchar();
	
	if (searchHoriCroLink(BasicEdge(2, 3, 1, Time())) == false)
		std::cout << "not find." << std::endl;
	else
		std::cout << "find.." << std::endl;		//find
	if (searchHoriCroLink(BasicEdge(1234, 124, 1, Time())) == false)
		std::cout << "not find." << std::endl;		//not
	else
		std::cout << "find.." << std::endl;
	
	std::vector<unsigned> vec;
	if (getAjaFromHoriCroLink(BasicEdge(1, 2, 1, Time()), vec) == false)
		std::cerr << "get error." << std::endl;
	for (const auto &u : vec)
		std::cout << u << ", ";
	std::cout << std::endl;

	if (getAjaFromVertCroLink(BasicEdge(2, 3, 1, Time()), vec) == false)
		std::cerr << "get error." << std::endl;
	for (const auto &u : vec)
		std::cout << u << ", ";
	std::cout << std::endl;
	getchar();
	return true;
}

unsigned CrossLink::getCapacity() { return capacity; }

const unsigned CrossLink::getCapacity() const { return capacity; }

void CrossLink::deconsCrossLinkObj()
{
	crossLinkObj.clear();
	capacity = 0;
}
bool CrossLink::consCrossLinkObj(const std::string &file)
{
	unsigned max = 0;
	if (false == findMaxNodeTagTime(file, max)) {
		std::cerr << "cons crosslink : find max error." << std::endl;
		return false;
	}
	capacity = max;
	crossLinkObj = std::vector<std::pair<CrossLink_t, CrossLink_t>>(max + 1);			// takes some time..
	
	std::cout << "\n" << "cons crosslink obj finish: " << file << std::endl;
	return true;
}
bool CrossLink::readFileConsCrosLink(const std::string &file)
{
	std::cout << "\n read file cons cross-link: " << file << std::endl;			////
	
	if (consCrossLinkObj(file) == false)
		return false;

	std::fstream inputFile(file, std::ios_base::in);
	if (!inputFile.is_open()) {
		std::cerr << "file open error." << std::endl;
		return false;
	}
	
	unsigned idOne, idTwo, year, month, day, hour, minute, second;
	long long line = 0;

	while (inputFile >> idOne >> idTwo >> year >> month >> day >> hour >> minute >> second) {
		if (++line % 10000 == 0)
			std::cout << "read line = " << line << std::endl;
		
		Time tempTimeObj = Time(year, month, day, hour, minute, second);
		BasicEdge tempEdgeObj = BasicEdge(idOne, idTwo, 1, tempTimeObj);
		BasicEdge tempEdgeRevObj = BasicEdge(idTwo, idOne, 1, tempTimeObj);

		if (false == insertHoriCroLink(tempEdgeObj)) {
			std::cerr << "insert error." << std::endl;
			return false;
		}
		if (false == insertVertCroLink(tempEdgeObj)) {
			std::cerr << "insert error." << std::endl;
			return false;
		}
	}
	inputFile.close();
	std::cout << "\n read file cons cross-link finish.." << std::endl;			////
	return true;
}
bool CrossLink::findMaxNodeTag(const std::string &file, unsigned &max)
{
	max = 0;
	std::cout << "find max node begin.." << std::endl;

	std::fstream inputFile(file, std::ios_base::in);
	if (!inputFile.is_open()) {
		std::cerr << "open file error." << std::endl;
		return false;
	}
	unsigned tempOne, tempTwo;
	while (inputFile >> tempOne >> tempTwo) {
		max = tempOne > max ? tempOne : max;
		max = tempTwo > max ? tempTwo : max;
	}
	inputFile.close();
	std::cout << "find max node finish.." << std::endl;
	return true;
}
bool CrossLink::findMaxNodeTagTime(const std::string &file, unsigned &max)
{
	max = 0;
	//std::cout << "find max node with time begin.." << std::endl;

	std::fstream inputFile(file, std::ios_base::in);
	if (!inputFile.is_open()) {
		std::cerr << "open file error." << std::endl;
		return false;
	}
	unsigned tempOne, tempTwo, year, month, day, hour, minute, second;
	while (inputFile >> tempOne >> tempTwo >> year >> month >> day >> hour >> minute >> second) {
		max = tempOne > max ? tempOne : max;
		max = tempTwo > max ? tempTwo : max;
	}
	inputFile.close();

	//std::cout << "find max node with time finish.." << std::endl;
	return true;
}

bool CrossLink::insertHoriCroLink(const BasicEdge &elem)
{
	unsigned tempX = elem.getFrom();
	if (tempX > this->capacity || tempX < 0)  {
		std::cerr << "CrosLink:insertHori error." << std::endl;
		return false;
	}
	crossLinkObj.at(tempX).first.insert(elem);
	return true;
}

bool CrossLink::insertVertCroLink(const BasicEdge &elem)
{
	unsigned tempY = elem.getTo();
	if (tempY > this->capacity || tempY < 0) {
		std::cerr << "CrosLink:insertVert error." << std::endl;
		return false;
	}
	crossLinkObj.at(tempY).second.insert(elem);
	return true;
}

bool CrossLink::searchHoriCroLink(const BasicEdge &elem)
{
	unsigned tempX = elem.getFrom();
	if (tempX > capacity || tempX < 0) {
		std::cerr << "CrosLink:searchHori error." << std::endl;
		return false;
	}
	if (crossLinkObj.at(tempX).first.find(elem) == crossLinkObj.at(tempX).first.end())
		return false;
	else
		return true;
}
/*
bool CrossLink::searchVertCroLink(const BasicEdge &elem)
{
	unsigned tempY = elem.getTo();
	if (tempY > capacity || tempY < 0) {
		std::cerr << "CrosLink:searhVert error." << std::endl;
		return false;
	}
	if (crossLinkObj.at(tempY).second.find(elem) == crossLinkObj.at(tempY).second.end())
		return false;
	else
		return true;
}
*/

bool CrossLink::getAjaOfNodeFromHoriCroLink(const _NodeTag_t i, std::vector<_NodeTag_t> &vec)
{
	vec.clear();
	if (i > capacity || i < 0)
		return false;
	auto multiIte = crossLinkObj.at(i).first;
	if (multiIte.size() == 0)
		return true;
	for (auto iter = multiIte.begin(); iter != multiIte.end(); iter++)
		vec.push_back(iter->getTo());
	return true;
}
bool CrossLink::getAjaOfNodeFromVertCroLink(const _NodeTag_t j, std::vector<_NodeTag_t> &vec)
{
	vec.clear();
	if (j > capacity || j < 0)
		return false;
	auto multiIte = crossLinkObj.at(j).second;
	if (multiIte.size() == 0)
		return true;
	for (auto iter = multiIte.begin(); iter != multiIte.end(); iter++)
		vec.push_back(iter->getFrom());
	return true;
}

bool CrossLink::getAjaFromHoriCroLink(const BasicEdge &u, std::vector<_NodeTag_t> &vec)
{
	vec.clear();
	unsigned tempX = u.getFrom();
	if (tempX > capacity || tempX < 0)
		return false;
	auto multiIte = crossLinkObj.at(tempX).first;
	if (multiIte.size() == 0)
		return true;
	for (auto iter = multiIte.begin(); iter != multiIte.end(); iter++)
		vec.push_back(iter->getTo());
	return true;
}
bool CrossLink::getAjaFromVertCroLink(const BasicEdge &u, std::vector<_NodeTag_t> &vec)
{
	vec.clear();
	unsigned tempY = u.getTo();
	if (tempY > capacity || tempY < 0)
		return false;
	auto multiIte = crossLinkObj.at(tempY).second;
	if (multiIte.size() == 0)
		return true;
	for (auto iter = multiIte.begin(); iter != multiIte.end(); iter++)
		vec.push_back(iter->getFrom());
	return true;
}
bool CrossLink::getAjaSizeFromHori(const _NodeTag_t u, unsigned &res)
{
	if (u > capacity || u <= 0)
		return false;
	auto multiIte = crossLinkObj.at(u).first;
	
	res = multiIte.size();
	return true;
}
bool CrossLink::getAjaSizeFromVert(const _NodeTag_t u, unsigned &res)
{
	if (u > capacity || u <= 0)
		return false;
	auto multiIte = crossLinkObj.at(u).second;

	res = multiIte.size();
	return true;
}
bool CrossLink::getDegreeAsUndi(const _NodeTag_t u, unsigned &res)
{
	if (u > capacity || u <= 0) return false;
	return crossLinkObj.at(u).first.size() + crossLinkObj.at(u).second.size();
}

bool CrossLink::getAjaOfTimeFromHori(const _NodeTag_t i, std::vector<Time> &vec)
{
	vec.clear();
	if (i > capacity || i < 0)
		return false;
	auto multiIte = crossLinkObj.at(i).first;
	if (multiIte.size() == 0)
		return true;
	for (auto iter = multiIte.begin(); iter != multiIte.end(); iter++)
		vec.push_back(iter->getTime());
	return true;
}
bool CrossLink::getAjaOfTimeFromVert(const _NodeTag_t j, std::vector<Time> &vec)
{
	vec.clear();
	if (j > capacity || j < 0)
		return false;
	auto multiIte = crossLinkObj.at(j).second;
	if (multiIte.size() == 0)
		return true;
	for (auto iter = multiIte.begin(); iter != multiIte.end(); iter++)
		vec.push_back(iter->getTime());
	return true;
}
/*
 *	k-core-decomposition 删除边
 *	这里的边一定是<from, to>，调用的时候区分有向和无向即可
 */
bool CrossLink::deleteUndiEdge(const _NodeTag_t from, const _NodeTag_t to)
{
	if (from <= 0 || from > capacity || to <= 0 || to > capacity) return false;

	BasicEdge delObj = BasicEdge(from, to, 1, Time());

	//这里十字链表的两个方向在构造的时候都插入了，所以都要删除
	//分别是邻接表、逆邻接表

	if (crossLinkObj.at(from).first.erase(delObj) != 1) {
		std::cerr << "delete undirected edge error. at hori: " << from << std::endl;
		return false;
	}
	if (crossLinkObj.at(to).second.erase(delObj) != 1) {
		std::cerr << "delete undirected edge error. at vert: " << to << std::endl;
		return false;
	}
	return true;
}

void CrossLink::showCroLink()
{
	std::cout << "============================== Aja table ===============================" << std::endl;
	for (unsigned i = 1; i <= capacity; ++i) {
		auto tempPair = crossLinkObj.at(i);
		if (tempPair.first.size() == 0)
			continue;

		std::cout << "start node = " << i << " : ";
		long long line = 0;
		for (const auto &elem : tempPair.first) {
			if (++line % 4 == 0)
				std::cout << std::endl;
			std::cout << elem << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << "press any key.." << std::endl;
	getchar();
	std::cout << "============================= Rev Aja table =============================" << std::endl;
	for (unsigned j = 1; j <= capacity; ++j) {
		auto tempPair = crossLinkObj.at(j);
		if (tempPair.second.size() == 0)
			continue;

		std::cout << "start node = " << j << " : ";
		long long line = 0;
		for (const auto &elem : tempPair.second) {
			if (++line % 4 == 0)
				std::cout << std::endl;
			std::cout << elem << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << "finish.." << std::endl;
}
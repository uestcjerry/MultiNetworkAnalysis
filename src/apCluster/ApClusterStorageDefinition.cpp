	#include "../../include/apCluster/ApClusterStorageDefinition.h"
#include <iostream>

/*
 *	Edge
 */
void ApEdgeStorage::initApEdgeStorage(const TAG_T length)
{
	boolVecLen_ = length;

	boolVec_ = BOOLVEC_T(length + 1, false);
	edgesVec_ = EDGESVEC_T(length + 1);
}

JUG_T ApEdgeStorage::searchFirstNodeInBoolVec(const TAG_T id)
{
	if (id > boolVecLen_) {
		std::cerr << "searchFirstNodeInBoolVec(): " << id << " is out of range." << std::endl;
		return false;
	}
	if (boolVec_[id]) 
		return true;
	else 
		return false;
}

JUG_T ApEdgeStorage::searchEdgeInEdgesVec(const TAG_T i, const TAG_T j, ApElement &k)
{
	if (!searchFirstNodeInBoolVec(i)) {
		//std::cerr << "searchEdgeInEdgesVec() did not search first node: " << i << std::endl;
		return false;
	}
	EDGES_HASH_MAP_T::const_iterator it = edgesVec_[i].find(j);
	if (it == edgesVec_[i].end())  
		return false;
	k = it->second;
	return true;
}

JUG_T ApEdgeStorage::insertEdgeToEdgesVec(const TAG_T i, const TAG_T j, const ApElement &k)
{
	if (i > boolVecLen_ || j > boolVecLen_) {
		std::cerr << "insertEdgeToEdgesVec() error: " << i << " or " << j << "." << std::endl;
		return false;
	}
	boolVec_[i] = true;
	std::pair<EDGES_HASH_MAP_T::iterator, bool> result = edgesVec_.at(i).insert(std::make_pair(j, k));
	return result.second;
}

/*
 *	更新元素
 */
JUG_T ApEdgeStorage::modifyEdgeElemS(TAG_T i, TAG_T j, ELEM_T v)
{
	if (i > boolVecLen_ || j > boolVecLen_) {
		std::cerr << "modifyEdgeElemS() out of range: " << i << ", " << j << std::endl;
		return false;
	}
	if (!boolVec_.at(i)) {
		std::cerr << "modifyEdgeElemS() first node did`t exist: " << i << std::endl;
		return false;
	}
	EDGES_HASH_MAP_T::iterator it = edgesVec_.at(i).find(j);
	if (it == edgesVec_.at(i).end())
		return false;

	it->second.setS(v);
	return true;
}
JUG_T ApEdgeStorage::modifyEdgeElemR(TAG_T i, TAG_T j, ELEM_T v)
{
	if (i > boolVecLen_ || j > boolVecLen_) {
		std::cerr << "modifyEdgeElemR() out of range: " << i << ", " << j << std::endl;
		return false;
	}
	if (!boolVec_.at(i)) {
		std::cerr << "modifyEdgeElemR() first node did`t exist: " << i << std::endl;
		return false;
	}
	EDGES_HASH_MAP_T::iterator it = edgesVec_.at(i).find(j);
	if (it == edgesVec_.at(i).end())
		return false;

	it->second.setR(v);
	return true;
}
JUG_T ApEdgeStorage::modifyEdgeElemA(TAG_T i, TAG_T j, ELEM_T v)
{
	if (i > boolVecLen_ || j > boolVecLen_) {
		std::cerr << "modifyEdgeElemA() out of range: " << i << ", " << j << std::endl;
		return false;
	}
	if (!boolVec_.at(i)) {
		std::cerr << "modifyEdgeElemA() first node did`t exist: " << i << std::endl;
		return false;
	}
	EDGES_HASH_MAP_T::iterator it = edgesVec_.at(i).find(j);
	if (it == edgesVec_.at(i).end())
		return false;

	it->second.setA(v);
	return true;
}

/*
 *	获取元素
 */
JUG_T ApEdgeStorage::getEdgeElemS(TAG_T i, TAG_T j, ELEM_T &r)
{
	if (i > boolVecLen_ || j > boolVecLen_) {
		std::cerr << "getEdgeElemS() out of range: " << i << ", " << j << std::endl;
		return false;
	}
	if (!boolVec_.at(i)) {
		std::cerr << "getEdgeElemS() first node did`t exist: " << i << std::endl;
		return false;
	}
	EDGES_HASH_MAP_T::iterator it = edgesVec_.at(i).find(j);
	if (it == edgesVec_.at(i).end())
		return false;
	r = it->second.getS();
	return true;
}
JUG_T ApEdgeStorage::getEdgeElemR(TAG_T i, TAG_T j, ELEM_T &r)
{
	if (i > boolVecLen_ || j > boolVecLen_) {
		std::cerr << "getEdgeElemR() out of range: " << i << ", " << j << std::endl;
		return false;
	}
	if (!boolVec_.at(i)) {
		std::cerr << "getEdgeElemR() first node did`t exist: " << i << std::endl;
		return false;
	}
	EDGES_HASH_MAP_T::iterator it = edgesVec_.at(i).find(j);
	if (it == edgesVec_.at(i).end())
		return false;
	r = it->second.getR();
	return true;
}
JUG_T ApEdgeStorage::getEdgeElemA(TAG_T i, TAG_T j, ELEM_T &r)
{
	if (i > boolVecLen_ || j > boolVecLen_) {
		std::cerr << "getEdgeElemA() out of range: " << i << ", " << j << std::endl;
		return false;
	}
	if (!boolVec_.at(i)) {
		std::cerr << "getEdgeElemA() first node did`t exist: " << i << std::endl;
		return false;
	}
	EDGES_HASH_MAP_T::iterator it = edgesVec_.at(i).find(j);
	if (it == edgesVec_.at(i).end())
		return false;
	r = it->second.getA();
	return true;
}

/*
*	测试
*/
void ApEdgeStorage::showApEdgeStorage()
{
	for (unsigned i = 1; i <= boolVecLen_; ++i) {
		if (!boolVec_.at(i))
			continue;

		std::cout << "first node is: " << i << std::endl;

		unsigned n = 0;
		EDGES_HASH_MAP_T::const_iterator it_begin = edgesVec_.at(i).begin();
		while (it_begin != edgesVec_.at(i).end()) {
			std::cout << "j = " << it_begin->first << "\t" << it_begin->second << std::endl;
			it_begin++;
			
			//if (n++ % 2 == 0)
			//	std::cout << std::endl;
			//else
			//	std::cout << " ";
		}

		getchar();
	}
}
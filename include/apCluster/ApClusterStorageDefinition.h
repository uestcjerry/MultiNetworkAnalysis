#ifndef APCLUSTERSTORAGEDEFINITION_H
#define APCLUSTERSTORAGEDEFINITION_H

#include "ApClusterBasicDefinition.h"

#include <bitset>
#include <vector>
#include <map>
#include <unordered_map>

#define TAG_T unsigned
#define JUG_T bool
#define ELEM_T double

/*
 *  storage information of {s, r, a} for edges and nodes in AP-clustering procedure 
 */

/*
 *	Edge  <x, y>
 */
class ApEdgeStorage {
public:
	typedef std::vector<JUG_T>							BOOLVEC_T;
	typedef std::unordered_map<TAG_T, ApElement>		EDGES_HASH_MAP_T;			// map -> unordered_map
	typedef std::vector<EDGES_HASH_MAP_T>				EDGESVEC_T;

	ApEdgeStorage() = default;
	ApEdgeStorage(const ApEdgeStorage &) = delete;
	ApEdgeStorage &operator=(const ApEdgeStorage &) = delete;
	~ApEdgeStorage() = default;

public:
	void	initApEdgeStorage(const TAG_T);		//最大节点编号max，下标有效范围 [1, max]
	JUG_T	searchFirstNodeInBoolVec(const TAG_T);
	
	JUG_T	searchEdgeInEdgesVec(const TAG_T, const TAG_T, ApElement &);
	JUG_T	insertEdgeToEdgesVec(const TAG_T, const TAG_T, const ApElement &);

	// 更新元素
	JUG_T	modifyEdgeElemS(TAG_T, TAG_T, ELEM_T);
	JUG_T	modifyEdgeElemR(TAG_T, TAG_T, ELEM_T);
	JUG_T	modifyEdgeElemA(TAG_T, TAG_T, ELEM_T);

	// 获取元素
	JUG_T	getEdgeElemS(TAG_T, TAG_T, ELEM_T &);
	JUG_T	getEdgeElemR(TAG_T, TAG_T, ELEM_T &);
	JUG_T	getEdgeElemA(TAG_T, TAG_T, ELEM_T &);

public:
	void	showApEdgeStorage();

private:
	TAG_T boolVecLen_;
	BOOLVEC_T boolVec_;

	EDGESVEC_T edgesVec_;
};


/*
*	int, char, long 可以直接用作key，自定义类型需要提供比价函数或者==
*	hash_map 需要的是 == 函数，map 需要 < 函数
*	hash_map 采用hash表存储，而 map 采用红黑树。

	为保证效率，最好用构造函数指定大小。
	其余常用的函数：
	hash_map(size_type n);
	const_iterator find(const key_type & k) const;
	data_type &operator[](const key_type &k);
	insert();
	erase();
*/

/*
struct compare_str {
	size_t operator()(const char *p1, const char *p2) const {
		return strcmp(p1, p2) == 0;
	}
};
typedef std::hash_map<const char *, std::string, std::hash<const char *>, compare_str> StrIntMap;
*/


#endif
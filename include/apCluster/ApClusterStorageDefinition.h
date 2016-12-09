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
	void	initApEdgeStorage(const TAG_T);		//���ڵ���max���±���Ч��Χ [1, max]
	JUG_T	searchFirstNodeInBoolVec(const TAG_T);
	
	JUG_T	searchEdgeInEdgesVec(const TAG_T, const TAG_T, ApElement &);
	JUG_T	insertEdgeToEdgesVec(const TAG_T, const TAG_T, const ApElement &);

	// ����Ԫ��
	JUG_T	modifyEdgeElemS(TAG_T, TAG_T, ELEM_T);
	JUG_T	modifyEdgeElemR(TAG_T, TAG_T, ELEM_T);
	JUG_T	modifyEdgeElemA(TAG_T, TAG_T, ELEM_T);

	// ��ȡԪ��
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
*	int, char, long ����ֱ������key���Զ���������Ҫ�ṩ�ȼۺ�������==
*	hash_map ��Ҫ���� == ������map ��Ҫ < ����
*	hash_map ����hash��洢���� map ���ú������

	Ϊ��֤Ч�ʣ�����ù��캯��ָ����С��
	���ೣ�õĺ�����
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
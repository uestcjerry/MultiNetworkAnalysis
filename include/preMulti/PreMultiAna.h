#ifndef PREMULTIANA_H
#define PREMULTIANA_H

#include "../../include/crossLink/CrossLink.h"

#include <string>
#include <vector>


/* 
 *	��������Pre�Ķ���������
 *	��Ҫ�����ж������
 */

class PreMultiAnalysis {
public:
	PreMultiAnalysis();
	~PreMultiAnalysis();

	unsigned getMultiNetNum();

	bool buildMultiNetwork(const std::string &, const std::vector<std::string> &);		//fileprefix multinetworks
	bool clearMultiNetwork();

public:


private:
	PreMultiAnalysis &operator=(const PreMultiAnalysis &) = delete;
	PreMultiAnalysis(const PreMultiAnalysis &) = delete;

private:
	//�����������
	std::vector<CrossLink> multiNetObj;
	unsigned multiNetNum;
};


bool preMultiAnalysisRevokeThis();			//after all, revoke this function


#endif
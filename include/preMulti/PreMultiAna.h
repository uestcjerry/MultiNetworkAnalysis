#ifndef PREMULTIANA_H
#define PREMULTIANA_H

#include "../../include/crossLink/CrossLink.h"

#include <string>
#include <vector>


/* 
 *	接下来做Pre的多层网络分析
 *	需要存所有多层网络
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
	//多层网络数据
	std::vector<CrossLink> multiNetObj;
	unsigned multiNetNum;
};


bool preMultiAnalysisRevokeThis();			//after all, revoke this function


#endif
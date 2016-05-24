#include "../../include/eventAnalysis/EventAnalysis.h"
#include "../../include/basicDef/BasicData.h"

#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include <cmath>
#include <ctgmath>
#include <vector>
#include <algorithm>
#include <memory>
#include <cstring>
#include <queue>

EventAnalysis::EventAnalysis() {}

EventAnalysis::~EventAnalysis() {}

bool EventAnalysis::initUserManageObj(const std::string &srcFilePrefix)
{
	std::cout << "initUserManage() begin.." << std::endl;

	if (userManageObj.consUserVecObj() == false) {
		std::cerr << "user manage obj cons error." << std::endl;
		return false;
	}
	for (const auto &file : BasicData::VecSrcUserFiles) {
		std::cout << "read User File : " << srcFilePrefix + file << std::endl;

		if (userManageObj.readFileConsUserManage(srcFilePrefix + file) == false) {
			std::cerr << "read file cons user manage obj error." << std::endl;
			return false;
		}
	}
	
	std::cout << "initUserManage() finish.." << std::endl;
	std::cout << "Press any key to continue.." << std::endl;
	getchar();
	return true;
}
bool EventAnalysis::buildNetworkOne(const std::string &file)
{
	if (networkOne.readFileConsCrosLink(file) == false)
		return false;
	return true;
}
bool EventAnalysis::buildNetworkOneWithTime(const std::string &file, const Time &minTime, const long long disOfDay)
{
	if (networkOne.readFileConsCrosLinkBeforTime(file, minTime, disOfDay) == false)
		return false;
	return true;
}
bool EventAnalysis::buildNetworkTwo(const std::string &file)
{
	if (networkTwo.readFileConsCrosLink(file) == false)
		return false;
	return true;
}
void EventAnalysis::clearNetworkOne()
{
	networkOne.deconsCrossLinkObj();
}
void EventAnalysis::clearNetworkTwo()
{
	networkTwo.deconsCrossLinkObj();
}

// revoke this function
bool eventAnalysisRevokeThis()
{
	EventAnalysis obj;

	// init once , enough
	//if (obj.initUserManageObj(BasicData::SrcUserRehashedDelRepPrefix) == false)
	//	return false;

	/*
	if (obj.anaDegreeRelaBetLevl() == false)
		return false;

	if (obj.anaOverlapOfDifLevl() == false)
		return false;

	if (obj.anaConnectedComponent() == false)
		return false;

	if (obj.anaTransProportion() == false)
		return false;
	*/

	//if (obj.kCoreDecomposition() == false)
	//	return false;

	if (obj.kcoreAnaBetTime() == false)
		return false;


	std::cout << "event analysis finish.." << std::endl;
	std::cout << "press any key to continue.." << std::endl;
	getchar();
	return true;
}


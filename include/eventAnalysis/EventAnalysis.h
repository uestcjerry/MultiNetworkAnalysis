#ifndef EVENTANALYSIS_H
#define EVENTANALYSIS_H

#include "../userManage/User.h"
#include "../userManage/UserManage.h"
#include "../crossLink/CrossLink.h"
#include <memory>


class EventAnalysis {
public:
	EventAnalysis();
	~EventAnalysis();

	bool initUserManageObj(const std::string &);

	bool buildNetworkOne(const std::string &);		//参数是文件全名
	bool buildNetworkTwo(const std::string &);
	bool buildNetworkOneWithTime(const std::string &, const Time &, const long long);

	void clearNetworkOne();
	void clearNetworkTwo();
public:
	// Pearson Sample co-efficiency
	bool anaDegreeRelaBetLevl();

	bool getCommonNode(std::vector<unsigned> &);
	bool getDegreeOfDifLevlFromCommonVec(std::vector<unsigned> &, std::set<unsigned> &,
		std::vector<std::pair<unsigned, unsigned>> &, std::vector<std::pair<unsigned, unsigned>> &);
	unsigned existTimeInSet(std::vector<unsigned> &, std::set<unsigned> &);
	double calAverageFromMap(std::vector<std::pair<unsigned, unsigned>> &);

	// Overlapping of different level (directed edge)
	bool anaOverlapOfDifLevl();

	bool calCommonNumber(std::vector<unsigned> &, std::set<unsigned> &, long long &);
	bool calUnionNumber(CrossLink &, std::set<unsigned> &, long long &);

	// connected component  连通分量
	bool anaConnectedComponent();

	bool procBfsArray(std::unique_ptr<BfsNode[]> &);
	bool setBfsBranch(std::unique_ptr<BfsNode[]> &, const unsigned, const unsigned);
	bool getConnectedComponent(std::unique_ptr<BfsNode[]> &, std::vector<std::pair<unsigned, unsigned>> &);

	// transmit proportion	随时间的转发率
	bool anaTransProportion();

	bool getMinMaxTimeOfEvent(CrossLink &, Time &, Time &);
	bool countingBucketAccordTime(CrossLink &, std::unique_ptr<long long[]> &, Time &, Time &);
	int countingBucketArray(std::unique_ptr<long long[]> &, const Time &, const Time &, const Time &, const long long);
	bool calTransPropResult(std::unique_ptr<long long[]> &, std::vector<std::pair<double, double>> &);

	// k-core decomposition k-壳中心性
	bool kCoreDecomposition();
	bool kCoreUserDecomposition();

	bool decompEventFile(CrossLink &, std::vector<std::pair<unsigned, unsigned>> &, const unsigned);
	unsigned findMinDegreeNow(const CrossLink &, const std::vector<std::pair<bool, unsigned>> &);
	bool delEdgesFromVec(CrossLink &, bool, const unsigned , std::vector<unsigned> &);
	bool kCoreDecomWriteFile(const std::string &, const unsigned ,std::vector<std::pair<unsigned, unsigned>> &);

	// 按照时间段分析 kcore随转发时间的变化
	bool kcoreAnaBetTime();

	bool findMinAndMaxTimeOfEvent(const std::string &, Time &, Time &);
	bool kcoreAnaWriteFile(const std::string &, const std::vector<std::pair<unsigned, unsigned>> &, const unsigned, const double);

	// 度中心性排序
	bool degreeCentrality();
	
	bool calculateEachNodeDegree(std::vector<unsigned> &);
	bool calculateEventNodeNumber(const std::vector<unsigned> &, unsigned &);			//获取时间用户个数
	bool writeFileDegreeCentra(const std::string &, const unsigned, const std::vector<unsigned> &, const unsigned);	//度中心性
	bool writeFileDegreeDis(const std::string &, const unsigned, const std::vector<unsigned> &);

	// 接近中心性
	bool closenessCentrality();

	bool calEachEventNodeNum(unsigned &);
	bool calEachEventClossCentra(const unsigned, std::vector<std::pair<unsigned, double>> &);
	bool calNodeSumDis(const unsigned, unsigned &);
	bool writeFileClosCentra(const std::string &, const unsigned, const std::vector<std::pair<unsigned, double>> &);


private:
	EventAnalysis(const EventAnalysis &) = delete;
	EventAnalysis& operator=(const EventAnalysis &) = delete;
private:
	//网络数据分析
	UserManage userManageObj;				//init once
	CrossLink networkOne;					//init whether needed
	CrossLink networkTwo;
};

bool eventAnalysisRevokeThis();				//revoke function


#endif
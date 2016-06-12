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

	bool buildNetworkOne(const std::string &);		//�������ļ�ȫ��
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

	// connected component  ��ͨ����
	bool anaConnectedComponent();

	bool procBfsArray(std::unique_ptr<BfsNode[]> &);
	bool setBfsBranch(std::unique_ptr<BfsNode[]> &, const unsigned, const unsigned);
	bool getConnectedComponent(std::unique_ptr<BfsNode[]> &, std::vector<std::pair<unsigned, unsigned>> &);

	// transmit proportion	��ʱ���ת����
	bool anaTransProportion();

	bool getMinMaxTimeOfEvent(CrossLink &, Time &, Time &);
	bool countingBucketAccordTime(CrossLink &, std::unique_ptr<long long[]> &, Time &, Time &);
	int countingBucketArray(std::unique_ptr<long long[]> &, const Time &, const Time &, const Time &, const long long);
	bool calTransPropResult(std::unique_ptr<long long[]> &, std::vector<std::pair<double, double>> &);

	// k-core decomposition k-��������
	bool kCoreDecomposition();
	bool kCoreUserDecomposition();

	bool decompEventFile(CrossLink &, std::vector<std::pair<unsigned, unsigned>> &, const unsigned);
	unsigned findMinDegreeNow(const CrossLink &, const std::vector<std::pair<bool, unsigned>> &);
	bool delEdgesFromVec(CrossLink &, bool, const unsigned , std::vector<unsigned> &);
	bool kCoreDecomWriteFile(const std::string &, const unsigned ,std::vector<std::pair<unsigned, unsigned>> &);

	// ����ʱ��η��� kcore��ת��ʱ��ı仯
	bool kcoreAnaBetTime();

	bool findMinAndMaxTimeOfEvent(const std::string &, Time &, Time &);
	bool kcoreAnaWriteFile(const std::string &, const std::vector<std::pair<unsigned, unsigned>> &, const unsigned, const double);

	// ������������
	bool degreeCentrality();
	
	bool calculateEachNodeDegree(std::vector<unsigned> &);
	bool calculateEventNodeNumber(const std::vector<unsigned> &, unsigned &);			//��ȡʱ���û�����
	bool writeFileDegreeCentra(const std::string &, const unsigned, const std::vector<unsigned> &, const unsigned);	//��������
	bool writeFileDegreeDis(const std::string &, const unsigned, const std::vector<unsigned> &);

	// �ӽ�������
	bool closenessCentrality();

	bool calEachEventNodeNum(unsigned &);
	bool calEachEventClossCentra(const unsigned, std::vector<std::pair<unsigned, double>> &);
	bool calNodeSumDis(const unsigned, unsigned &);
	bool writeFileClosCentra(const std::string &, const unsigned, const std::vector<std::pair<unsigned, double>> &);


private:
	EventAnalysis(const EventAnalysis &) = delete;
	EventAnalysis& operator=(const EventAnalysis &) = delete;
private:
	//�������ݷ���
	UserManage userManageObj;				//init once
	CrossLink networkOne;					//init whether needed
	CrossLink networkTwo;
};

bool eventAnalysisRevokeThis();				//revoke function


#endif
#include "../../include/basicDef/BasicData.h"
#include "../../include/eventAnalysis/EventAnalysis.h"

#include <iostream>
#include <sstream>
#include <fstream>


/* ================================= transmit proportion ========================================*/

/*
*	转发事件比例
*	考虑转发时间，time_min -> time_max 映射为[0 ~ 1]
*	桶计数，按照时间划分为 0.1 ~ 1.0 十个区间，桶计数
*	转发比 = 当前时刻转发数量 / 总转发数量
*	output format: (for each event, ten lines for output)
*	% file : filename
*	0.1 transmit_Proportion
*	0.2 ...
*	...
*	1.0 1.0
*/
bool EventAnalysis::anaTransProportion()
{
	std::cout << "analysis transmit proportion begin.." << std::endl;

	std::string srcFilePrefix = BasicData::SrcEventWithTimePrefix;

	for (unsigned i = 0; i < BasicData::VecSrcEventFiles.size(); ++i) {
		std::cout << "\n first for: " << BasicData::VecSrcEventFiles.at(i) << std::endl;	/////////////

		if (buildNetworkOne(srcFilePrefix + BasicData::VecSrcEventFiles.at(i)) == false) {
			std::cerr << "build network one error." << std::endl, getchar();
			clearNetworkOne();
			return false;
		}
		// to do
		std::unique_ptr<long long[]> bucketArray(new long long[10]);			// [0 ~ 9] counting number
		for (int k = 0; k < 10; k++)
			bucketArray[k] = 0;

		Time minTimeObj = Time(2015, 0, 0, 0, 0, 0), maxTimeObj = Time(2000, 0, 0, 0, 0, 0);
		if (getMinMaxTimeOfEvent(networkOne, minTimeObj, maxTimeObj) == false) {
			std::cerr << "get min max time error." << std::endl;
			return false;
		}
		std::cout << "get min max time finish.." << std::endl;				///////////////////

		if (countingBucketAccordTime(networkOne, bucketArray, minTimeObj, maxTimeObj) == false) {
			std::cerr << "counting bucket accord time error." << std::endl;
			return false;
		}
		std::cout << "counting bucket accord time finish.." << std::endl;		////////////////////

		std::vector<std::pair<double, double>> transProResult;
		if (calTransPropResult(bucketArray, transProResult) == false) {
			std::cerr << "cal transmit proportion result error." << std::endl;
			return false;
		}
		std::cout << "cal trans prop result finish.." << std::endl;			///////////////////

		std::cout << "write file begin.." << std::endl;					//////////////////////////
		std::stringstream ss;
		ss << i;		std::string fileId = ss.str();
		std::fstream outpufFile(BasicData::TargetAnaResPrefix + "TransmitProportion_" + fileId, std::ios_base::out);
		if (!outpufFile.is_open()) {
			std::cerr << "open output file error." << std::endl;
			return false;
		}

		outpufFile << "% file :" << BasicData::VecSrcEventFiles.at(i) << "\n";
		for (const auto &elem : transProResult)
			outpufFile << elem.first << "\t" << elem.second << "\n";

		outpufFile.close();
		std::cout << "write file finish.." << std::endl;

		std::cout << "\n clearNetworkOne() begin.." << std::endl;
		clearNetworkOne();
		std::cout << "\n clearNetworkOne() finish.." << std::endl;
	}

	std::cout << "analysis transmit proportion finish.." << std::endl;
	return true;
}
bool EventAnalysis::getMinMaxTimeOfEvent(CrossLink &network, Time &minTime, Time &maxTime)
{
	unsigned capa = network.getCapacity();
	for (unsigned i = 1; i <= capa; ++i) {
		std::vector<Time> ajaVec;
		if (network.getAjaOfTimeFromHori(i, ajaVec) == false)
			return false;
		for (const auto &elem : ajaVec) {
			if (elem < minTime)
				minTime = elem;
			if (elem > maxTime)
				maxTime = elem;
		}
		std::vector<Time> reajaVec;
		if (network.getAjaOfTimeFromVert(i, reajaVec) == false)
			return false;
		for (const auto &elem : reajaVec) {
			if (elem < minTime)
				minTime = elem;
			if (elem > maxTime)
				maxTime = elem;
		}
	}
	std::cout << "min time=" << minTime << std::endl;
	std::cout << "max time=" << maxTime << std::endl;
	//getchar();
	return true;
}

bool EventAnalysis::countingBucketAccordTime(CrossLink &network, std::unique_ptr<long long[]> &bucketArray,
	Time &minTime, Time &maxTime)
{
	long long distanceDay = distanceDayBetTime(minTime, maxTime);

	unsigned capa = networkOne.getCapacity();


	long long totaledge = 0, totalAdd = 0;

	for (unsigned i = 1; i <= capa; ++i) {
		std::vector<Time> ajaVec;
		if (network.getAjaOfTimeFromHori(i, ajaVec) == false)
			return false;
		// to do..
		totaledge += ajaVec.size();

		int tag = 0;
		for (const auto &elem : ajaVec)
			if ((tag = countingBucketArray(bucketArray, elem, minTime, maxTime, distanceDay)) == -1) {
				std::cerr << "counting bucket array error." << std::endl;
				return false;
			}
			else {
				bucketArray[tag]++; totalAdd++;
			}
			std::vector<Time> reajaVec;
			if (network.getAjaOfTimeFromVert(i, reajaVec) == false)
				return false;

			totaledge += reajaVec.size();

			for (const auto &elem : reajaVec)
				if ((tag = countingBucketArray(bucketArray, elem, minTime, maxTime, distanceDay)) == -1){
					std::cerr << "counting bucket array error." << std::endl;
					return false;
				}
				else {
					bucketArray[tag]++; totalAdd++;
				}
	}

	return true;
}
/*
* bucket array[10] : 0 ~ 9
*/
int EventAnalysis::countingBucketArray(std::unique_ptr<long long[]> &bucketArray, const Time &nowTime,
	const Time &minTime, const Time &maxTime, const long long distanceDay)
{
	if (nowTime == minTime) {
		//bucketArray[0]++;
		return 0;
	}
	if (nowTime == maxTime) {
		//bucketArray[9]++; 
		return 9;
	}
	long long distance = distanceDayBetTime(minTime, nowTime);

	int tag = static_cast<int>(((double)distance / (double)distanceDay) * 100 / 10);

	if (tag < 0 || tag > 9) {
		std::cerr << "countingBucketArray: tag out of range.." << std::endl;

		std::cout << "disday = " << distanceDay << std::endl;
		std::cout << "distance= " << distance << std::endl;
		std::cout << "tag = " << tag << std::endl;	///////////

		std::cout << "min time=" << minTime << std::endl;
		std::cout << "now time=" << nowTime << std::endl;
		getchar();
		return -1;
	}

	return tag;
}
bool EventAnalysis::calTransPropResult(std::unique_ptr<long long[]> &bucketArray, std::vector<std::pair<double, double>> &res)
{
	std::cout << "cal trans prop result begin.." << std::endl;

	unsigned total = 0, now = 0;
	double tag = 0.1;

	for (unsigned j = 0; j < 10; ++j)
		total += bucketArray[j], std::cout << "bucket=" << bucketArray[j] << std::endl;
	//getchar();

	for (unsigned i = 0; i < 10; ++i) {
		now += bucketArray[i];
		double percentage = (double)now / (double)total;

		std::cout << "tag=" << tag << "\t per=" << percentage << std::endl;

		res.push_back(std::make_pair(tag, percentage));
		tag += 0.1;
	}

	//std::cout << "press any key.." << std::endl;
	//getchar();
	return true;
}
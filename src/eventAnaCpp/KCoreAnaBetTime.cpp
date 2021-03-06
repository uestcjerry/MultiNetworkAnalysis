#include "../../include/eventAnalysis/EventAnalysis.h"

#include <iostream>
#include <cmath>
#include <ctgmath>
#include <cstring>
#include <string>
#include <fstream>

/*
 * 按照时间段分析 kcore随转发时间的变化
 */
bool EventAnalysis::kcoreAnaBetTime()
{
	std::cout << "k core analysis each event bet time begin.." << std::endl;

	std::string srcFilePrefix = BasicData::SrcEventWithTimePrefix;

	for (unsigned i = 0; i < BasicData::VecSrcEventFiles.size(); ++i) {
		std::cout << "\n handling : " << BasicData::VecSrcEventFiles.at(i) << std::endl;	

		const std::string srcFileName = srcFilePrefix + BasicData::VecSrcEventFiles.at(i);

		Time minTime, maxTime;
		long long totalDisOfDay = 0;		//划分distance区间，看区间段内的时间kcore结果

		if (findMinAndMaxTimeOfEvent(srcFileName, minTime, maxTime) == false) {
			std::cerr << "find min and max time of event error." << std::endl;
			return false;
		}

		std::cout << minTime << std::endl;
		std::cout << maxTime << std::endl;
		totalDisOfDay = distanceDayBetTime(maxTime, minTime);
		std::cout << "event: " << i << ", totalDisOfDay = " << totalDisOfDay << std::endl;

		// 0.2 ~ 1.0, 0.2 += 0.2 划分五段
		// 做对比，划分二十段 0.05 ~ 1.00			write file 修改一下即可
		for (double div = 0.05; div < 1.05; div += 0.05) {

			std::cout << "current div = " << div << ", eventId = " << i << std::endl;		///////////////
			
			long long divDisOfDay = static_cast<long long>(totalDisOfDay * div);

			if (buildNetworkOneWithTime(srcFileName, minTime, divDisOfDay) == false) {
				std::cerr << "build network one with time error, at div = " << div << std::endl;
				clearNetworkOne();
				return false;
			}

			// decomposition
			// result format: pair<ks, nodeid> 保存，first是ks值，second为节点号
			std::vector<std::pair<unsigned, unsigned>> resultVec;
			 
			if (decompEventFile(networkOne, resultVec, i) == false) {
				std::cerr << "de-composite event file error : " << BasicData::VecSrcEventFiles.at(i) << std::endl;
				return false;
			}

			//写入文件之中
			if (kcoreAnaWriteFile(BasicData::TargetAnaResPrefix, resultVec, i, div) == false) {
				std::cerr << "k-core decomposition write file error." << std::endl;
				return false;
			}
			clearNetworkOne();
		}

	}

	std::cout << "k core decomposition finish.." << std::endl;
	std::cout << "Press any key to continue.." << std::endl;
	getchar();
	
	return true;
}
bool EventAnalysis::findMinAndMaxTimeOfEvent(const std::string &file, Time &minTime, Time &maxTime)
{
	std::cout << "find min and max time of event begin.." << std::endl;

	std::fstream inputFile(file, std::ios_base::in);
	if (!inputFile.is_open()) {
		std::cerr << "open input file error." << std::endl;
		return false;
	}

	Time tempMinTime, tempMaxTime;
	
	unsigned idOne, idTwo, year, month, day, hour, minute, second;
	
	bool firstTime = true;

	while (inputFile >> idOne >> idTwo >> year >> month >> day >> hour >> minute >> second) {
		if (firstTime) {
			tempMinTime = Time(year, month, day, hour, minute, second);
			tempMaxTime = Time(year, month, day, hour, minute, second);
			firstTime = false;
		}
		
		Time tempTimeObj = Time(year, month, day, hour, minute, second);
		if (tempTimeObj < tempMinTime)
			tempMinTime = tempTimeObj;
		if (tempTimeObj > tempMaxTime)
			tempMaxTime = tempTimeObj;
	}
	inputFile.close();

	minTime = tempMinTime; maxTime = tempMaxTime;

	return true;
}
bool EventAnalysis::kcoreAnaWriteFile(const std::string &tarFilePref, const std::vector<std::pair<unsigned, unsigned>> &resultVec,
								const unsigned eventId, const double div)
{
	std::stringstream ss;
	ss << "event";
	ss << eventId;
	ss << "_";

	//ss << div * 10;		// 划分为五段 0.2， 这里 *10
	ss << div * 100;		// 划分为二十段  0.05，这里 * 100

	std::string fileName;
	ss >> fileName;

	const std::string outputFileName = tarFilePref + fileName;

	std::fstream outpufFile(outputFileName, std::ios_base::out);
	if (!outpufFile.is_open()) {
		std::cerr << "open output file error." << std::endl;
		return false;
	}
	
	//outpufFile << "% k-core decomposition <ks, nodeId> event:" << eventId << ", div = " << div << std::endl;

	for (const auto &elem : resultVec)
		outpufFile << elem.first << "\t" << elem.second << "\n";

	outpufFile.close();

	return true;
}
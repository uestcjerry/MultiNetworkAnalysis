#include "../../include/kendall/Kendall.h"

#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <ctgmath>

/*
 *	��Դ��	http://www.xuebuyuan.com/840043.html
 *
 *	kendall ��ȡֵ��Χ�� [-1, 1] ֮�䣬 ���ڼ�����������������Ե�ͳ��ֵ
 *	ֵΪ 1 ��ʱ���ʾ�����������ӵ��һ�µ������
 *	ֵΪ -1 ��ʱ���ʾ�����������ӵ����ȫ�෴�������
 *	ֵΪ 0 ��ʱ���ʾ������������໥����
 */

KendallAnalysis::KendallAnalysis()
{
	// default src and tar file pref in Windows.
	srcFilePref = "e:\\";
	tarFilePref = "e:\\";
}
KendallAnalysis::KendallAnalysis(const std::string &src, const std::string &tar)
{
	srcFilePref = src; tarFilePref = tar;
}
KendallAnalysis::~KendallAnalysis() { }

/*
 *	�ж����������Ƿ���ϼ���ı�׼
 */
bool KendallAnalysis::judgeTwoSrcFile(const std::string &srcFileOne, const std::string &srcFileTwo)
{
	if (srcFilePref == "" || tarFilePref == "") {
		std::cerr << "srcFilePref or tarFilePref is NULL, need initialize.." << std::endl;
		return false;
	}
	
	const std::string srcFile1 = srcFilePref + srcFileOne;
	const std::string srcFile2 = srcFilePref + srcFileTwo;
	
	unsigned fileOneNode = 0;
	std::fstream inputFile1(srcFile1, std::ios_base::in);
	if (!inputFile1.is_open()) {
		std::cerr << "open file error at: " << srcFile1 << std::endl;
		return false;
	}
	unsigned temp;
	while (inputFile1 >> temp)
		fileOneNode++;
	inputFile1.close();


	unsigned fileTwoNode = 0;
	std::fstream inputFile2(srcFile2, std::ios_base::in);
	if (!inputFile2.is_open()) {
		std::cerr << "open file error at: " << srcFile2 << std::endl;
		return false;
	}
	//unsigned temp;
	while (inputFile2 >> temp)
		fileTwoNode++;
	inputFile2.close();

	// judgement
	if (fileOneNode == fileTwoNode)
		return true;
	else
		return false;
}


/*
 *	kendall ��ʽһ
 *	T = 2 * (C - D) /  (N * (N - 1))
 *	C ��ʾ X Y ��ӵ��һ���Ե�Ԫ�ض���������Ԫ��Ϊһ�ԣ��� D ��ʾ X Y �в�һ�µ�Ԫ�ض���
 *	ע�⣺
 *	��һ����ʽ���������ڼ���X �� Y�о���������ͬԪ�ص�����������и���Ԫ��Ψһ��
 *
 *	һ���ԣ�
 *	(x_i > x_j && y_i > y_j) || (x_i < x_j && y_i < y_j)
 *	��һ���ԣ�
 *	(x_i < x_j && y_i > y_j) || (x_i > x_j && y_i < y_j)
 *
 *	��������Ȳ���һ��Ҳ���ǲ�һ��
 */
bool KendallAnalysis::kendallExpressionOne(const std::string &srcFileOne, const std::string &srcFileTwo
	, const std::string &tarFileName)
{
	if (judgeTwoSrcFile(srcFileOne, srcFileTwo) == false) {
		std::cerr << "src file do not meet the requirment.." << std::endl;
		return false;
	}

	//////////////////////////

	const std::string srcFile1 = srcFilePref + srcFileOne;
	const std::string srcFile2 = srcFilePref + srcFileTwo;

	const std::string tarFile = tarFilePref + tarFileName;

	//////////////////////////////////////////////////////////////

	long long totalNodeNum = 0;
	long long commonPairNum = 0, unCommonPariNum = 0;

	// src vector..
	std::vector<unsigned> fileOneVec;
	std::vector<unsigned> fileTwoVec;

	std::fstream inputFile1(srcFile1, std::ios_base::in);
	if (!inputFile1.is_open())
		return false;
	unsigned temp;
	while (inputFile1 >> temp)
		fileOneVec.push_back(temp);
	inputFile1.close();

	std::fstream inputFile2(srcFile2, std::ios_base::in);
	if (!inputFile2.is_open())
		return false;
	while (inputFile2 >> temp)
		fileTwoVec.push_back(temp);
	inputFile2.close();

	///////////////////////////////////////////////////////////////

	totalNodeNum = fileOneVec.size();

	unsigned vecSize = fileOneVec.size();
	for (unsigned i = 0; i < vecSize; ++i)
		for (unsigned j = i + 1; j < vecSize; ++j) {
			if (fileOneVec.at(i) > fileOneVec.at(j) && fileTwoVec.at(i) > fileTwoVec.at(j))
				commonPairNum++;
			else if (fileOneVec.at(i) < fileOneVec.at(j) && fileTwoVec.at(i) < fileTwoVec.at(j))
				commonPairNum++;
			else if (fileOneVec.at(i) > fileOneVec.at(j) && fileTwoVec.at(i) < fileTwoVec.at(j))
				unCommonPariNum++;
			else if (fileOneVec.at(i) < fileOneVec.at(j) && fileTwoVec.at(i) > fileTwoVec.at(j))
				unCommonPariNum++;
			else
				;
		}

	double firstPart = static_cast<double>(commonPairNum - unCommonPariNum);
	double secondPart = static_cast<double>(totalNodeNum * (totalNodeNum - 1));

	double kendallRes = (2 * firstPart) / secondPart;

	//

	std::fstream outputFile(tarFile, std::ios_base::out);
	if (!outputFile.is_open()) {
		std::cerr << "open output file error: " << tarFile << std::endl;
		return false;
	}

	outputFile << kendallRes << "\n";

	outputFile.close();

	return true;
}

/*
 *	kendall ��ʽ��
 *	T = (C - D) / (sqrt(N3 - N1) * sqrt(N3 - N2))
 *	N3 = (1/2) * N * (N - 1), N1 = sum(i = 1..s) (1/2)*Ui*(Ui - 1), N2 = sum(i = 1..t) (1/2)*Vi*(Vi - 1)
 *	Ui �� Vi ���ص����ϵĴ�С
 *
 *	ע�⣺
 *	��һ����ʽ�����ڼ���X ���� Y �д�����ͬԪ�ص��������X ����Y ֮�о���������ͬԪ��ʱ����ʽ�����ڹ�ʽһ
 */
bool KendallAnalysis::kendallExpressionTwo(const std::string &srcFileOne, const std::string &srcFileTwo
					, const std::string &tarFileName)
{
	if (judgeTwoSrcFile(srcFileOne, srcFileTwo) == false) {
		std::cerr << "src file do not meet the requirment.." << std::endl;
		return false;
	}
	/////////////////////////////////////////////////////

	const std::string srcFile1 = srcFilePref + srcFileOne;
	const std::string srcFile2 = srcFilePref + srcFileTwo;

	const std::string tarFile = tarFilePref + tarFileName;

	/////////////////////////////////////////////////////

	unsigned totalNodeNum = 0;
	long long commonPairNum = 0, unCommonPariNum = 0;				// C and D

	std::vector<unsigned> fileOneVec;
	std::vector<unsigned> fileTwoVec;

	std::fstream inputFile1(srcFile1, std::ios_base::in);
	if (!inputFile1.is_open())
		return false;
	unsigned temp;
	while (inputFile1 >> temp)
		fileOneVec.push_back(temp);
	inputFile1.close();

	std::fstream inputFile2(srcFile2, std::ios_base::in);
	if (!inputFile2.is_open())
		return false;
	while (inputFile2 >> temp)
		fileTwoVec.push_back(temp);
	inputFile2.close();

	///////////////////////////////////////////////////////////////

	totalNodeNum = fileOneVec.size();

	unsigned vecSize = fileOneVec.size();
	for (unsigned i = 0; i < vecSize; ++i)
		for (unsigned j = i + 1; j < vecSize; ++j) {
			if (fileOneVec.at(i) > fileOneVec.at(j) && fileTwoVec.at(i) > fileTwoVec.at(j))
				commonPairNum++;
			else if (fileOneVec.at(i) < fileOneVec.at(j) && fileTwoVec.at(i) < fileTwoVec.at(j))
				commonPairNum++;
			else if (fileOneVec.at(i) > fileOneVec.at(j) && fileTwoVec.at(i) < fileTwoVec.at(j))
				unCommonPariNum++;
			else if (fileOneVec.at(i) < fileOneVec.at(j) && fileTwoVec.at(i) > fileTwoVec.at(j))
				unCommonPariNum++;
			else
				;
		}

	//////////////////////////////////////////////////

	unsigned maxNodeInOne = 0, maxNodeInTwo = 0;
	for (const auto &elem : fileOneVec)
		if (elem > maxNodeInOne)
			maxNodeInOne = elem;
	for (const auto &elem : fileTwoVec)
		if (elem > maxNodeInTwo)
			maxNodeInTwo = elem;

	//////////////////////////////////////////////////

	std::vector<unsigned> timeCountOneVec = std::vector<unsigned>(maxNodeInOne + 1);
	std::vector<unsigned> timeCountTwoVec = std::vector<unsigned>(maxNodeInTwo + 1);

	for (const auto &elem : fileOneVec)
		timeCountOneVec.at(elem)++;
	for (const auto &elem : fileTwoVec)
		timeCountTwoVec.at(elem)++;

	/////////////////////////////////////////////////

	double N3 = 0.5 * static_cast<double>(totalNodeNum * (totalNodeNum - 1));
	double N1 = 0, N2 = 0;

	for (unsigned i = 1; i <= maxNodeInOne; ++i) {
		unsigned temp = timeCountOneVec.at(i);
		if (temp > 1)
			N1 += 0.5 * static_cast<double>(temp * (temp - 1));
	}
			
	for (unsigned j = 1; j <= maxNodeInTwo; ++j) {
		unsigned temp = timeCountTwoVec.at(j);
		if (temp > 1)
			N2 += 0.5 * static_cast<double>(temp * (temp - 1));
	}
	
	//	T = (C - D) / (sqrt(N3 - N1) * sqrt(N3 - N2))
	double firstPart = static_cast<double>(commonPairNum - unCommonPariNum);
	double secondPart = sqrt(static_cast<double>(N3 - N1));
	double thirdPart = sqrt(static_cast<double>(N3 - N2));
	
	double kendallRes = firstPart / (secondPart * thirdPart);

	//double kendall = static_cast<double>(commonPairNum - unCommonPariNum) / 
	//	(sqrt(static_cast<double>(N3 - N1)) * sqrt(static_cast<double>(N3 - N2)));

	std::fstream outputFile(tarFile, std::ios_base::out);
	if (!outputFile.is_open()) {
		std::cerr << "open output file error. at: " << tarFile << std::endl;
		return false;
	}

	//outputFile << "% src file: " << srcFileOne << "\n";
	//outputFile << "% src file: " << srcFileTwo << "\n";
	outputFile << kendallRes << "\n";

	outputFile.close();

	return true;
}
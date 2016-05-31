#include "../../include/kendall/Kendall.h"

#include <fstream>
#include <vector>
#include <map>

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
	unsigned temp;
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
 */
bool KendallAnalysis::kendallExpressionOne(const std::string &srcFileOne, const std::string &srcFileTwo, const std::string &tarFileName)
{
	const std::string srcFile1 = srcFilePref + srcFileOne;
	const std::string srcFile2 = srcFilePref + srcFileTwo;

	const std::string tarFile = tarFilePref + tarFileName;

	if (judgeTwoSrcFile(srcFile1, srcFile2) == false) {
		std::cerr << "src file do not meet the requirment.." << std::endl;
		return false;
	}

	//////////////////////////////////////////////////////////////

	unsigned totalNodeNum = 0;
	unsigned commonPairNum = 0, unCommonPariNum = 0;

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
			else
				unCommonPariNum++;
		}

	double kendallRes = 2 * static_cast<double>(commonPairNum - unCommonPariNum) / static_cast<double>(totalNodeNum * (totalNodeNum - 1));

	//////////////////////////////////////////////////////////////
	std::fstream outputFile(tarFile, std::ios_base::out);
	if (!outputFile.is_open()) {
		std::cerr << "open output file error: " << tarFile << std::endl;
		return false;
	}

	outputFile << "% src file: " << srcFileOne << "\n";
	outputFile << "% src file: " << srcFileTwo << "\n";
	outputFile << kendallRes << "\n";

	outputFile.close();

	return true;
}

/*
 *	kendall ��ʽ��
 *	T = (C - D) / (sqrt(N3 - N1) * sqrt(N3 - N2))
 *	N3 = (1/2) * N * (N - 1), N1 = sum(i = 1..s) (1/2)*Ui*(Ui - 1), N2 = sum(i = 1..t) (1/2)*Vi*(Vi - 1)
 *	ע�⣺
 *	��һ����ʽ�����ڼ���X ���� Y �д�����ͬԪ�ص��������X ����Y ֮�о���������ͬԪ��ʱ����ʽ�����ڹ�ʽһ
 */
bool KendallAnalysis::kendallExpressionTwo(const std::string &srcFileOne, const std::string &srcFileTwo, const std::string &tarFileName)
{
	const std::string srcFile1 = srcFilePref + srcFileOne;
	const std::string srcFile2 = srcFilePref + srcFileTwo;

	const std::string tarFile = tarFilePref + tarFileName;

	if (judgeTwoSrcFile(srcFile1, srcFile2) == false) {
		std::cerr << "src file do not meet the requirment.." << std::endl;
		return false;
	}

	// continue..


	return true;
}
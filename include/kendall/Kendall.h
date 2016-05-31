#ifndef KENDALL_H
#define KENDALL_H

#include <iostream>
#include <string>

class KendallAnalysis {
public:
	KendallAnalysis();
	KendallAnalysis(const std::string &, const std::string &);
	~KendallAnalysis();

	bool judgeTwoSrcFile(const std::string &, const std::string &);

	bool kendallExpressionOne(const std::string &, const std::string &, const std::string &);
	
	bool kendallExpressionTwo(const std::string &, const std::string &, const std::string &);


private:
	KendallAnalysis(const KendallAnalysis &) = delete;
	KendallAnalysis &operator=(const KendallAnalysis &) = delete;

private:
	std::string srcFilePref;
	std::string tarFilePref;
};


#endif
#include "../../include/preMulti/PreMultiAna.h"

PreMultiAnalysis::PreMultiAnalysis() { multiNetNum = 0; }
PreMultiAnalysis::~PreMultiAnalysis() {}

unsigned PreMultiAnalysis::getMultiNetNum() { return multiNetNum; }

bool PreMultiAnalysis::buildMultiNetwork(const std::string &filePref, const std::vector<std::string> &multiNetworks)
{
	multiNetNum = multiNetworks.size();



	return true;
}
bool PreMultiAnalysis::clearMultiNetwork()
{

}

/*
 *	pre multi analysis revoke function
 */
bool preMultiAnalysisRevokeThis()
{



	return true;
}
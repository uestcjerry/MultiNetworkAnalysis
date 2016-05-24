#include "../include/basicDef/BasicStruct.h"
#include "../include/crossLink/CrossLink.h"
#include "../include/userManage/UserManage.h"
#include "../include/basicDef/BasicData.h"
#include "../include/eventAnalysis/EventAnalysis.h"
#include "../include/preMulti/PreMultiAna.h"

#include <iostream>

int main()
{
	// event analysis
	
	if (eventAnalysisRevokeThis() == false)
		return false;
	
	/*
	Time oldday(2016, 5,1, 0,0,0), newday(2016, 5, 31,0,0,0);
	std::cout << distanceDayBetTime(newday, oldday) << std::endl;
	getchar();
	*/

	// pre multilayer analysis
	/*
	if (preMultiAnalysisRevokeThis() == false) {
		getchar();
		return false;
	}
	*/
		

	return 0;
}